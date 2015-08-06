/*
 *	linoleum_linux32 Linoleum Run-Time Module for linux 32-bit systems
 *	Copyright (C) 2004-2006 Peterpaul Klein Haneveld
 *
 *	This program is free software ;  you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation ;  either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program	;  if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <pthread.h>
#include "lino_socket.h"
#include "lino_file.h"

#ifdef pr_network

#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

pthread_t netThread;

int inet_aton(const char *cp, struct in_addr *inp);

#endif				/* pr_network */



#ifdef pr_network
/**
 * binds to socket and listens for new connections
 * @return NULL
 * @param args not used, only needed for pthread_create
 */
void *ll_socket_listen(void *args)
{
	bool error = false;
	socklen_t s_len;
	int sock;
	struct sockaddr_in name;
	int server_socket = (int) pUIWorkspace[mm_Socket];
	unit *client_vector = &pWorkspace[pUIWorkspace[mm_Clients]];

	args = NULL;

	while (!error) {
		s_len = sizeof(name);
		sock = accept(server_socket, (struct sockaddr *) &name, &s_len);
		if (sock == -1) {
			/* error on accept, assertion fails */
			if (errno != EAGAIN) {
				error = true;
				PRINTERROR("accept");
				PRINT("Error accepting :(\n");
			}
		} else {
			/* listen for new connections */
			while (pUIWorkspace[mm_Connections] >=
			       pUIWorkspace[mm_MaxConnections]);

			/* wait untill GODEAF flag is reset */
			while (pUIWorkspace[mm_NetStatus] & GODEAF);
			/* new socket accepted */
			client_vector[pUIWorkspace[mm_Connections]] = sock;
			pUIWorkspace[mm_Connections]++;
		}
	}

	return NULL;
}
#endif

bool initNetCommand(void)
{
#ifdef pr_network
	pUIWorkspace[mm_NetStatus] = READY;
#else
	pUIWorkspace[mm_NetStatus] = 0;
#endif

	pUIWorkspace[mm_Connections] = 0;
	return true;
}

/**
 * handles all new commands.
 * @return 1 when errors, 0 otherwise
 */
bool krnlNetCommand(NetCommand command)
{

	bool result = true;

#ifdef pr_network
	int name_len;
	int addr_len;
	int sock = 0;
	socklen_t s_len = 0;
	char *hostname;
	char *hostaddr;
	struct sockaddr_in name;
	struct hostent *hostinfo;

	struct timeval sto;	/* for NETISEXCEPTED, NETISREADABLE and NETISWRITABLE */
	fd_set fds;		/* for NETISEXCEPTED, NETISREADABLE and NETISWRITABLE */

	unit *client_vector;

	if (command != IDLE) {
		PRINT1("Net Command: %u\n", command);
	}

	switch (command) {
	case IDLE:
		break;
	case GETHOSTBYNAME:
		name_len = ustrlen(&pWorkspace[pUIWorkspace[mm_HostName]]);
		hostname = (char *) malloc(name_len + 1);
		utrsstring(hostname, &pWorkspace[pUIWorkspace[mm_HostName]]);
		hostinfo = gethostbyname(hostname);
		free(hostname);
		if (hostinfo == NULL) {
			result = false;
			pUIWorkspace[mm_NetStatus] = (NETFAILURE | READY);
			break;
		}
		memcpy(&(name.sin_addr.s_addr), hostinfo->h_addr,
		       hostinfo->h_length);
		btrsstring((unit *) & pWorkspace[pUIWorkspace[mm_HostAddress]],
			   inet_ntoa(name.sin_addr));
		pUIWorkspace[mm_NetStatus] = (NETSUCCESS | READY);
		break;
	case GETHOSTBYADDR:{
		struct in_addr h_addr;
		addr_len =
			ustrlen(&pWorkspace[pUIWorkspace[mm_HostAddress]]);
		hostaddr = (char *) malloc(addr_len + 1);
		utrsstring(hostaddr,
			   &pWorkspace[pUIWorkspace[mm_HostAddress]]);
		if (inet_aton(hostaddr, &h_addr) == 0) {
			free(hostaddr);
			result = false;
			pUIWorkspace[mm_NetStatus] =
				(NETFAILURE | READY);
			break;
		}
		free(hostaddr);
		hostinfo =
			gethostbyaddr(&h_addr, sizeof(h_addr), AF_INET);
		if (hostinfo == NULL) {
			result = false;
			pUIWorkspace[mm_NetStatus] =
				(NETFAILURE | READY);
			break;
		}
		btrsstring(&pWorkspace[pUIWorkspace[mm_HostName]],
			   hostinfo->h_name);
		pUIWorkspace[mm_NetStatus] = (NETSUCCESS | READY);
		break;
	}
	case GETPEERBYSOCKET:
		s_len = sizeof(name);
		if (getpeername
		    ((int) pUIWorkspace[mm_Socket], (struct sockaddr *) &name,
		     &s_len) < 0) {
			PRINTERROR("getpeername");
			result = false;
			break;
		}
		if (name.sin_family != AF_INET) {
			result = false;
			break;
		}
		pUIWorkspace[mm_Port] = name.sin_port;
		btrsstring((unit *)
			   & pWorkspace[pUIWorkspace[mm_HostAddress]],
			   inet_ntoa(name.sin_addr));
		break;
	case CANCELREQUEST:
		/* since the GETHOSTBY(NAME)(ADDR) are implemented synchronically,
		 * this routine isn't needed, and always returns sucessfully.
		 */
		break;
	case NETOPEN:
		/* create socket */
		sock = socket(PF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			PRINTERROR("socket");
			result = false;
			break;
		}
		/* set socket options */
		if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK) <
		    0) {
			PRINTERROR("fcntl");
			result = false;
			break;
		}
		pUIWorkspace[mm_Socket] = sock;
		break;
	case NETCLOSE:{
		int i;

		/* make network command deaf */
		pUIWorkspace[mm_NetStatus] &= !GODEAF;
		if (close((int) pUIWorkspace[mm_Socket]) < 0) {
			PRINTERROR("shutdown");
			result = false;
		}
		/* find if socket in client vector */
		client_vector = &pWorkspace[pUIWorkspace[mm_Clients]];
		sock = pUIWorkspace[mm_Socket];
		/* start search */
		i = 0;
		while ((client_vector[i] != sock)
		       && (i < pUIWorkspace[mm_Connections]))
			i++;

		if (i == pUIWorkspace[mm_Connections]) {
			PRINT1("Socket (%u) not in client vector.\n",
			       sock);
			result++;
		} else {
			/* remove socket from client vector */
			memmove(&client_vector[i],
				&client_vector[i + 1],
				(pUIWorkspace[mm_Connections] - i -
				 1) * sizeof(unit));
			pUIWorkspace[mm_Connections]--;
		}

		pUIWorkspace[mm_NetStatus] |= GODEAF;
		break;
	}
	case NETCONNECT:
		/** in: [Socket]
		 *      [HostAddress]
		 *      [Port]
		 * fails or ends
		 * stat_NETISWRITABLE
		 * stat_NETISEXCEPTED
		 */
		addr_len = ustrlen(&pWorkspace[pUIWorkspace[mm_HostAddress]]);
		hostaddr = (char *) malloc(addr_len + 1);
		utrsstring(hostaddr, &pWorkspace[pUIWorkspace[mm_HostAddress]]);
		if (!(inet_aton(hostaddr, &name.sin_addr))) {
			result = false;
			free(hostaddr);
			break;
		}
		free(hostaddr);
		name.sin_family = AF_INET;
		name.sin_port = htons((uint16_t) pUIWorkspace[mm_Port]);
		if (connect
		    ((int) pUIWorkspace[mm_Socket], (struct sockaddr *) &name,
		     sizeof(name)) == -1 && (errno != EINPROGRESS)) {
			/* possibly check for the cause of error. (errno) */
			PRINTERROR("connect");
			result = false;
			break;
		}
		break;
	case NETLISTEN:
		/* get the length of the [Host Address] string. */
		addr_len = ustrlen(&pWorkspace[pUIWorkspace[mm_HostAddress]]);
		hostaddr = (char *) malloc(addr_len + 1);
		utrsstring(hostaddr, &pWorkspace[pUIWorkspace[mm_HostAddress]]);
		if (!(inet_aton(hostaddr, &name.sin_addr))) {
			result = false;
			free(hostaddr);
			break;
		}
		free(hostaddr);
		name.sin_family = AF_INET;
		name.sin_port = htons((uint16_t) pUIWorkspace[mm_Port]);
		if (bind
		    ((int) pUIWorkspace[mm_Socket], (struct sockaddr *) &name,
		     sizeof(name)) < 0) {
			PRINTERROR("bind");
			result = false;
			break;
		}

		if (listen
		    ((int) pUIWorkspace[mm_Socket],
		     pUIWorkspace[mm_MaxConnections]) < 0) {
			PRINTERROR("listen");
			result = false;
			break;
		}

		if (pthread_create(&netThread, NULL, ll_socket_listen, NULL) ==
		    0) {
			PRINT("Net is listening...\n");
			/* successfully initialized, */
		} else {
			result = false;
		}
		break;
	case NETSEND:{
		ssize_t s_size = send((int) pUIWorkspace[mm_Socket],
				      &pWorkspace[pUIWorkspace
						  [mm_NetBlockPointer]],
				      pUIWorkspace[mm_NetBlockSize], 0);
		if (s_size == -1) {
			PRINTERROR("send");
			pUIWorkspace[mm_NetBlockSize] = 0;
			result = false;
			break;
		}
		pUIWorkspace[mm_NetBlockSize] = s_size;
	}
		break;
	case NETRECV:{
		ssize_t s_size = recv((int) pUIWorkspace[mm_Socket],
				      &pWorkspace[pUIWorkspace
						  [mm_NetBlockPointer]],
				      pUIWorkspace[mm_NetBlockSize], 0);
		if (s_size == -1) {
			PRINTERROR("recv");
			pUIWorkspace[mm_NetBlockSize] = 0;
			result = false;
			break;
		}
		pUIWorkspace[mm_NetBlockSize] = s_size;
	}
		break;
	case NETISREADABLE:
		sto.tv_sec = 0;
		sto.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET((int) pUIWorkspace[mm_Socket], &fds);
		if (select
		    ((int) pUIWorkspace[mm_Socket] + 1, &fds, NULL, NULL, &sto)
		    != 1)
			result = false;
		break;
	case NETISWRITABLE:
		sto.tv_sec = 0;
		sto.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET((int) pUIWorkspace[mm_Socket], &fds);
		if (select
		    ((int) pUIWorkspace[mm_Socket] + 1, NULL, &fds, NULL, &sto)
		    != 1)
			result = false;
		break;
	case NETISEXCEPTED:
		sto.tv_sec = 0;
		sto.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET((int) pUIWorkspace[mm_Socket], &fds);
		if (select
		    ((int) pUIWorkspace[mm_Socket] + 1, NULL, NULL, &fds, &sto)
		    != 1)
			result = false;
		break;
	default:
		result = false;
		break;
	}
#else
	/* #ifndef pr_network */
	result = false;
	PRINT("Net Command not enabled in RTM.\n");
#endif				/* pr_network */

	return result;
}

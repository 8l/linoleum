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

#ifndef __RTM_H
#define __RTM_H

#define pr_network

#define _POSIX_C_SOURCE 199309

/* global includes */
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>

#include "lino_types.h"
#include "lino_kernel.h"

/* toggle this define to include debug output. */
/* #define DEBUG */

#ifndef DEBUG
#define PRINT(x)
#define PRINT1(x, a1)
#define PRINT2(x, a1, a2)
#define PRINT3(x, a1, a2, a3)
#define PRINTERROR(x)
#else
#define PRINT(x)               printf(x)
#define PRINT1(x, a1)          printf(x, a1);
#define PRINT2(x, a1, a2)      printf(x, a1, a2)
#define PRINT3(x, a1, a2, a3)  printf(x, a1, a2, a3)
#define PRINTERROR(x)          perror(x)
#endif

/* global definitions */
struct LNLMINIT {		/* LNLM initialization structure */
	unsigned char appname[40];	/* application name, zero terminated */
	unit app_ws_size;	/* size of main workspace (units) */
	unit app_code_size;	/* sz.of main & libraries code (units) */
	unit app_code_entry;	/* entry to main "programme" (units) */
	unit physwsentry;	/* pratically, byte size of r-t mod. */
	unit physappsize;	/* byte size of final executable file */
	unit default_ramtop;	/* total size of workspace (units) */
	unit app_code_pri;	/* thread priority (0 to 5) */
	unit lfb_x_atstartup;	/* display x pos. at startup (pixels) */
	unit lfb_y_atstartup;	/* display y pos. at startup (pixels) */
	unit lfb_w_atstartup;	/* display width at startup (pixels) */
	unit lfb_h_atstartup;	/* display height at startup (pixels) */
	unit pointermode;	/* pointing device operating mode */
	unit testflags;		/* debug/test flags */
	unit displaymode;	/* display mode (0=coop./1=exl./2=h-w) */
};

extern unit *pUIWorkspace;
extern unit *pWorkspace;
extern unit current_ramtop;
extern char dmsStockFilename[32768];	/* Name of THIS executable file */
extern char **environment;
extern struct LNLMINIT *IParagraph;
extern unit dmsFileOrigin;

void ISOKRNLCALL(void);

bool krnlPCMdataCommand(PCMdataCommand);
bool krnlConsoleCommand(ConsoleCommand);
bool krnlFileCommand(FileCommand);
bool krnlSYStimeCommand(SYStimeCommand);
bool krnlAPDCommand(APDCommand);
bool krnlPrinterCommand(PrinterCommand);
bool krnlProcessCommand(ProcessCommand);
bool krnlGlobalKCommand(GlobalKCommand);
bool krnlDisplayCommand(DisplayCommand);
bool krnlPointerCommand(PointerCommand);
bool krnlNetCommand(NetCommand);
bool krnlClipCommand(ClipCommand);

/* function prototypes */
void programError(char *);
void init_section(unit **, int, const char *);
void read_section(unit **, int, int, const char *);

char *realpath(const char *path, char *resolved_path);
int snprintf(char *str, size_t size, const char *format, ...);
int truncate(const char *path, off_t length);

#endif

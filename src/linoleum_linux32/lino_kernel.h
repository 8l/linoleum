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

#ifndef LINO_KERNEL_H
#define LINO_KERNEL_H

#define IDLE 0
#define mm_ProcessOrigin 0

/* layout of isokernel communication section */
enum {
	mm_ProcessISOcall = 0,
	mm_ProcessRAMtop,
	mm_ProcessPriority,
	mm_ProcessCommandLine,

	mm_DisplayCommand = 32771,
	mm_DisplayStatus,
	mm_DisplayOrigin,
	mm_DisplayWidth,
	mm_DisplayHeight,
	mm_DisplayPhysicalWidth,
	mm_DisplayPhysicalHeight,
	mm_DisplayXPosition,
	mm_DisplayYPosition,
	mm_DisplayLiveRegion,

	mm_PCMdataCommand,	/* = 32781 */
	mm_PCMdataStatus,
	mm_PCMdataCHannels,
	mm_PCMdataBitsPerSample,
	mm_PCMdataSamplesPerSec,
	mm_PCMdataSilenceThreshold,
	mm_PCMdataOrigin,
	mm_PCMdataOffset,
	mm_PCMdataSize,

	mm_ConsoleCommand,	/* = 32790 */
	mm_ConsoleInput,
	mm_ConsoleOrigin,
	/* LUCK Table here */

	mm_PointerCommand = 32890,
	mm_PointerMode,
	mm_PointerStatus,
	mm_PointerDeltaX,
	mm_PointerDeltaY,
	mm_PointerDeltaZ,
	mm_PointerXCoordinate,
	mm_PointerYCoordinate,
	mm_PointerZCoordinate,

	mm_FileCommand,		/* = 32899 */
	mm_FileStatus,
	mm_BlockPointer,
	mm_BlockSize,
	mm_FileName,
	mm_FileSize,
	mm_FilePosition,

	mm_SYStimeCommand,	/* 32906 */
	mm_SYStimeYear,
	mm_SYStimeMonth,
	mm_SYStimeDay,
	mm_SYStimeDayOfWeek,
	mm_SYStimeHour,
	mm_SYStimeMinute,
	mm_SYStimeSecond,
	mm_SYStimeMilliSeconds,
	mm_SYStimeCounts,
	mm_CountsPerMillisecond,

	mm_APDCommand,		/* 32917 */
	mm_APDLine,
	mm_APDLines,
	mm_APDStatus,
	mm_APDXMeter,
	mm_APDYMeter,
	mm_APDZMeter,

	mm_PrinterCommand,	/* 32924 */
	mm_PageOrigin,
	mm_PageWidth,
	mm_PageHeight,

	mm_ProcessCommand,	/* 32928 */
	mm_SleepTimeout,

	mm_NetCommand,		/* 32930 */
	mm_NetStatus,
	mm_Socket,
	mm_HostName,
	mm_HostAddress,
	mm_Port,
	mm_NetBlockPointer,
	mm_NetBlockSize,
	mm_MaxConnections,
	mm_Connections,
	mm_Clients,

	mm_GlobalKCommand,	/* 32941 */
	mm_GlobalKName,
	mm_GlobalKData,

	mm_ClipCommand,		/* 32944 */
	mm_ClipSize,
	mm_ClipString
};

/* Commands */
typedef enum {
	RETRACE = 1,
	SETCOOPERATIVEMODE,
	SETEXCLUSIVEMODE
} DisplayCommand;

typedef enum {
	GETDATAOFFSET = 4,
	PLAYONCE,
	PLAYCONTINUOUSLY,
	_PAUSE,
	_UNPAUSE,
	_STOP
} PCMdataCommand;

typedef enum {
	GETCONSOLEINPUT = 10,
	CLEARCONSOLEBUFFER
} ConsoleCommand;

typedef enum {
	READPOINTER = 12
} PointerCommand;

typedef enum {
	_TEST = 13,
	READ,
	WRITE,
	SET_SIZE,
	DESTROY,
	_RUN,
	SET_DIR,
	GET_DIR,
	MKDIR,
	RMDIR,
	GET_FIRST_FILE,
	GET_NEXT_FILE,
	GET_FIRST_DIR,
	GET_NEXT_DIR
} FileCommand;

typedef enum {
	READTIME = 27,
	READUTCTIME,
	READCOUNTS
} SYStimeCommand;

typedef enum {
	QUERYAPDLINES = 30,
	READAPDLINE
} APDCommand;

typedef enum {
	PRINTPAGE = 32
} PrinterCommand;

typedef enum {
	ENDPROCESS = 33,
	FAILPROCESS,
	_SLEEP
} ProcessCommand;

typedef enum {
	GETHOSTBYNAME = 36,
	GETHOSTBYADDR,
	GETPEERBYSOCKET,
	CANCELREQUEST,
	NETOPEN,
	NETCLOSE,
	NETCONNECT,
	NETLISTEN,
	NETSEND,
	NETRECV,
	NETISREADABLE,
	NETISWRITABLE,
	NETISEXCEPTED
} NetCommand;

typedef enum {
	KREAD = 49,
	KWRITE,
	KDESTROY
} GlobalKCommand;

typedef enum {
	GETCLIPSIZE = 52,
	READCLIP,
	WRITECLIP
} ClipCommand;

/* command specific defines */
typedef enum {
	COOPERATIVE = 0,
	EXCLUSIVE,
	ACTIVE
} DisplayStatus;

typedef enum {
	MIDDLE = 1048577
} DisplayPosition;

typedef enum {
	WHOLEDISPLAY = 0,
	VOIDREGION = -1
} DisplayRegion;

typedef enum {
	PCMREADY = 1,
	PCMPAUSED
} PCMdataStatus;

typedef enum {
	MP_BYCOORDINATE = 0x0,
	MP_BYDELTA = 0x1
} PointerMode;

typedef enum {
	MP_PRESENT = 0x1,
	MP_INSIGHT = 0x2,
	MP_LBUTTONDOWN = 0x4,
	MP_RBUTTONDOWN = 0x8,
	MP_MBUTTONDOWN = 0x10
} PointerStatus;

typedef enum {
	FILEREADY = 0x1,
	FILEERROR = 0x2,
	FILEPERMIT_TO_READ = 0x4,
	FILEPERMIT_TO_WRITE = 0x8
} FileStatus;

#define STD_CONSOLE
typedef enum {
	STOCKFILE = 0,
#ifdef STD_CONSOLE
	STDIN = -1,
	STDOUT = -2,
	STDERR = -3
#endif
} FileHandles;

typedef enum {
	READY = 0x1,
	GODEAF = 0x2,
	NETSUCCESS = 0x4,
	NETFAILURE = 0x8
} NetStatus;

#endif				/* LINO_KERNEL_H */

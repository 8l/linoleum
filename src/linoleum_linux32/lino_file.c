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

/*
 * This file contains several specific extensions:
 * - When writing to files containing .bin, this file is marked as executable
 * - Support for writing to stderr and stdout, and reading from stdin
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <dirent.h>

#include <wordexp.h>

#include "lino_file.h"

DIR *hDir;
int hFile;
struct stat fileStatus;
struct dirent *entry;
char *directory;
char *filename;
char dmsfilename[32768];
unit dmsFileOrigin;		/* Base byte for file (0 for normal files). */

/**
 * get the file size of the file in filename
 * @return the file size on success, -1 on error
 */
long getFileSize(void)
{
	if (stat(filename, &fileStatus) == 0) {
		if (S_ISREG(fileStatus.st_mode))
			return (long) fileStatus.st_size;
	} else {
		PRINTERROR("stat");
	}
	return -1;
}

/**
 * get the file size of the file in filename
 * @return the file size on success, -1 on error
 */
long getFileSizeF(void)
{
	if (fstat(hFile, &fileStatus) == 0) {
		if (S_ISREG(fileStatus.st_mode))
			return (long) fileStatus.st_size;
	} else {
		PRINTERROR("stat");
	}
	return -1;
}

/**
 * translates a byte string to a unit string
 * @param to buffer to store the unit string
 * @param from the character string to convert
 * @return a pointer to the resulting unit string
 */
unit *btrsstring(unit * to, const char *from)
{
	int i = 0;
	while ((to[i] = (unit) from[i]))
		i++;
	return (to);
}

/**
 * translates a unit string to a byte string
 * @param to buffer to store the character string
 * @param from the unit string to convert
 * @return a pointer to the resulting character string
 */
char *utrsstring(char *to, const unit * from)
{
	int i = 0;
	while ((to[i] = (char) from[i]))
		i++;
	return (to);
}

/**
 * counts the number of characters in a unit string
 * @param str the unit string to get the length of
 * @return the number of characters in the string not including the string terminator
 */
size_t ustrlen(const unit * str)
{
	size_t len = 0;
	while (str[len] != 0) {
		len++;
	}
	return len;
}

/**
 * translates a unit symbolic filename to a character filename
 * this function never fails, although the results differ,
 * when there is either a memory error, or an error in the realpath
 * routine, the string is just copied.
 * @param dest character pointer where to store the result
 * @param src unit pointer to source string
 */
void lino_file_realpath(const unit * src)
{
	int i;
	const unit *start = src;
	if (src[0] == (unit) '~') {
		i = 0;
		/* find HOME environment variable */
		while (environment[i] != NULL) {
			if (strncmp(environment[i], "HOME=", 5) == 0) {
				strcpy(dmsfilename, environment[i] + 5);
				start++;
				break;
			}
			i++;
		}
	} else
		dmsfilename[0] = '\0';

	utrsstring(dmsfilename + strlen(dmsfilename), start);
}

/**
 * handles all file commands.
 * @return 0 on success, a positive integer otherwise
 */
bool krnlFileCommand(FileCommand command)
{
	bool result = true;

	/* reserve memory for result */
	wordexp_t cmd;
	mode_t w_mode;
	/* find the separator of command and parameters */
	char *params;

	pUIWorkspace[mm_FileStatus] = 0;

	if (command != IDLE) {
		PRINT1("File Command(%d): ", command);

		/* read file into buffer */
		switch (pUIWorkspace[mm_FileName]) {
		case STOCKFILE:
			filename = dmsStockFilename;
			dmsFileOrigin = IParagraph->physappsize;
			PRINT1("stockfile: %s\n", filename);
			break;
#ifdef STD_CONSOLE
		case STDIN:
		case STDOUT:
		case STDERR:
			filename = NULL;
			dmsFileOrigin = 0;
			break;
#endif
		default:
			lino_file_realpath(&pWorkspace
					   [pUIWorkspace[mm_FileName]]);
			filename = dmsfilename;
			dmsFileOrigin = 0;
			PRINT1("file: %s\n", filename);
			break;
		}
	}

	switch (command) {
	case IDLE:
		break;
	case _TEST:
		PRINT1("Test file: %s\n", filename);
		/* read flags */
		if (access(filename, F_OK) == -1) {
			PRINTERROR("access");
			result = false;
			break;
		} else {
			pUIWorkspace[mm_FileStatus] |= FILEREADY;
		}
		if (access(filename, R_OK) == 0)
			pUIWorkspace[mm_FileStatus] |= FILEPERMIT_TO_READ;
		if (access(filename, W_OK) == 0)
			pUIWorkspace[mm_FileStatus] |= FILEPERMIT_TO_WRITE;
		if ((pUIWorkspace[mm_FileSize] = getFileSize()) == -1)
			result = false;
		break;
	case READ:
#ifdef STD_CONSOLE
		if (pUIWorkspace[mm_FileName] == STDIN) {
			if ((pUIWorkspace[mm_BlockSize] =
			     read(0,
				  &pWorkspace[pUIWorkspace[mm_BlockPointer]],
				  pUIWorkspace[mm_BlockSize])) == -1) {
				PRINTERROR("read");
				result = false;
				break;
			}
		} else {
#endif
			/* open file for reading */
			PRINT1("Read from file: %s\n", filename);
			if ((hFile = open(filename, O_RDONLY)) == -1) {
				PRINTERROR("open");
				result = false;
				break;
			}
			pUIWorkspace[mm_FileStatus] |= FILEPERMIT_TO_READ;
			/* set pointer in file */
			dmsFileOrigin += pUIWorkspace[mm_FilePosition];
			if (lseek(hFile, (off_t) dmsFileOrigin, SEEK_SET) == -1) {
				PRINTERROR("lseek");
				result = false;
				close(hFile);
				break;
			}
			/* read data from file */
			if ((pUIWorkspace[mm_BlockSize] = read(hFile,
							       &pWorkspace
							       [pUIWorkspace
								[mm_BlockPointer]],
							       pUIWorkspace
							       [mm_BlockSize]))
			    == -1) {
				PRINTERROR("read");
				result = false;
			}
			/* get file length */
			if ((pUIWorkspace[mm_FileSize] = getFileSizeF()) == -1)
				result = false;
			close(hFile);
#ifdef STD_CONSOLE
		}
#endif
		break;
	case WRITE:
#ifdef STD_CONSOLE
		/* check if write to stdout or stderr */
		if (pUIWorkspace[mm_FileName] == STDOUT) {
			if ((pUIWorkspace[mm_BlockSize] =
			     write(1,
				   &pWorkspace[pUIWorkspace[mm_BlockPointer]],
				   pUIWorkspace[mm_BlockSize])) == -1) {
				PRINTERROR("write");
				result = false;
				break;
			}
		} else if (pUIWorkspace[mm_FileName] == STDERR) {
			if ((pUIWorkspace[mm_BlockSize] =
			     write(2,
				   &pWorkspace[pUIWorkspace[mm_BlockPointer]],
				   pUIWorkspace[mm_BlockSize])) == -1) {
				PRINTERROR("write");
				result = false;
				break;
			}
		} else {
#endif
			PRINT1("Write to file: %s\n", filename);
			if (strstr(filename, ".bin") == NULL)
				w_mode = 0644;
			else
				w_mode = 0755;
			/* open file for writing */
			if ((hFile =
			     open(filename, O_WRONLY | O_CREAT,
				  w_mode)) == -1) {
				PRINTERROR("open");
				result = false;
				break;
			}
			pUIWorkspace[mm_FileStatus] |= FILEPERMIT_TO_WRITE;
			/* set pointer in file */
			dmsFileOrigin += pUIWorkspace[mm_FilePosition];
			if (lseek(hFile, (off_t) dmsFileOrigin, SEEK_SET) == -1) {
				PRINTERROR("lseek");
				result = false;
				close(hFile);
				break;
			}
			/* write data to file */
			if ((pUIWorkspace[mm_BlockSize] =
			     write(hFile,
				   &pWorkspace[pUIWorkspace[mm_BlockPointer]],
				   pUIWorkspace[mm_BlockSize])) == -1) {
				PRINTERROR("write");
				result = false;
			}
			/* get file length */
			else if ((pUIWorkspace[mm_FileSize] =
				  getFileSizeF()) == -1)
				result = false;
			close(hFile);
#ifdef STD_CONSOLE
		}
#endif
		break;
	case SET_SIZE:
		PRINT1("Set file size: %s\n", filename);
		if (truncate(filename, pUIWorkspace[mm_FileSize]) == -1) {
			PRINTERROR("truncate");
			result = false;
		}
		break;
	case DESTROY:
		PRINT1("Destroy file: %s\n", filename);
		if (unlink(filename) == -1) {
			PRINTERROR("unlink");
			result = false;
		}
		break;
	case _RUN:{
		unsigned i;
		pid_t newProcess;

		PRINT1("Run command: %s\n", filename);
		params = strstr(filename, "||");
		if (params != NULL) {
			/* parameters */
			strncpy(params, "  ", 2);
		}

		if (wordexp(filename, &cmd, 0)) {
			PRINTERROR("wordexp");
		}

		PRINT1("Run program: %s\n", cmd.we_wordv[0]);
		PRINT("Parameters:\n");
		for (i = 1; i < cmd.we_wordc; i++) {
			PRINT2("param %d: %s\n", i, cmd.we_wordv[i]);
		}

		newProcess = fork();
		if (newProcess == -1) {
			result = false;
			PRINTERROR("fork");
		} else if (newProcess == 0) {
			/* inside child process */
			if (execv(cmd.we_wordv[0], cmd.we_wordv) == -1) {
				PRINTERROR("execve");
				exit(1);
			}
		} else {
			/* inside main process */
			PRINT(":)\n");
		}
		break;
	}
	case SET_DIR:
		PRINT1("Set directory: %s\n", filename);
		if (chdir(filename) == -1) {
			PRINTERROR("chdir");
			result = false;
		}
		break;
	case GET_DIR:
		PRINT("Get current directory.\n");
		directory = getcwd(NULL, 0);
		if (directory == NULL) {
			PRINTERROR("getcwd");
			result = false;
			break;
		}
		btrsstring((unit *) & pWorkspace[pUIWorkspace[mm_FileName]],
			   directory);
		free(directory);
		break;
	case MKDIR:
		PRINT1("Make directory: %s\n", filename);
		if (mkdir(filename, 0755) == -1) {
			PRINTERROR("mkdir");
			result = false;
		}
		break;
	case RMDIR:
		PRINT1("Remove directory: %s\n", filename);
		if (rmdir(filename) == -1) {
			PRINTERROR("rmdir");
			result = false;
		}
		break;
	case GET_FIRST_FILE:
		PRINT("Get first file.\n");
		/* get string with path of current directory */
		directory = getcwd(NULL, 0);
		if (directory == NULL) {
			PRINTERROR("getcwd");
			result = false;
			break;
		}
		/* open current directory */
		hDir = opendir(directory);
		if (!hDir) {
			PRINTERROR("opendir");
			result = false;
			free(directory);
			break;
		}
		/* read the directory's contents, print out the name of each entry. */
		do {
			entry = readdir(hDir);
			if (entry != NULL) {
				/* get statistics of file */
				if (stat(entry->d_name, &fileStatus) != 0) {
					PRINTERROR("stat");
					result = false;
				}
			} else {
				result = false;
				/* close directory */
				if (closedir(hDir) == -1) {
					PRINTERROR("closedir");
				}
			}
		}
		while (entry != NULL && !(S_ISREG(fileStatus.st_mode)));
		if (result == true && S_ISREG(fileStatus.st_mode)) {
			btrsstring((unit *)
				   & pWorkspace[pUIWorkspace[mm_FileName]],
				   entry->d_name);
		} else {
			PRINT("No file found.\n");
			result = false;
		}
		free(directory);
		break;
	case GET_NEXT_FILE:
		PRINT("Get next file.\n");
		/* read the directory's contents, print out the name of each entry. */
		do {
			entry = readdir(hDir);
			if (entry != NULL) {
				/* get statistics of file */
				if (stat(entry->d_name, &fileStatus) != 0) {
					PRINTERROR("stat");
					result = false;
				}
			} else {
				result = false;
				/* close directory */
				if (closedir(hDir) == -1) {
					PRINTERROR("closedir");
				}
			}
		}
		while (entry != NULL && !(S_ISREG(fileStatus.st_mode)));
		if (result == true && S_ISREG(fileStatus.st_mode)) {
			btrsstring((unit *)
				   & pWorkspace[pUIWorkspace[mm_FileName]],
				   entry->d_name);
		} else {
			result = false;
		}
		break;
	case GET_FIRST_DIR:
		PRINT("Get first directory.\n");
		/* get string with path of current directory */
		directory = getcwd(NULL, 0);
		if (directory == NULL) {
			PRINTERROR("getcwd");
			result = false;
			break;
		}
		/* open current directory */
		hDir = opendir(directory);
		if (!hDir) {
			PRINTERROR("opendir");
			result = false;
			free(directory);
			break;
		}
		/* read the directory's contents, print out the name of each entry. */
		do {
			entry = readdir(hDir);
			if (entry != NULL) {
				/* get statistics of file */
				if (stat(entry->d_name, &fileStatus) != 0) {
					PRINTERROR("stat");
					result = false;
				}
			} else {
				result = false;
				/* close directory */
				if (closedir(hDir) == -1) {
					PRINTERROR("closedir");
				}
			}
		}
		while (entry != NULL && !(S_ISDIR(fileStatus.st_mode)));
		if (result == true && S_ISDIR(fileStatus.st_mode)) {
			btrsstring((unit *)
				   & pWorkspace[pUIWorkspace[mm_FileName]],
				   entry->d_name);
		} else {
			result = false;
		}
		free(directory);
		break;
	case GET_NEXT_DIR:
		PRINT("Get next directory.\n");
		/* read the directory's contents, print out the name of each entry. */
		do {
			entry = readdir(hDir);
			if (entry != NULL) {
				/* get statistics of file */
				if (stat(entry->d_name, &fileStatus) != 0) {
					PRINTERROR("stat");
					result = false;
				}
			} else {
				result = false;
				/* close directory */
				if (closedir(hDir) == -1) {
					PRINTERROR("closedir");
				}
			}
		}
		while (entry != NULL && !(S_ISDIR(fileStatus.st_mode)));
		if (result == true && S_ISDIR(fileStatus.st_mode)) {
			btrsstring((unit *)
				   & pWorkspace[pUIWorkspace[mm_FileName]],
				   entry->d_name);
		} else {
			result = false;
		}
		break;
	default:
		PRINT("Undefined File Command.\n");
		result = false;
		break;
	}
	/* check result */
	if (result == false) {
		pUIWorkspace[mm_FileStatus] |= FILEERROR;
		pUIWorkspace[mm_FileSize] = 0;
	} else if (command < DESTROY && command != IDLE) {
		pUIWorkspace[mm_FileStatus] |= FILEREADY;
	}

	return result;
}

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
#include <unistd.h>
#include <ctype.h>

#include "lino_globalK.h"
#include "rtm.h"
#include "lino_file.h"

char *globalK_path = "~/linoleum/.k/";

unit *globalK_name;
unit globalK_filename[50];

/**
 * Converts a character to a valid globalK name character.
 * @param u Character to convert
 * @return '_' if u isn't alphanumeric, u otherwise
 */
static unit globalK_name_chr(unit u)
{
	if (u && !isalnum(u))
		return '_';
	return u;
}

/** copies a global k name to another location, and checks
 * and converts the name to a valid global K name
 * @param to target buffer to hold global K name
 * @param from source buffer
 * @return a pointer to the target buffer
 */
unit *lino_globalK_name_copy(unit * to, const unit * from)
{
	int i = 0;

	while (i < 24 && (to[i] = globalK_name_chr(from[i])))
		i++;

	if (i == 24)
		to[24] = '\0';

	return to;
}

/**
 * handles all Global K commands.
 * @return 1 when errors, 0 otherwise
 */
bool krnlGlobalKCommand(GlobalKCommand command)
{
	/* declarations */
	FILE *kfile;
	bool result = true;
	int globalK_units;

	if (command != IDLE) {
		/* translate the path to a unit string */
		btrsstring(globalK_filename, globalK_path);
		/* get a pointer to where the name will be stored */
		globalK_name = &globalK_filename[ustrlen(globalK_filename)];
		/* copy and check the name */
		lino_globalK_name_copy(globalK_name,
				       &pWorkspace[pUIWorkspace
						   [mm_GlobalKName]]);
		/* get the realpath of this file */
		lino_file_realpath(globalK_filename);
	}

	switch (command) {
	case IDLE:
		break;
	case KREAD:
		/* open global K file for reading */
		kfile = fopen(dmsfilename, "r");
		if (!kfile) {
			result = false;
			break;
		}
		/* read the data into the workspace */
		globalK_units =
		    fread(&pWorkspace[pUIWorkspace[mm_GlobalKData]],
			  sizeof(unit), 255, kfile);
		if (globalK_units != 255) {
			result = false;
		}
		/* close the file */
		fclose(kfile);
		break;
	case KWRITE:
		/* open global K file for writing */
		kfile = fopen(dmsfilename, "w");
		if (!kfile) {
			result = false;
			break;
		}
		/* write the data into the file */
		globalK_units =
		    fwrite(&pWorkspace[pUIWorkspace[mm_GlobalKData]],
			   sizeof(unit), 255, kfile);
		if (globalK_units != 255) {
			result = false;
		}
		/* close the file */
		fclose(kfile);
		break;
	case KDESTROY:
		if (unlink(dmsfilename) == -1) {
			result = false;
		}
		break;
	default:
		result = false;
		break;
	}

	pUIWorkspace[mm_GlobalKCommand] = IDLE;
	return result;
}

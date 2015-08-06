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

#include "rtm.h"
#include "lino_keyboard.h"

#define LINO_CONSOLE_BUFFER_SIZE 32

char lino_buffer[LINO_CONSOLE_BUFFER_SIZE];
int buffer_size = 0;		/* number of elements in buffer */
int buffer_tail = 0;		/* pointer to next element to return */


/**
 * adds a character to the buffer if the buffer isn't full
 * @param c the character to add to the buffer
 * @return true iff the character is added to the buffer, false otherwise
 */
bool lino_buffer_add(char c)
{
	if (buffer_size >= LINO_CONSOLE_BUFFER_SIZE)
		return false;
	lino_buffer[(buffer_size + buffer_tail) % LINO_CONSOLE_BUFFER_SIZE] = c;
	buffer_size++;
	return true;
}

/**
 * clears the buffer, by setting the buffer size to 0
 */
void lino_buffer_clear(void)
{
	buffer_size = 0;
	buffer_tail = 0;
}

/**
 * gets the next value from the buffer
 * @param res pointer to the location where to store the character
 * @return true iff the character is returned, false otherwise
 */
bool lino_buffer_get(char *res)
{
	if (buffer_size <= 0)
		return false;
	*res = lino_buffer[buffer_tail % LINO_CONSOLE_BUFFER_SIZE];
	buffer_tail++;
	buffer_size--;
	return true;
}

/**
 * hook routine for the isokernel command
 */
bool krnlConsoleCommand(ConsoleCommand command)
{
	char console_input;
	unit unit_input;

	switch (command) {
	case IDLE:
		break;
	case GETCONSOLEINPUT:
		if (!lino_buffer_get(&console_input))
			return false;
		unit_input = (unit) console_input;
		pUIWorkspace[mm_ConsoleInput] = unit_input;
		break;
	case CLEARCONSOLEBUFFER:
		lino_buffer_clear();
		break;
	default:
		return false;
		break;
	}

	return true;
}

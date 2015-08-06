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

#ifndef __LINO_KEYBOARD_H
#define __LINO_KEYBOARD_H

#include "lino_types.h"

bool lino_buffer_add(char);
void lino_buffer_clear(void);
bool lino_buffer_get(char *);

#endif				/* __LINO_KEYBOARD_H */

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

#ifndef __LINO_DISPLAY_H
#define __LINO_DISPLAY_H

#include "lino_types.h"

bool lino_display_init(unit, unit, unit, unit, void *);
bool lino_display_retrace(void);
bool lino_display_retrace_region(unit *);
bool lino_display_move(unit x, unit y);
bool lino_display_resize(unit, unit);
bool lino_display_set_origin(void *data);
bool lino_display_close(void);
void lino_display_check_position(unit *, unit *);

#endif				/* __LINO_DISPLAY_H */

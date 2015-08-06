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

#ifndef __LINO_MOUSE_H
#define __LINO_MOUSE_H

#include "lino_types.h"

#define LEFT_BUTTON_PRESSED 1
#define MIDDLE_BUTTON_PRESSED 2
#define RIGHT_BUTTON_PRESSED 4

#define LEFT_BUTTON_RELEASED 0xFFFFFFFE
#define MIDDLE_BUTTON_RELEASED 0xFFFFFFFD
#define RIGHT_BUTTON_RELEASED 0xFFFFFFFB

typedef struct {
	int x;
	int y;
	unsigned int button;
} lino_mouse;

extern lino_mouse lm;
extern lino_mouse prev;
extern unit current_mode;

void lino_mouse_press(bool, bool, bool);
void lino_mouse_release(bool, bool, bool);
lino_mouse *lino_mouse_update_position(void);

bool initPointerCommand(void);

#endif				/* __LINO_MOUSE_H */

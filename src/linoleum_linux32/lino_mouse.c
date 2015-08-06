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

#include "lino_mouse.h"
#include "lino_types.h"
#include "lino_xdisplay.h"
#include "rtm.h"

lino_mouse lm;
lino_mouse prev;
unit current_mode;

lino_mouse *lino_mouse_update_position(void)
{
	int nothing, result;
	unsigned int unothing;
	Window root, child;
	result = XQueryPointer(xw.display, xw.window,
			       &root, &child, &nothing, &nothing,
			       &lm.x, &lm.y, &unothing);
	if (result) {
		pUIWorkspace[mm_PointerStatus] |= MP_INSIGHT;
	} else {
		pUIWorkspace[mm_PointerStatus] &= !MP_INSIGHT;
	}
	return &lm;
}

bool initPointerCommand(void)
{
	pUIWorkspace[mm_PointerStatus] = MP_PRESENT;

	current_mode = pUIWorkspace[mm_PointerMode];
	switch (current_mode) {
	case MP_BYDELTA:
		/* grab the pointer */
		if (XGrabPointer(xw.display, xw.window, True,	/* Bool owner events */
				 0,	/* event_mask */
				 GrabModeAsync,	/* pointer mode */
				 GrabModeAsync,	/* keyboard mode */
				 xw.window,	/* Window confine_to */
				 None,	/* Cursor cursor */
				 CurrentTime) != GrabSuccess) {
			current_mode = MP_BYCOORDINATE;
			pUIWorkspace[mm_PointerMode] = MP_BYCOORDINATE;
		}
		break;
	case MP_BYCOORDINATE:
		break;
	default:
		return false;
		break;
	}

	lino_mouse_update_position();
	return true;
}

bool krnlPointerCommand(PointerCommand command)
{
	switch (command) {
	case IDLE:
		break;
	case READPOINTER:
		prev = lm;
		pUIWorkspace[mm_PointerStatus] = MP_PRESENT;
		lino_mouse_update_position();
		/* update buttons */
		if (lm.button & LEFT_BUTTON_PRESSED)
			pUIWorkspace[mm_PointerStatus] |= MP_LBUTTONDOWN;
		if (lm.button & RIGHT_BUTTON_PRESSED)
			pUIWorkspace[mm_PointerStatus] |= MP_MBUTTONDOWN;
		if (lm.button & MIDDLE_BUTTON_PRESSED)
			pUIWorkspace[mm_PointerStatus] |= MP_RBUTTONDOWN;
		/* update position information */
		switch (pUIWorkspace[mm_PointerMode]) {
		case MP_BYDELTA:
			if (current_mode != MP_BYDELTA) {
				printf("%s: setting pointer mode BYDELTA.\n",
				       __func__);
				if (XGrabPointer(xw.display, xw.window, True,	/* Bool owner events */
						 0,	/* event_mask */
						 GrabModeAsync,	/* pointer mode */
						 GrabModeAsync,	/* keyboard mode */
						 xw.window,	/* Window confine_to */
						 None,	/* Cursor cursor */
						 CurrentTime) != GrabSuccess) {
					current_mode = MP_BYCOORDINATE;
					pUIWorkspace[mm_PointerMode] =
					    MP_BYCOORDINATE;
					prev = lm;
				} else {
					current_mode = MP_BYDELTA;
				}
			}
			pUIWorkspace[mm_PointerDeltaX] = lm.x - prev.x;
			pUIWorkspace[mm_PointerDeltaY] = lm.y - prev.y;
			XWarpPointer(xw.display,
				     xw.window,
				     xw.window,
				     0, 0, xw.width, xw.height, prev.x, prev.y);
			lm = prev;
			break;
		case MP_BYCOORDINATE:
			if (current_mode != MP_BYCOORDINATE) {
				printf
				    ("%s: setting pointer mode BYCOORDINATE.\n",
				     __func__);
				XUngrabPointer(xw.display, CurrentTime);
				current_mode = MP_BYCOORDINATE;
			}
			pUIWorkspace[mm_PointerXCoordinate] = lm.x;
			pUIWorkspace[mm_PointerYCoordinate] = lm.y;
			break;
		default:
			printf("Invalid Pointer Mode!\n");
			return false;
			break;
		}
		break;
	default:
		return false;
		break;
	}

	return true;
}

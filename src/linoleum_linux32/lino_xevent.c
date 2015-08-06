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

#include <X11/Xlib.h>		/* fundamental X data structures */
#include <X11/Xutil.h>		/* data definitions for various functions */
#include <X11/keysym.h>		/* for a perfect use of keyboard events */

#include <stdio.h>

#include "rtm.h"
#include "lino_types.h"
#include "lino_xevent.h"
#include "lino_keyboard.h"
#include "lino_luck.h"
#include "lino_mouse.h"

#define K_BUFFER_SIZE 10

lino_event ev;
KeySym key;

bool is_dirty = false;
unit dirty_region[4];

static unit min_unit(unit a, unit b)
{
	return a < b ? a : b;
}
static unit max_unit(unit a, unit b)
{
	return a > b ? a : b;
}

static void set_dirty_region(unit left, unit top, unit right, unit bottom)
{
	if (!is_dirty) {
		is_dirty = true;
		dirty_region[0] = left;
		dirty_region[1] = top;
		dirty_region[2] = right;
		dirty_region[3] = bottom;
	} else {
		dirty_region[0] = min_unit(dirty_region[0], left);
		dirty_region[1] = min_unit(dirty_region[1], top);
		dirty_region[2] = max_unit(dirty_region[2], right);
		dirty_region[3] = max_unit(dirty_region[3], bottom);
	}
}

static void retrace_dirty_region()
{
	lino_display_retrace_region(&dirty_region[0]);
	is_dirty = false;
}

/** 
 * handles all pending events
 */
void handle_pending_events(void)
{
	XEvent ev;
	KeySym key;
	XComposeStatus compose;

	int charcount, i;
	char buffer[K_BUFFER_SIZE];

	while (XPending(xw.display)) {
		XNextEvent(xw.display, &ev);
		switch (ev.type) {
		case Expose:
			set_dirty_region(ev.xexpose.x, ev.xexpose.y,
					 ev.xexpose.x + ev.xexpose.width,
					 ev.xexpose.y + ev.xexpose.height);
			if (ev.xexpose.count == 0) {
				retrace_dirty_region();
			}
			break;
/* 		case MotionNotify: */
/* 			lino_mouse_update_position(); */
/* 			break; */
		case ButtonPress:
			switch (ev.xbutton.button) {
			case Button1:
				lm.button |= LEFT_BUTTON_PRESSED;
				break;
			case Button2:
				lm.button |= RIGHT_BUTTON_PRESSED;
				break;
			case Button3:
				lm.button |= MIDDLE_BUTTON_PRESSED;
				break;
			}
			break;
		case ButtonRelease:
			switch (ev.xbutton.button) {
			case Button1:
				lm.button &= LEFT_BUTTON_RELEASED;
				break;
			case Button2:
				lm.button &= RIGHT_BUTTON_RELEASED;
				break;
			case Button3:
				lm.button &= MIDDLE_BUTTON_RELEASED;
				break;
			}
			break;
		case KeyPress:
			charcount =
			    XLookupString(&ev.xkey, buffer, K_BUFFER_SIZE, &key,
					  &compose);
			if (charcount > 0) {
				for (i = 0; i < charcount; i++)
					if (!lino_buffer_add(buffer[i]))
						fprintf(stdout,
							"Buffer Full!!??\n");
				fflush(stdout);
			}

			if (key >= XK_A && key <= XK_Z)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_A +
					     KEY_A] = 1;
			else if (key >= XK_a && key <= XK_z)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_a +
					     KEY_A] = 1;
			else if (key >= XK_0 && key <= XK_9)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_0 +
					     KEY_0] = 1;
			else if (key >= XK_F1 && key <= XK_F24)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_F1 +
					     KEY_F1] = 1;
			else if (key >= XK_KP_0 && key <= XK_KP_9)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_KP_9 +
					     KEY_0N] = 1;
			else {
				switch (key) {
				case XK_BackSpace:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_BACKSPACE] = 1;
					break;
				case XK_Tab:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_TAB] = 1;
					break;
				case XK_Return:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_RETURN] = 1;
					break;
				case XK_Escape:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_ESCAPE] = 1;
					break;
				case XK_space:
				case XK_KP_Space:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SPACE_BAR] = 1;
					break;
				case XK_Insert:
				case XK_KP_Insert:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_INSERT] = 1;
					break;
				case XK_Delete:
				case XK_KP_Delete:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_DELETE] = 1;
					break;
				case XK_Home:
				case XK_KP_Home:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_HOME] = 1;
					break;
				case XK_End:
				case XK_KP_End:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_END] = 1;
					break;
				case XK_Page_Up:
				case XK_KP_Page_Up:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PGUP] = 1;
					break;
				case XK_Page_Down:
				case XK_KP_Page_Down:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PGDN] = 1;
					break;
				case XK_Up:
				case XK_KP_Up:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_UP] = 1;
					break;
				case XK_Down:
				case XK_KP_Down:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_DOWN] = 1;
					break;
				case XK_Left:
				case XK_KP_Left:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_LEFT] = 1;
					break;
				case XK_Right:
				case XK_KP_Right:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_RIGHT] = 1;
					break;
				case XK_slash:
				case XK_KP_Divide:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SLASH] = 1;
					break;
				case XK_asterisk:
				case XK_KP_Multiply:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_ASTERISK] = 1;
					break;
				case XK_minus:
				case XK_KP_Subtract:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_MINUS] = 1;
					break;
				case XK_plus:
				case XK_KP_Add:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PLUS] = 1;
					break;
				case XK_period:
				case XK_KP_Separator:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_DOT] = 1;
					break;
				case XK_Shift_L:
				case XK_Shift_R:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SHIFT] = 1;
					break;
				case XK_Control_L:
				case XK_Control_R:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_CONTROL] = 1;
					break;
				case XK_Alt_L:
				case XK_Alt_R:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_ALTERNATE] = 1;
					break;
				case XK_Pause:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PAUSE] = 1;
					break;
				case XK_Num_Lock:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_NUM_LOCK] = 1;
					break;
				case XK_Caps_Lock:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_CAPS_LOCK] = 1;
					break;
				case XK_Scroll_Lock:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SCROLL_LOCK] = 1;
					break;
				default:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_UNCLASSIFIED] = 1;
					break;
				}
			}
			break;
		case KeyRelease:
			XLookupString(&ev.xkey, buffer, 1, &key, &compose);
			if (key >= XK_A && key <= XK_Z)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_A +
					     KEY_A] = 0;
			else if (key >= XK_a && key <= XK_z)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_a +
					     KEY_A] = 0;
			else if (key >= XK_0 && key <= XK_9)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_0 +
					     KEY_0] = 0;
			else if (key >= XK_F1 && key <= XK_F24)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_F1 +
					     KEY_F1] = 0;
			else if (key >= XK_KP_0 && key <= XK_KP_9)
				pUIWorkspace[mm_ConsoleOrigin + key - XK_KP_9 +
					     KEY_0N] = 0;
			else {
				switch (key) {
				case XK_BackSpace:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_BACKSPACE] = 0;
					break;
				case XK_Tab:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_TAB] = 0;
					break;
				case XK_Return:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_RETURN] = 0;
					break;
				case XK_Escape:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_ESCAPE] = 0;
					break;
				case XK_space:
				case XK_KP_Space:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SPACE_BAR] = 0;
					break;
				case XK_Insert:
				case XK_KP_Insert:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_INSERT] = 0;
					break;
				case XK_Delete:
				case XK_KP_Delete:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_DELETE] = 0;
					break;
				case XK_Home:
				case XK_KP_Home:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_HOME] = 0;
					break;
				case XK_End:
				case XK_KP_End:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_END] = 0;
					break;
				case XK_Page_Up:
				case XK_KP_Page_Up:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PGUP] = 0;
					break;
				case XK_Page_Down:
				case XK_KP_Page_Down:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PGDN] = 0;
					break;
				case XK_Up:
				case XK_KP_Up:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_UP] = 0;
					break;
				case XK_Down:
				case XK_KP_Down:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_DOWN] = 0;
					break;
				case XK_Left:
				case XK_KP_Left:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_LEFT] = 0;
					break;
				case XK_Right:
				case XK_KP_Right:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_RIGHT] = 0;
					break;
				case XK_slash:
				case XK_KP_Divide:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SLASH] = 0;
					break;
				case XK_asterisk:
				case XK_KP_Multiply:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_ASTERISK] = 0;
					break;
				case XK_minus:
				case XK_KP_Subtract:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_MINUS] = 0;
					break;
				case XK_plus:
				case XK_KP_Add:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PLUS] = 0;
					break;
				case XK_period:
				case XK_KP_Separator:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_DOT] = 0;
					break;
				case XK_Shift_L:
				case XK_Shift_R:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SHIFT] = 0;
					break;
				case XK_Control_L:
				case XK_Control_R:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_CONTROL] = 0;
					break;
				case XK_Alt_L:
				case XK_Alt_R:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_ALTERNATE] = 0;
					break;
				case XK_Pause:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_PAUSE] = 0;
					break;
				case XK_Num_Lock:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_NUM_LOCK] = 0;
					break;
				case XK_Caps_Lock:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_CAPS_LOCK] = 0;
					break;
				case XK_Scroll_Lock:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_SCROLL_LOCK] = 0;
					break;
				default:
					pUIWorkspace[mm_ConsoleOrigin +
						     KEY_UNCLASSIFIED] = 0;
					break;
				}
			}
			break;
		default:
			PRINT("Unhandled event\n");
			break;
		}
	}
}

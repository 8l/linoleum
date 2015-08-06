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

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "rtm.h"
#include "lino_xdisplay.h"
#include "lino_file.h"

/**
 * handles all Clipboard commands
 * @return false when errors, true otherwise
 */
bool krnlClipCommand(ClipCommand command)
{
	bool result = true;

	Atom type;
	int format, res;
	unsigned long bytes_left, len, dummy;
	unsigned char *data;
	Window selectionOwner;

	if (command != IDLE) {
		PRINT1("Clip Command: %u\n", command);
	}

	switch (command) {
	case IDLE:
		break;
	case GETCLIPSIZE:
		/* check if some window has a selection */
		selectionOwner = XGetSelectionOwner(xw.display, XA_PRIMARY);
		if (selectionOwner == None) {
			result = false;
			break;
		}
		/* convert the selection to a string  */
		XConvertSelection(xw.display, XA_PRIMARY, XA_STRING, None,
				  selectionOwner, CurrentTime);
		XFlush(xw.display);
		/* get the properties of the selection */
		XGetWindowProperty(xw.display, selectionOwner, XA_STRING, 0, 0,
				   0, AnyPropertyType, &type, &format, &len,
				   &bytes_left, &data);
		pUIWorkspace[mm_ClipSize] = bytes_left;

		break;
	case READCLIP:
		/* check if some window has a selection */
		selectionOwner = XGetSelectionOwner(xw.display, XA_PRIMARY);
		if (selectionOwner == None) {
			result = false;
			break;
		}
		/* convert the selection to a string */
		XConvertSelection(xw.display, XA_PRIMARY, XA_STRING, None,
				  selectionOwner, CurrentTime);
		XFlush(xw.display);
		/* actually read the selection */
		res =
		    XGetWindowProperty(xw.display, selectionOwner, XA_STRING, 0,
				       pUIWorkspace[mm_ClipSize] - 1, 0,
				       AnyPropertyType, &type, &format, &len,
				       &dummy, &data);
		if (res != Success) {
			result = false;
		} else {
			/* write the data to the application workspace */
			btrsstring(&pWorkspace[pUIWorkspace[mm_ClipString]],
				   (const char *) data);
		}
		/* clear allocated memory */
		XFree(data);
		break;
	case WRITECLIP:
		result = false;
		break;
	default:
		result = false;
		break;
	}

	if (!result) {
		pUIWorkspace[mm_ClipSize] = 0;
	}

	return result;
}

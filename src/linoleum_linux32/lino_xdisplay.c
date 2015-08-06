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

/* includes */
#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>		/* fundamental X data structures */
#include <X11/Xutil.h>		/* data definitions for various functions */
#include <X11/keysym.h>		/* for a perfect use of keyboard events */

#include "rtm.h"
#include "lino_xdisplay.h"
#include "lino_mouse.h"

/* define flags */

/* local declarations */
XWindow xw;
unit origin;
/* bool lino_display_active = false; */

bool krnlDisplayCommand(DisplayCommand command)
{
	bool result = true;
	/* check if the display is initialized, no display call when not initialized */
	if (!xw.display) {
		PRINT1("%s: Display isn't initialized.", __func__);
		return result;
	}

	/* check if other display origin is set */
	if (origin != pUIWorkspace[mm_DisplayOrigin]) {
		origin = pUIWorkspace[mm_DisplayOrigin];
		lino_display_set_origin(&pWorkspace[origin]);
	}

	/* check if window must be resized */
	if ((xw.width != pUIWorkspace[mm_DisplayWidth])
	    || (xw.height != pUIWorkspace[mm_DisplayHeight])) {
		lino_display_resize(pUIWorkspace[mm_DisplayWidth],
				    pUIWorkspace[mm_DisplayHeight]);
	}

	/* check if window must be moved */
	if ((xw.x != pUIWorkspace[mm_DisplayXPosition])
	    || (xw.y != pUIWorkspace[mm_DisplayYPosition])) {
		lino_display_move(pUIWorkspace[mm_DisplayXPosition],
				  pUIWorkspace[mm_DisplayYPosition]);
	}

	switch (command) {
	case IDLE:
		break;
	case RETRACE:
		PRINT1("%s: command = RETRACE\n", __func__);
		switch (pUIWorkspace[mm_DisplayLiveRegion]) {
		case WHOLEDISPLAY:
			if (lino_display_retrace() == false)
				result = false;
			break;
		case VOIDREGION:
			pUIWorkspace[mm_DisplayLiveRegion] = WHOLEDISPLAY;
			break;
		default:
			if (!lino_display_retrace_region
			    (&pWorkspace[pUIWorkspace[mm_DisplayLiveRegion]]))
				result = false;
			pUIWorkspace[mm_DisplayLiveRegion] = WHOLEDISPLAY;
			break;
		}
		break;
	case SETCOOPERATIVEMODE:{
#ifdef ENABLE_EXCLUSIVE_MODE
		Atom wmState =
			XInternAtom(xw.display, "_NET_WM_STATE", False);
		Atom atom_fullScreen = XInternAtom(xw.display,
						   "_NET_WM_STATE_FULLSCREEN",
						   False);

		XEvent xev;
		xev.xclient.type = ClientMessage;
		xev.xclient.serial = 0;
		xev.xclient.send_event = True;
		xev.xclient.window = xw.window;
		xev.xclient.message_type = wmState;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
		xev.xclient.data.l[1] = atom_fullScreen;
		xev.xclient.data.l[2] = 0;

		XSendEvent(xw.display, DefaultRootWindow(xw.display),
			   False,
			   SubstructureRedirectMask |
			   SubstructureNotifyMask, &xev);
#endif
	}
		break;
	case SETEXCLUSIVEMODE:{
#ifdef ENABLE_EXCLUSIVE_MODE
		Atom wmState =
			XInternAtom(xw.display, "_NET_WM_STATE", False);
		Atom atom_fullScreen = XInternAtom(xw.display,
						   "_NET_WM_STATE_FULLSCREEN",
						   False);

		XEvent xev;
		xev.xclient.type = ClientMessage;
		xev.xclient.serial = 0;
		xev.xclient.send_event = True;
		xev.xclient.window = xw.window;
		xev.xclient.message_type = wmState;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
		xev.xclient.data.l[1] = atom_fullScreen;
		xev.xclient.data.l[2] = 0;

		XSendEvent(xw.display, DefaultRootWindow(xw.display),
			   False,
			   SubstructureRedirectMask |
			   SubstructureNotifyMask, &xev);
#endif
		/* exclusive mode is currently not supported */
		printf
			("%s: EXCLUSIVE display mode not (yet) supported.\n",
			 __func__);
	}
		break;
	default:
		result = false;
		break;
	}
	return result;
}

/** 
 * linoleum display help routines
 */

/**
 * initialises the display
 * @param x the x coordinate of the display
 * @param y the y coordinate of the display
 * @param width the width of the display
 * @param height the height of the display
 * @return false if an error occurred while setting up the display section
 */
bool lino_display_init(unit x, unit y, unit w, unit h, void *data)
{
	PRINT1("%s: Initializing display...\n", __func__);

	xw.visible = false;

	/* open display */
	if (!(xw.display = XOpenDisplay(NULL)))
		return false;

	/* initialize xw structure with default values */
	xw.pixelsize = 4;
	xw.screensize = w * h * xw.pixelsize;
	xw.width = w;
	xw.height = h;
	xw.screennum = DefaultScreen(xw.display);
	xw.depth = DefaultDepth(xw.display, xw.screennum);
	xw.screenptr = DefaultScreenOfDisplay(xw.display);
	xw.visual = DefaultVisualOfScreen(xw.screenptr);
	xw.phys_width = DisplayWidth(xw.display, xw.screennum);
	xw.phys_height = DisplayHeight(xw.display, xw.screennum);

	/* initialize workspace parameters */
	pUIWorkspace[mm_DisplayWidth] = w;
	pUIWorkspace[mm_DisplayHeight] = h;
	pUIWorkspace[mm_DisplayPhysicalWidth] = xw.phys_width;
	pUIWorkspace[mm_DisplayPhysicalHeight] = xw.phys_height;
	pUIWorkspace[mm_DisplayStatus] = ACTIVE;

	/* only 24 bits screens are supported */
	if (xw.depth != 24)
		return false;

	/* create the window */
	xw.window = XCreateWindow(xw.display,
				  RootWindowOfScreen(xw.screenptr),
				  0 /* x */ , 0 /* y */ ,
				  256 /* w */ , 192 /* h */ ,
				  0, xw.depth, InputOutput, xw.visual, 0, NULL);
	if (!xw.window)
		return false;

	/* initialize graphic context */
	xw.gc = XCreateGC(xw.display, xw.window, 0, NULL);
	if (!xw.gc)
		return false;

	/* create image to hold background */
	xw.ximage = XCreateImage(xw.display, xw.visual, xw.depth, ZPixmap,
				 0, data, xw.width, xw.height,
				 32, xw.width * xw.pixelsize);
	if (!xw.ximage)
		return false;

	/* We change the title of the window (default:Untitled) */
	XStoreName(xw.display, xw.window, (char *) &IParagraph->appname);

	/* set autorepeat for keyboard input */
	XAutoRepeatOn(xw.display);

	/* initialize events */
	XSelectInput(xw.display, xw.window,
		     ExposureMask | KeyPressMask | KeyReleaseMask
		     | ButtonPressMask | ButtonReleaseMask);

	/* Set the size of the window */
	lino_display_resize(w, h);

	/* set the position of the window */
	lino_display_move(x, y);

	PRINT1("%s: Initializing display done.\n", __func__);
	return true;
}

/** 
 * retraces the display
 * @param data pointer to the actual video buffer
 * @return false if an error occurred, otherwise true
 */
bool lino_display_retrace(void)
{
	if (xw.visible && xw.ximage->data != NULL) {
		XPutImage(xw.display, xw.window, xw.gc, xw.ximage,
			  0, 0, 0, 0, xw.width, xw.height);
	}
	return true;
}

/**
 * retraces only part of the display
 * @param data pointer to the actual video buffer
 * @param region pointer to the LEFT, TOP, RIGHT and BOTTOM bounds of the region to retrace
 * @return false if an error occurred, otherwise true
 */
bool lino_display_retrace_region(unit * region)
{
	if (xw.visible && xw.ximage->data != NULL) {
		XPutImage(xw.display, xw.window, xw.gc, xw.ximage,
			  region[0], region[1], region[0], region[1],
			  region[2] - region[0] + 1, region[3] - region[1] + 1);
	}
	return true;
}

/**
 * Checks if the special value MIDDLE is used for x or y 
 * position.
 * @param x pointer to unit holding left position of window
 * @param y pointer to unit holding top position of window
 */
void lino_display_check_position(unit * x, unit * y)
{
	if (*x == MIDDLE)
		*x = (xw.phys_width - xw.width) / 2;

	if (*y == MIDDLE)
		*y = (xw.phys_height - xw.height) / 2;
}

/**
 * moves the display to the specified coordinates
 * (this command is obsolete in Xlib)
 * @param x the x coordinate
 * @param y the y coordinate
 */
bool lino_display_move(unit x, unit y)
{
	lino_display_check_position(&x, &y);
	if (xw.x != x || xw.y != y) {
		XMoveWindow(xw.display, xw.window, (int) x, (int) y);
		xw.x = x;
		xw.y = y;
	}

	return true;
}

/**
 * sets the origin of the display
 * @param data pointer to the display data
 */
bool lino_display_set_origin(void *data)
{
	xw.ximage->data = (char *) data;
	return true;
}

/**
 * updates the size of the display
 * @param width the new pixel width of the display
 * @param height then new pixel height of the display
 * @return false if an error occurred, otherwise true
 */
bool lino_display_resize(unit w, unit h)
{
	/* Set the visibility mode of the display */
	if (w <= 0 || h <= 0) {
		if (xw.visible) {
			/* set the visibility to false.
			 * TODO: display default screen 
			 *   OR: minimize window
			 */
			xw.visible = false;
		}
	} else {
		XResizeWindow(xw.display, xw.window, (int) w, (int) h);
		xw.width = w;
		xw.height = h;
		xw.ximage->width = w;
		xw.ximage->height = h;
		xw.ximage->bytes_per_line = w * xw.pixelsize;
		if (!xw.visible) {
			/* set the visibility to true. */
			xw.visible = true;
			/* map the window and raise it to the top of the stack */
			XMapRaised(xw.display, xw.window);
		}
	}
	return true;
}

/**
 * clean up all the held resources
 */
bool lino_display_close(void)
{
	/* clean the garbage left */
	PRINT1("%s: clearing image data.\n", __func__);
	xw.ximage->data = NULL;
	PRINT1("%s: destroying image.\n", __func__);
	XDestroyImage(xw.ximage);
	PRINT1("%s: freeing graphics context.\n", __func__);
	XFreeGC(xw.display, xw.gc);
	PRINT1("%s: destroying window.\n", __func__);
	XDestroyWindow(xw.display, xw.window);
	PRINT1("%s: closing display.\n", __func__);
	XCloseDisplay(xw.display);
	PRINT1("%s finished\n", __func__);
	return true;
}

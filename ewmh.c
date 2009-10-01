/* evilwm - Minimalist Window Manager for X
 * Copyright (C) 1999-2009 Ciaran Anscomb <evilwm@6809.org.uk>
 * see README for license and other details. */

#include <stdlib.h>
#include <unistd.h>
#include "evilwm.h"
#include "log.h"

/* Root Window Properties (and Related Messages) */
static Atom xa_net_supported;
static Atom xa_net_client_list;
static Atom xa_net_client_list_stacking;
#ifdef VWM
static Atom xa_net_number_of_desktops;
Atom xa_net_current_desktop;
#endif
Atom xa_net_active_window;
static Atom xa_net_supporting_wm_check;

/* Other Root Window Messages */
Atom xa_net_close_window;
Atom xa_net_moveresize_window;
Atom xa_net_request_frame_extents;

/* Application Window Properties */
static Atom xa_net_wm_name;
#ifdef VWM
Atom xa_net_wm_desktop;
#endif
Atom xa_net_wm_state;
#ifdef VWM
Atom xa_net_wm_state_sticky;
#endif
Atom xa_net_wm_state_maximized_vert;
Atom xa_net_wm_state_maximized_horz;
Atom xa_net_wm_state_fullscreen;
static Atom xa_net_wm_allowed_actions;
static Atom xa_net_wm_action_move;
static Atom xa_net_wm_action_resize;
static Atom xa_net_wm_action_stick;
static Atom xa_net_wm_action_maximize_horz;
static Atom xa_net_wm_action_maximize_vert;
static Atom xa_net_wm_action_fullscreen;
static Atom xa_net_wm_action_change_desktop;
static Atom xa_net_wm_action_close;
static Atom xa_net_wm_pid;
Atom xa_net_frame_extents;

void ewmh_init(void) {
	/* Root Window Properties (and Related Messages) */
	xa_net_supported = XInternAtom(dpy, "_NET_SUPPORTED", False);
	xa_net_client_list = XInternAtom(dpy, "_NET_CLIENT_LIST", False);
	xa_net_client_list_stacking = XInternAtom(dpy, "_NET_CLIENT_LIST_STACKING", False);
#ifdef VWM
	xa_net_number_of_desktops = XInternAtom(dpy, "_NET_NUMBER_OF_DESKTOPS", False);
	xa_net_current_desktop = XInternAtom(dpy, "_NET_CURRENT_DESKTOP", False);
#endif
	xa_net_active_window = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
	xa_net_supporting_wm_check = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False);

	/* Other Root Window Messages */
	xa_net_close_window = XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
	xa_net_moveresize_window = XInternAtom(dpy, "_NET_MOVERESIZE_WINDOW", False);
	xa_net_request_frame_extents = XInternAtom(dpy, "_NET_REQUEST_FRAME_EXTENTS", False);

	/* Application Window Properties */
	xa_net_wm_name = XInternAtom(dpy, "_NET_WM_NAME", False);
#ifdef VWM
	xa_net_wm_desktop = XInternAtom(dpy, "_NET_WM_DESKTOP", False);
#endif
	xa_net_wm_state = XInternAtom(dpy, "_NET_WM_STATE", False);
#ifdef VWM
	xa_net_wm_state_sticky = XInternAtom(dpy, "_NET_WM_STATE_STICKY", False);
#endif
	xa_net_wm_state_maximized_vert = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_VERT", False);
	xa_net_wm_state_maximized_horz = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	xa_net_wm_state_fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
	xa_net_wm_allowed_actions = XInternAtom(dpy, "_NET_WM_ALLOWED_ACTIONS", False);
	xa_net_wm_action_move = XInternAtom(dpy, "_NET_WM_ACTION_MOVE", False);
	xa_net_wm_action_resize = XInternAtom(dpy, "_NET_WM_ACTION_RESIZE", False);
	xa_net_wm_action_stick = XInternAtom(dpy, "_NET_WM_ACTION_STICK", False);
	xa_net_wm_action_maximize_horz = XInternAtom(dpy, "_NET_WM_ACTION_MAXIMIZE_HORZ", False);
	xa_net_wm_action_maximize_vert = XInternAtom(dpy, "_NET_WM_ACTION_MAXIMIZE_VERT", False);
	xa_net_wm_action_fullscreen = XInternAtom(dpy, "_NET_WM_ACTION_FULLSCREEN", False);
	xa_net_wm_action_change_desktop = XInternAtom(dpy, "_NET_WM_ACTION_CHANGE_DESKTOP", False);
	xa_net_wm_action_close = XInternAtom(dpy, "_NET_WM_ACTION_CLOSE", False);
	xa_net_wm_pid = XInternAtom(dpy, "_NET_WM_PID", False);
	xa_net_frame_extents = XInternAtom(dpy, "_NET_FRAME_EXTENTS", False);
}

void ewmh_init_screen(ScreenInfo *s) {
	unsigned long pid = getpid();
	Atom supported[] = {
		xa_net_client_list,
		xa_net_client_list_stacking,
#ifdef VWM
		xa_net_number_of_desktops,
		xa_net_current_desktop,
#endif
		xa_net_active_window,
		xa_net_supporting_wm_check,

		xa_net_close_window,
		xa_net_moveresize_window,
		xa_net_request_frame_extents,

#ifdef VWM
		xa_net_wm_desktop,
#endif
		xa_net_wm_state,
#ifdef VWM
		xa_net_wm_state_sticky,
#endif
		xa_net_wm_state_maximized_vert,
		xa_net_wm_state_maximized_horz,
		xa_net_wm_state_fullscreen,
		xa_net_wm_allowed_actions,
		/* Not sure if it makes any sense including every action here
		 * as they'll already be listed per-client in the
		 * _NET_WM_ALOWED_ACTIONS property, but EWMH spec is unclear.
		 * */
		xa_net_wm_action_move,
		xa_net_wm_action_resize,
#ifdef VWM
		xa_net_wm_action_stick,
#endif
		xa_net_wm_action_maximize_horz,
		xa_net_wm_action_maximize_vert,
		xa_net_wm_action_fullscreen,
		xa_net_wm_action_change_desktop,
		xa_net_wm_action_close,
		xa_net_frame_extents,
	};
#ifdef VWM
	unsigned long num_desktops = 8;
	unsigned long vdesk = s->vdesk;
#endif
	s->supporting = XCreateSimpleWindow(dpy, s->root, 0, 0, 1, 1, 0, 0, 0);
	XChangeProperty(dpy, s->root, xa_net_supported,
			XA_ATOM, 32, PropModeReplace,
			(unsigned char *)&supported,
			sizeof(supported) / sizeof(Atom));
#ifdef VWM
	XChangeProperty(dpy, s->root, xa_net_number_of_desktops,
			XA_CARDINAL, 32, PropModeReplace,
			(unsigned char *)&num_desktops, 1);
	XChangeProperty(dpy, s->root, xa_net_current_desktop,
			XA_CARDINAL, 32, PropModeReplace,
			(unsigned char *)&vdesk, 1);
#endif
	XChangeProperty(dpy, s->root, xa_net_supporting_wm_check,
			XA_WINDOW, 32, PropModeReplace,
			(unsigned char *)&s->supporting, 1);
	XChangeProperty(dpy, s->supporting, xa_net_supporting_wm_check,
			XA_WINDOW, 32, PropModeReplace,
			(unsigned char *)&s->supporting, 1);
	XChangeProperty(dpy, s->supporting, xa_net_wm_name,
			XA_STRING, 8, PropModeReplace,
			(const unsigned char *)"evilwm", 6);
	XChangeProperty(dpy, s->supporting, xa_net_wm_pid,
			XA_CARDINAL, 32, PropModeReplace,
			(unsigned char *)&pid, 1);
}

void ewmh_deinit_screen(ScreenInfo *s) {
	XDeleteProperty(dpy, s->root, xa_net_supported);
	XDeleteProperty(dpy, s->root, xa_net_client_list);
	XDeleteProperty(dpy, s->root, xa_net_client_list_stacking);
#ifdef VWM
	XDeleteProperty(dpy, s->root, xa_net_number_of_desktops);
	XDeleteProperty(dpy, s->root, xa_net_current_desktop);
#endif
	XDeleteProperty(dpy, s->root, xa_net_active_window);
	XDeleteProperty(dpy, s->root, xa_net_supporting_wm_check);
	XDestroyWindow(dpy, s->supporting);
}

void ewmh_init_client(Client *c) {
	Atom allowed_actions[] = {
		xa_net_wm_action_move,
#ifdef VWM
		xa_net_wm_action_stick,
#endif
		xa_net_wm_action_maximize_horz,
		xa_net_wm_action_maximize_vert,
		xa_net_wm_action_fullscreen,
		xa_net_wm_action_change_desktop,
		xa_net_wm_action_close,
		/* nelements reduced to omit this if not possible: */
		xa_net_wm_action_resize,
	};
	int nelements = sizeof(allowed_actions) / sizeof(Atom);
	/* Omit resize element if resizing not possible: */
	if (c->max_width && c->max_width == c->min_width
			&& c->max_height && c->max_height == c->min_height)
		nelements--;
	XChangeProperty(dpy, c->window, xa_net_wm_allowed_actions,
			XA_ATOM, 32, PropModeReplace,
			(unsigned char *)&allowed_actions,
			nelements);
}

void ewmh_deinit_client(Client *c) {
	XDeleteProperty(dpy, c->window, xa_net_wm_allowed_actions);
}

void ewmh_withdraw_client(Client *c) {
#ifdef VWM
	XDeleteProperty(dpy, c->window, xa_net_wm_desktop);
#endif
	XDeleteProperty(dpy, c->window, xa_net_wm_state);
}

void ewmh_select_client(Client *c) {
	clients_tab_order = list_to_head(clients_tab_order, c);
}

void ewmh_raise_client(Client *c) {
	clients_stacking_order = list_to_tail(clients_stacking_order, c);
	ewmh_set_net_client_list_stacking(c->screen);
}

void ewmh_lower_client(Client *c) {
	clients_stacking_order = list_to_head(clients_stacking_order, c);
	ewmh_set_net_client_list_stacking(c->screen);
}

void ewmh_set_net_client_list(ScreenInfo *s) {
	struct list *iter;
	XDeleteProperty(dpy, s->root, xa_net_client_list);
	for (iter = clients_mapping_order; iter; iter = iter->next) {
		Client *c = iter->data;
		if (c->screen == s) {
			XChangeProperty(dpy, s->root, xa_net_client_list,
					XA_WINDOW, 32, PropModeAppend,
					(unsigned char *)&c->window, 1);
		}
	}
}

void ewmh_set_net_client_list_stacking(ScreenInfo *s) {
	struct list *iter;
	XDeleteProperty(dpy, s->root, xa_net_client_list_stacking);
	for (iter = clients_stacking_order; iter; iter = iter->next) {
		Client *c = iter->data;
		if (c->screen == s) {
			XChangeProperty(dpy, s->root, xa_net_client_list_stacking,
					XA_WINDOW, 32, PropModeAppend,
					(unsigned char *)&c->window, 1);
		}
	}
}

#ifdef VWM
void ewmh_set_net_current_desktop(ScreenInfo *s) {
	unsigned long vdesk = s->vdesk;
	XChangeProperty(dpy, s->root, xa_net_current_desktop,
			XA_CARDINAL, 32, PropModeReplace,
			(unsigned char *)&vdesk, 1);
}
#endif

void ewmh_set_net_active_window(Client *c) {
	int i;
	for (i = 0; i < num_screens; i++) {
		Window w;
		if (c && i == c->screen->screen) {
			w = c->window;
		} else {
			w = None;
		}
		XChangeProperty(dpy, screens[i].root, xa_net_active_window,
				XA_WINDOW, 32, PropModeReplace,
				(unsigned char *)&w, 1);
	}
}

#ifdef VWM
void ewmh_set_net_wm_desktop(Client *c) {
	unsigned long vdesk = c->vdesk;
	XChangeProperty(dpy, c->window, xa_net_wm_desktop,
			XA_CARDINAL, 32, PropModeReplace,
			(unsigned char *)&vdesk, 1);
}
#endif

void ewmh_set_net_wm_state(Client *c) {
	Atom state[4];
	int i = 0;
#ifdef VWM
	if (is_sticky(c))
		state[i++] = xa_net_wm_state_sticky;
#endif
	if (c->oldh)
		state[i++] = xa_net_wm_state_maximized_vert;
	if (c->oldw)
		state[i++] = xa_net_wm_state_maximized_horz;
	if (c->oldh && c->oldw)
		state[i++] = xa_net_wm_state_fullscreen;
	XChangeProperty(dpy, c->window, xa_net_wm_state,
			XA_ATOM, 32, PropModeReplace,
			(unsigned char *)&state, i);
}

void ewmh_set_net_frame_extents(Window w) {
	unsigned long extents[4];
	extents[0] = extents[1] = extents[2] = extents[3] = opt_bw;
	XChangeProperty(dpy, w, xa_net_frame_extents,
			XA_CARDINAL, 32, PropModeReplace,
			(unsigned char *)&extents, 4);
}

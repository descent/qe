/*
 *  menu.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <string.h>
#include "config.h"
#include "menu.h"
#include "filebuffer.h"
#include "colors.h"
#include "keymap.h"


menu_list::menu_list(void) {
    init_menu_list();
}

void menu_list::init_menu_list(const char *name, int (filebuffer::*function)(void)){
    init_menu_list();

    if (name != NULL) {
	int len = strlen(name);
	keyidx = -1;

	for (int i = 0; i < len; i++) {
	    if (name[i] == '&') {
		keyidx = i;
		len--;
		break;
	    }
	}
	title = new char[len + 1];
	switch (keyidx) {
	case -1:
	    strcpy(title, name);
	    break;
	case  0:
	    strcpy(title, &name[1]);
	    break;
	default:
	    strncpy(title, name, keyidx);
	    strcpy(&title[keyidx], &name[keyidx+1]);
	    break;
	}
    }

    action = function;
}

menu_list::menu_list(const char *name, int (filebuffer::*function)(void)) {
    init_menu_list(name, function);
}

menu_list::menu_list(const char *name, int (filebuffer::*function)(void),
				char *(filebuffer::*tfcn)(const char *)) {
    init_menu_list(name, function);
    getTitle = tfcn;
}

void menu_list::init_menu_list(void) {
    title  = NULL;
    action = (int (filebuffer::*)(void))  NULL;
    getTitle = (char *(filebuffer::*)(const char *)) NULL;
    next   = NULL;
    keyidx = -1;
}

menu_list::~menu_list(void) {
    delete title;
}


/////////////////////////////////////////////

menu_bar::menu_bar(void) {
    init_menu_bar();
}

menu_bar::menu_bar(const int  num, const char *name) {
    init_menu_bar();

    id = num;
    title = new char[strlen(name) + 1];
    strcpy(title, name);
}

void menu_bar::init_menu_bar(void) {
    id	     = -1;
    width    = 0;
    height   = 0;
    sel      = 0;
    title    = NULL;
    ml	     = NULL;
    next     = NULL;
    previous = NULL;
}

menu_bar::~menu_bar(void) {
    for (menu_list *ptr = ml; ptr != NULL; ptr = ml) {
	ml = ml->next;
	ptr->~menu_list();
	delete ptr;
    }
    delete title;
}


int menu_bar::add_item(const char *name, int (filebuffer::*function)(void),
					   char *(filebuffer::*tfcn)(const char *)) {

    menu_list  *ptr = new menu_list(name, function, tfcn);

    int  len = strlen(name);
    if	(tfcn != NULL) len += 10;

    width = maximum(width, len);
    height++;

    if (ml == NULL) {
	ml = ptr;
    } else {
	menu_list *q;
	for (q = ml; q->next != NULL; q = q->next)
	    ;

	q->next = ptr;
    }
    return 1;
}

////////////////////////////////////////


menu::menu(void) {
    list = NULL;
    mw	 = NULL;
    assigned = 0;
    bar[0] = '\0';
}

menu::~menu(void) {
}

int menu::add_menu(const char *name) {
    int  val = assigned;

    sprintf(&bar[val], "%s  ", name);
    assigned = strlen(bar);

    menu_bar  *ptr = new menu_bar(val, name);

    ptr->next	  = NULL;
    ptr->previous = NULL;

    if (list == NULL) {
	list = ptr;
    } else {
	menu_bar *q;
	for (q = list; q->next != NULL; q = q->next)
	    ;

	q->next = ptr;
	ptr->previous = q;
    }
    return val;
}

int menu::add_item(const int mid, const char *name,
	  int (filebuffer::*function)(void), char *(filebuffer::*tfcn)(const char *)) {
    for (menu_bar  *ptr = list; ptr != NULL; ptr = ptr->next) {
	if (ptr->id == mid) {
	    ptr->add_item(name, function, tfcn);
	    return 1;
	}
    }
    return 0;
}

int menu::add_item(const char *mt, const char *name,
	  int (filebuffer::*function)(void), char *(filebuffer::*tfcn)(const char *)) {
    for (menu_bar  *ptr = list; ptr != NULL; ptr = ptr->next) {
	if (strcasecmp(ptr->title, mt) == 0) {
	    ptr->add_item(name, function, tfcn);
	    return 1;
	}
    }
    return 0;
}


int menu::active(WINDOW *win, const int y, const int x, filebuffer* fb) {
    static menu_bar   *ptr = list;
    static int	      lastlang = -1;
    int 	      key;

    if (lastlang != filebuffer::is_big5()) {
	lastlang = filebuffer::is_big5();
	ptr = list;
    }

//  char menuBar[MAX_BUFFER_LEN];
//  int  i = 0;

//  for (menu_bar  *ptr = list; ptr != NULL; ptr = ptr->next) {
//	sprintf(&menuBar[i], "%s  ", ptr->title);
//	i = strlen(menuBar);
//  }

    WINDOW  *winlist = NULL;

    screen_x = x;
    screen_y = y;

    sprintf(fmt, "%%-%ds", x);

//  mw = subwin(win, 1, screen_x, 0, 0);
    mw = newwin(1, screen_x, 0, 0);

    keypad(mw, TRUE);
    nodelay(mw, FALSE);

    int        finish = 0;
    int        sel = -2;
    int        rtnval = 0;

    show_title_bar();
    show_list(winlist, ptr, sel, fb);

    while (! finish) {
	switch(key = wgetch(mw)) {
	case KEY_LEFT:
//	case 'j':
//	case 'J':
	    if (ptr->previous != NULL) {
		ptr = ptr->previous;
	    } else {
		while (ptr->next != NULL) ptr = ptr->next;
	    }

	    if (winlist != NULL) {
		delwin(winlist);
		touchwin(win);
		wrefresh(win);
		winlist = NULL;
	    }
	    show_title_bar();
	    sel = -2;
	    show_list(winlist, ptr, sel, fb);
	    break;
	case KEY_RIGHT:
//	case 'l':
//	case 'L':
	    if (ptr->next != NULL) {
		ptr = ptr->next;
	    } else {
		ptr = list;
	    }
	    if (winlist != NULL) {
		delwin(winlist);
		touchwin(win);
		wrefresh(win);
		winlist = NULL;
	    }
	    show_title_bar();
	    sel = -2;
	    show_list(winlist, ptr, sel, fb);

	    break;
	case KEY_UP:
//	case 'i':
//	case 'I':
	    sel--;
	    show_list(winlist, ptr, sel, fb, 1);
	    break;
	case KEY_DOWN:
//	case 'k':
//	case 'K':
	    sel++;
	    show_list(winlist, ptr, sel, fb);
	    break;
	case KEY_ESC:
	    break;
	case KEY_ENTER:
	case KEY_RETURN:
	case KEY_LINEFEED:
	    if (function != (int (filebuffer::*)(void)) NULL) {
		rtnval = (fb->*function)();
	    }
	    // do something
	case ' ':
	case KEY_F(1):
	    finish = 1;
	    break;
	default:
	    if ((key >= 'a' && key <= 'z') ||
		(key >= 'A' && key <= 'Z') ||
		(key >= '0' && key <= '9')) {

		if (key >= 'A' && key <= 'Z') key = key + 'a' - 'A';

		for (menu_list *mp = ptr->ml; mp != NULL; mp = mp->next) {
		    if (mp->keyidx != -1) {
			int k = mp->title[mp->keyidx];
			if (k >= 'A' && k <= 'Z') k = k + 'a' - 'A';

			if (k == key) {
			    function = mp->action;
			    if (function != (int (filebuffer::*)(void)) NULL) {
				rtnval = (fb->*function)();
				finish = 1;
				break;
			    }
			}
		    }
		}
	    } else {
		if (key < ' ') finish = 1;
	    }
	    break;
	}
    }

    delwin(mw);
    touchwin(win);
    wrefresh(win);
    return rtnval;
}

void menu::show_list(WINDOW*& win, menu_bar *mb, int& selected, filebuffer *fb, const int up) {
    setattr(mw, ATTRIBUTE_menu_bar_select);
    mvwprintw(mw, 0, mb->id, "%s", mb->title);
    wmove(mw, 0, screen_x-1);

    function = (int (filebuffer::*)(void)) NULL;

    if (mb->height > 0) {
	char  fmt[10];

	sprintf(fmt, " %%-%ds ", mb->width);

	win = newwin(mb->height + 2, mb->width + 4, 1, mb->id);
	setattr(win, ATTRIBUTE_menu_border);

//	if (filebuffer::is_big5()) {
//	    box(win, '|', '-');
//	} else {
//	    box(win, 0, 0);
//	}
	box(win, 0, 0);

	if (selected != -2) mb->sel = (selected + mb->height) % mb->height;
	selected = mb->sel;

	menu_list *ptr;
	int	  i;

	for (i = 0, ptr = mb->ml; ptr != NULL; ptr = ptr->next, i++) {
	    if (i == selected) {
		if (ptr->action == (int (filebuffer::*)(void)) NULL) {
		    if (up) {
			mb->sel = (mb->sel - 1 + mb->height)  % mb->height;
		    } else {
			mb->sel = (mb->sel + 1) % mb->height;
		    }
		    selected = mb->sel;
		}
		break;
	    }
	}

	for (i = 0, ptr = mb->ml; ptr != NULL; ptr = ptr->next, i++) {
	    if (i == selected) {
		setattr(win, ATTRIBUTE_menu_item_select);
		function = ptr->action;
	    } else {
		setattr(win, ATTRIBUTE_menu_item);
	    }

	    if (ptr->action == (int (filebuffer::*)(void)) NULL) {
#if defined(__FreeBSD__) || defined(FREEBSD)
		wmove(win, i+1, 1);
		whline(win, 0, mb->width+2);
#else
		mvwhline(win, i+1, 1, 0, mb->width+2);
#endif
	    } else {
		if (ptr->getTitle == (char* (filebuffer::*)(const char *)) NULL) {
		    mvwprintw(win, i+1, 1, fmt, ptr->title);
		} else {
		    char* (filebuffer::*fcn)(const char *) =  ptr->getTitle;

		    mvwprintw(win, i+1, 1, fmt, (fb->*fcn)(ptr->title));
		}
		if (i != selected && ptr->keyidx != -1) {
		    setattr(win, ATTRIBUTE_menu_item_hotkey);
		    mvwaddch(win, i+1, ptr->keyidx+2, ptr->title[ptr->keyidx]);
		}
	    }
	}

	// wmove(win, selected + 1, mb->width+2);

	touchwin(win);
	wrefresh(win);
    }
}

void menu::show_title_bar(void) {
    setattr(mw, ATTRIBUTE_menu_bar);
    mvwprintw(mw, 0, 0, fmt, bar);
    touchwin(mw);
    wrefresh(mw);
}

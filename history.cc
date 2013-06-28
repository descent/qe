/*
 *  history.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <string.h>
#include "config.h"
#include "history.h"
#include "colors.h"
#include "keymap.h"
#include "linebuffer.h"
#include "filebuffer.h"

cmdHistory::cmdHistory(void) {
    init_cmdHistory();
}

cmdHistory::cmdHistory(const int  n) {
    init_cmdHistory();
    max_record = n;
}

void cmdHistory::init_cmdHistory(void) {
    head       = NULL;
    tail       = NULL;
    max_record = 50;
    record_no  = 0;
}

cmdHistory::~cmdHistory(void) {
    while (head != NULL) {
	tail = head->next;
	head->~linebuffer();
	delete head;
	head = tail;
    }
}

void cmdHistory::push(const char *str) {
    linebuffer *ptr = new linebuffer(str);

    ptr->next	  = NULL;

    if (head == NULL) {
	head = ptr;
	tail = ptr;
	ptr->previous = NULL;
    } else {
	ptr->previous = tail;
	tail->next    = ptr;
	tail	      = ptr;
    }

    if (record_no >= max_record) {
	ptr = head;
	head = head->next;
	head->previous = NULL;

	ptr->~linebuffer();
	delete ptr;
    } else {
	record_no++;
    }
}

char* cmdHistory::pop(void) {
    static char buffer[MAX_BUFFER_LEN+1];

    if (tail == NULL) {
	buffer[0] = '\0';
    } else {
	strcpy(buffer, tail->getString());
	linebuffer *ptr = tail->previous;

	tail->~linebuffer();
	delete tail;
	tail = ptr;
	if (tail != NULL) {
	    tail->next = NULL;
	} else {
	    head = NULL;
	}

	record_no--;
    }
    return buffer;
}

char* cmdHistory::top(void) {
    static char buffer[MAX_BUFFER_LEN+1];

    if (tail == NULL) {
	buffer[0] = '\0';
    } else {
	strcpy(buffer, tail->getString());
    }
    return buffer;
}

int cmdHistory::display_history(linebuffer *ptr, const linebuffer *sel) {
    char       buffer[MAX_BUFFER_LEN+1];
    char       fmt[10];
    linebuffer *p = ptr;
    int        yval = height;

    sprintf(fmt, " %%-%ds ", width);

    for (int i = 0; i < height; i++) {
	if (p != NULL) {
	    if (p == sel) {
		setattr(win, ATTRIBUTE_cmdhistory_select);
		yval = height - i;
	    } else {
		setattr(win, ATTRIBUTE_cmdhistory_text);
	    }
	    strncpy(buffer, p->getString(), width);
	    buffer[width] = '\0';
	    p = p->previous;
	} else {
	    strcpy(buffer, "<empty>");
	}
	mvwprintw(win, height - i, 1, fmt, buffer);
    }
    wmove(win, yval, width+2);
    return 0;
}

int cmdHistory::active(char *cmdline, WINDOW *pwin,
	   const int screen_y, const int screen_x, const int h, const int w) {

    int        key;
    linebuffer *sel, *bottom;
    int        dist;

    height = minimum(h, record_no);
    width  = w;

    win = newwin(height + 2, width + 4, screen_y - 5 - height, 0);
    keypad(win, TRUE);
    nodelay(win, FALSE);
    setattr(win, ATTRIBUTE_cmdhistory_border);
    box(win, 0, 0);

    sel     = tail;
    bottom  = tail;
    dist    = 0;

    display_history(bottom, sel);
    int finish;

    for (finish = 0; ! finish; ) {
	switch (key = wgetch(win)) {
	case KEY_UP:
	    if (sel->previous != NULL) {
		sel = sel->previous;
		if (dist < height-1) {
		    dist++;
		} else {
		    bottom = bottom->previous;
		}
		display_history(bottom, sel);
	    }
	    break;
	case KEY_DOWN:
	    if (sel->next != NULL) {
		sel = sel->next;
		if (dist > 0) {
		    dist--;
		} else {
		    bottom = bottom->next;
		}
		display_history(bottom, sel);
	    } else {
		finish = 1;
	    }
	    break;
	case KEY_LINEFEED:
	case KEY_RETURN:
	case KEY_ENTER:
	    if (sel != NULL) {
		strcpy(cmdline, sel->getString());
		finish = 2;
	    } else {
		finish = 1;
	    }
	    break;
	case ' ':
	    finish = 1;
	    break;
	}
    }

    delwin(win);
    touchwin(pwin);
    wrefresh(pwin);
    return finish-1;
}

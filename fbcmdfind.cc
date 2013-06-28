/*
 *  fbcmdfind.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "colors.h"
#include "linebuffer.h"
#include "filebuffer.h"
#include "keymap.h"
#include "messages.h"

int filebuffer::cmd_search(void) {
    static char  str[21];
    static int	 idx = 0;
    int 	 key;
    int 	 dir = 0;
    int 	 length = 0;
    int 	 wrapped = 0;
    const char	 *fmtstr =
		  "ENTER,UP,DOWN: search,  Ctrl-G: stop. Search: %s";

    str[idx] = '\0';
    display_messages(fmtstr, str);

    write_workbuffer(QE_IF_NECESSARY);

    while (1) {
	for (int finish = 0; ! finish; ) {
	    if ((key = wgetch(win)) >= ' ' && key < 255 && key != 127) {
		str[idx] = key;
		idx = (idx < 20) ? idx+1 : idx;
	    } else {
		if (length != 0) {
		    // setattr(win, ATTRIBUTE_normal_text);
		    ca_attrset();
		    mvwaddnstr(win, cursor_y, cursor_x,
					     &((*current)[buffer_x]), length);
		    length = 0;
		}

		switch (key) {
		case KEY_UP:
		    dir = 1;
		    finish = 1;
		    break;
		case KEY_DOWN:
		    dir = 0;
		    finish = 1;
		    break;
		case KEY_ENTER:
		case KEY_RETURN:
		case KEY_LINEFEED:
		    finish = 1;
		    break;
		case KEY_LEFT:
		case KEY_DC:
		case KEY_BACKSPACE:
		    idx = (idx > 0) ? idx-1 : idx;
		    str[idx] = '\0';
		    display_messages(fmtstr, str);
		    break;
		case KEY_CTRL('g'):
		    cmd_cursor_data();
		    display_messages();
		    return 0;
		default:
		    break;
		}
	    }
	    str[idx] = '\0';
	    display_messages(fmtstr, str);
	}

	if (idx != 0) {
	    if (cmd_find(str, dir)) {
		length = idx;
		setattr(win, ATTRIBUTE_find_text);
		mvwaddnstr(win, cursor_y, cursor_x,
						&((*current)[buffer_x]), idx);
	    } else {
		int   x = buffer_x, y = buffer_y;

		if (wrapped == 0) {
		    wrapped = 1;
		    display_messages(MSG_NO_MATCH_CTRL_G);
		} else {
		    wrapped = 0;

		    if (dir == 0) {
			moveto(0, 0);
		    } else {
			moveto(total_line, screen_x-1);
		    }

		    if (cmd_find(str, dir)) {
			length = idx;
			setattr(win, ATTRIBUTE_find_text);
			mvwaddnstr(win, cursor_y, cursor_x,
						 &((*current)[buffer_x]), idx);
			display_messages(MSG_SEARCH_WRAPPED);
		    } else {
			moveto(y, x);
			display_messages(MSG_NO_MATCH_CTRL_G);
		    }
		}
	    }
	}
    }
    return 0;
}

int filebuffer::cmd_change(const char *from, const char *to, const int dir,
					 const int mk, int confirm, int all) {
    char   tmpbuf[MAX_BUFFER_LEN+1];
    int    doit;
    int    len	= strlen(from);
    int    len2 = strlen(to);



    while (cmd_find(from, dir, mk)) {
	if (confirm) {
	    if (read_only) {
		display_messages(MSG_READ_ONLY_FILE);
		return 0;
	    }

	    setattr(win, ATTRIBUTE_find_text);
	    mvwaddnstr(win, cursor_y, cursor_x, &((*current)[buffer_x]), len);

	    display_messages(MSG_CONFIRM_CHANGE);
	    while (1) {
		int k = wgetch(win);

		if (k == 'y' || k == 'Y' || k == 'a' || k == 'A') {
		    display_messages(MSG_CHANGE_IT);
		    doit = 1;
		    if (k == 'a' || k == 'A') {
			confirm = 0;
			all = 1;
		    }
		    break;
		} else if (k == 'n' || k == 'N' || k == 'q' || k == 'Q') {
		    // setattr(win, ATTRIBUTE_normal_text);
		    ca_attrset();
		    mvwaddnstr(win, cursor_y, cursor_x,
				       current->getString(buffer_x), len);
//						 &(*current)[buffer_x], len);
		    display_messages(MSG_NO_CHANGE);
		    doit = 0;
		    if (k == 'q' || k == 'Q') return 0;
		    break;
		}
	    }
	} else {
	    doit = 1;
	}

	if (doit) {
	    if (read_only) {
		display_messages(MSG_READ_ONLY_FILE);
	    } else {
		dirty_buffer(1);
		strncpy(tmpbuf, current->getString(), buffer_x);
		strcpy(&tmpbuf[buffer_x], to);
		strcpy(&tmpbuf[strlen(tmpbuf)],
			      current->getString(buffer_x + strlen(from)));
//				      &(*current)[buffer_x + strlen(from)]);
		// current->assign(tmpbuf);
		*current = tmpbuf;
		load_workbuffer();
		refresh_bufferline();
		moveto(buffer_y, buffer_x + len2);
	    }
	}

	if (! all) return 0;
    }

    return 0;
}

int filebuffer::cmd_find(const char *str, const int dir, const int mk) {
    int   x = buffer_x, y = buffer_y;
    int   u = 0, d = total_line;
    int   l = 0, r = MAX_BUFFER_LEN;
    int   no_need_to_search = 0;
    // int   len = strlen(str);

    if (mk) {
	if (current_mark == UNKNOW_MARK) {
	    no_need_to_search = 1;
	} else {
	    if (current_mark == BLOCK_MARK) {
		l = minimum(current_mark.fc, current_mark.lc);
		r = maximum(current_mark.fc, current_mark.lc);
	    }

	    u = minimum(current_mark.fn, current_mark.ln);
	    d = maximum(current_mark.fn, current_mark.ln);

	    if (dir) {
		if (buffer_y < u) no_need_to_search = 1;
	    } else {
		if (buffer_y > d) no_need_to_search = 1;
	    }
	}
    }

    if (! no_need_to_search) {
	linebuffer    *ptr = current;

	write_workbuffer(QE_IF_NECESSARY);

	for (int found = 0;; found = 0) {
	    if (search_current_line(str, dir, ptr, x)) {
		found = 1;
	    } else if (search_next_line(str, dir, ptr, y, x)) {
		found = 1;
		load_workbuffer();
	    }

	    if (found) {
		if (y >= u && y <= d && x >= l && x <= r) {
		    moveto(y, x + strlen(str));
		    moveto(y, x);
		    // setattr(win, ATTRIBUTE_find_text);
		    // mvwaddnstr(win, cursor_y, cursor_x,
		    //				 &(*current)[buffer_x], len);

		    // setattr(win, ATTRIBUTE_normal_text);
		    return 1;
		}
	    } else {
		break;
	    }
	}
    }

    display_messages(MSG_NO_MATCH_FOUND);
    return 0;
}

int filebuffer::search_current_line(const char *sstr, const int dir,
						   linebuffer* lptr, int &x) {
    int        sstrlen = strlen(sstr);
    char       first   = sstr[0];

    if (first >= 'a' && first <= 'z') {
	first = first - 'a' + 'A';
    } else if (first >= 'A' && first <= 'Z') {
	first = first - 'A' + 'a';
    }

    int  len = lptr->getLength();

    if (dir) {
	for (int i = x - 1; i >= 0; i--) {
	    if ((*lptr)[i] == sstr[0] || (*lptr)[i] == first) {
		if (stringncompare(&((*lptr)[i]), sstr, sstrlen) == 0) {
		    x = i;
		    return 1;
		}
	    }
	}
    } else {
	for (int i = x + 1; i <= len - sstrlen; i++) {
	    if ((*lptr)[i] == sstr[0] || (*lptr)[i] == first) {
		if (stringncompare(&((*lptr)[i]), sstr, sstrlen) == 0) {
		    x = i;
		    return 1;
		}
	    }
	}
    }
    return 0;
}

int filebuffer::search_next_line(const char *sstr, const int  dir,
					 linebuffer*& lptr, int &y, int &x) {
    int        sstrlen = strlen(sstr);
    int        lineno;
    char       first   = sstr[0];

    if (first >= 'a' && first <= 'z') {
	first = first - 'a' + 'A';
    } else if (first >= 'A' && first <= 'Z') {
	first = first - 'A' + 'a';
    }

    if (dir) {	// reverse search
	   lineno = y - 1;

	for (linebuffer *ptr = lptr->previous; ptr != head;
					       ptr = ptr->previous, lineno--) {
	    int  len = ptr->getLength();

	    for (int i = len - sstrlen; i >= 0; i--) {
		if ((*ptr)[i] == sstr[0] || (*ptr)[i] == first) {
		    if (stringncompare(&((*ptr)[i]), sstr, sstrlen) == 0) {
			lptr = ptr;
			y    = lineno;
			x    = i;
			return 1;
		    }
		}
	    }
	}
    } else {
	lineno = y + 1;

	for (linebuffer *ptr = lptr->next; ptr != tail;
						  ptr = ptr->next, lineno++) {
	    int  len = ptr->getLength();

	    for (int i = 0; i <= len - sstrlen; i++) {
		if ((*ptr)[i] == sstr[0] || (*ptr)[i] == first) {
		    if (stringncompare(&((*ptr)[i]), sstr, sstrlen) == 0) {
			lptr = ptr;
			y    = lineno;
			x    = i;
			return 1;
		    }
		}
	    }
	}
    }
    return 0;
}



//  int filebuffer::isearch(const char *str) {
//  }
//
//  int filebuffer::isearch_backward(const char *str) {
//  }


int filebuffer::stringcompare(const char *s1, const char *s2) {
    if (search_case) {
	return ::strcmp(s1, s2);
    } else {
	return ::strcasecmp(s1, s2);
    }
}

int filebuffer::stringncompare(const char *s1, const char *s2, const int n) {
    if (search_case) {
	return ::strncmp(s1, s2, n);
    } else {
	return ::strncasecmp(s1, s2, n);
    }
}

/*
 *  fbcmdmove.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "colors.h"
#include "keymap.h"
#include "filebuffer.h"


int filebuffer::cmd_beginline(void) {
    if (mode == FB_DATAAREA_MODE) {
	cursor_x = buffer_x = workbufidx = 0;
	if (left_col_no != 0) {
	    left_col_no = 0;
	    refresh_clientarea(1);
	}
    } else {
	if (cmdline_idx < screen_x -1) {
	    cmdline_idx = 0;
	} else {
	    cmdline_idx = 0;
	    refresh_cmdarea();
	}
    }
    return 0;
}

int filebuffer::cmd_endline(void) {
    if (mode == FB_DATAAREA_MODE) {
	buffer_x = workbuflen;
	workbufidx = workbuflen;

	if (workbuflen < screen_x) {
	    cursor_x = workbuflen;
	    if (left_col_no != 0) {
		left_col_no = 0;
		refresh_clientarea(1);
	    }
	} else {
	    unsigned int   tmpval = left_col_no;
	    cursor_x = screen_x - 1;
	    left_col_no = workbuflen - screen_x + 1;
	    if (left_col_no != tmpval) {
		refresh_clientarea(1);
	    }
	}
    } else {
	int  oldval = cmdline_idx;

	if ((cmdline_idx = strlen(cmdline)) != oldval) {
	    refresh_cmdarea();
	}
    }
    return 0;
}

int filebuffer::cmd_left(void) {
    if (mode == FB_DATAAREA_MODE) {
	workbufidx = workbufidx > 0 ? workbufidx - 1 : 0;
	buffer_x   = workbufidx;

	if (cursor_x > 0) {
	    cursor_x--;
	} else {
	    if (left_col_no != 0) {
		left_col_no--;
		refresh_clientarea(1);
	    }
	}
    } else {
	int oldval = cmdline_idx;

	cmdline_idx = cmdline_idx > 0 ? cmdline_idx - 1 : 0;

	if (oldval > screen_x + 1)
	    refresh_cmdarea();
    }
    return 0;
}

int filebuffer::cmd_right(void) {
    if (mode == FB_DATAAREA_MODE) {
	if (workbufidx < MAX_BUFFER_LEN-2) {
	    workbufidx = workbufidx + 1;
	    buffer_x   = workbufidx;
	    if (cursor_x < screen_x - 1) {
		cursor_x++;
	    } else {
		left_col_no++;
		refresh_clientarea(1);
	    }
	}
    } else {
	int  len = strlen(cmdline);
	cmdline_idx = cmdline_idx < len ? cmdline_idx + 1 : len;

	if (cmdline_idx >= screen_x + 1)
	    refresh_cmdarea();
    }
    return 0;
}

int filebuffer::cmd_tab(void) {
    for (int i = buffer_x % TAB_SPACE; i < TAB_SPACE; i++)
	cmd_right();
    return 0;
}


////////////////////////////////////////////////////////////////////////


int filebuffer::cmd_up(void) {
    cmd_cursor_data();

    if (buffer_y > 0) {
	write_workbuffer(QE_IF_NECESSARY);
	buffer_y--;
	current = current->previous;
	if (cursor_y == 0) {
	    top_line_no--;
	    topline = topline->previous;
	    refresh_clientarea(0);
	} else {
	    cursor_y--;
	}
	load_workbuffer();
    } else {
	if (top_line_no != -1) {
	    top_line_no = -1;
	    topline = head;
	    cursor_y++;
	    refresh_clientarea(1);
	}
    }
    return 0;
}

int filebuffer::cmd_down(void) {
    cmd_cursor_data();

    if (buffer_y < total_line - 1) {
	write_workbuffer(QE_IF_NECESSARY);
	buffer_y++;
	current = current->next;
	if (cursor_y == screen_y-4) {
	    top_line_no++;
	    topline = topline->next;
	    refresh_clientarea(0);
	} else {
	    cursor_y++;
	}
	load_workbuffer();
    } else {
	if (cursor_y == screen_y-4) {
	    top_line_no++;
	    topline = topline->next;
	    cursor_y--;
	    refresh_clientarea(1);
	}
    }
    return 0;
}

int filebuffer::cmd_pgup(void) {
    if (top_line_no != -1) {
	write_workbuffer(QE_IF_NECESSARY);
	for (int i = 1; top_line_no >= 0 && i < screen_y-3; i++) {
	    topline = topline->previous;
	    top_line_no--;

	    if (buffer_y > 0) {
		current = current->previous;
		buffer_y--;
	    } else {
		cursor_y++;
	    }
      }
      refresh_clientarea(0);
      load_workbuffer();
    }
    return 0;
}

int filebuffer::cmd_pgdn(void) {
    if (top_line_no + screen_y - 4 < total_line) {
	write_workbuffer(QE_IF_NECESSARY);
	for (int i = 1; top_line_no + screen_y - 4 < total_line &&
				      i <  screen_y-3; i++, top_line_no++) {
	    topline = topline->next;
	    if (buffer_y < total_line - 1) {
		current = current->next;
		buffer_y++;
	    } else {
		cursor_y--;
	    }
	}
	refresh_clientarea(0);
	load_workbuffer();
    }
    return 0;
}

int filebuffer::cmd_top(void) {
    cmd_cursor_data();
    if (buffer_y != 0) {
	write_workbuffer(QE_IF_NECESSARY);
	buffer_y    = 0;
	cursor_y    = 1;
	top_line_no = -1;
	topline     = head;
	current     = head->next;
	refresh_clientarea(0);
	load_workbuffer();
    }

    return 0;
}

int filebuffer::cmd_bottom(void) {
    int    i;

    cmd_cursor_data();

    if (buffer_y != total_line - 1) {
	write_workbuffer(QE_IF_NECESSARY);

	buffer_y = total_line - 1;
	current  = tail->previous;

	topline     = current;
	top_line_no = buffer_y;

	for (i = 0; i < screen_y-5 && top_line_no >= 0 ; i++) {
	    topline = topline->previous;
	    top_line_no--;
	}
	cursor_y = i;
	refresh_clientarea(0);
	load_workbuffer();
    }
    return 0;
}

int filebuffer::cmd_top_edge(void) {
    int   updateflag = 0;

    cmd_cursor_data();

    while ((buffer_y > top_line_no) && (buffer_y > 0)) {
	if (! updateflag) write_workbuffer(QE_IF_NECESSARY);
	updateflag = 1;
	buffer_y--;
	cursor_y--;
	current = current->previous;
    }

    if (updateflag) load_workbuffer();

    return 0;
}

int filebuffer::cmd_bottom_edge(void) {
    int   updateflag = 0;

    cmd_cursor_data();

    while ((buffer_y < top_line_no + screen_y -4)&&(buffer_y < total_line-1)) {
	if (! updateflag) write_workbuffer(QE_IF_NECESSARY);
	updateflag = 1;
	buffer_y++;
	cursor_y++;
	current = current->next;
    }

    if (updateflag) load_workbuffer();

    return 0;
}

int filebuffer::cmd_scrollup(void) {
    cmd_cursor_data();

    if (top_line_no + screen_y - 4 < total_line) {
	write_workbuffer(QE_IF_NECESSARY);
	top_line_no++;
	topline = topline->next;
	if (buffer_y == total_line-1) {
	    cursor_y--;
	} else {
	    current = current->next;
	    buffer_y++;
	}
	load_workbuffer();
	refresh_clientarea(0);
    } else {
	return cmd_down();
    }
    // wprintw(win, "[scrollup]");
    return 0;
}

int filebuffer::cmd_scrolldown(void) {
    cmd_cursor_data();

    if (top_line_no > -1 && buffer_y > 0) {
	write_workbuffer(QE_IF_NECESSARY);
	top_line_no--;
	topline = topline->previous;
	current = current->previous;
	buffer_y--;
	load_workbuffer();
	refresh_clientarea(0);
    } else {
	return cmd_up();
    }
    return 0;
}

int filebuffer::cmd_scrollleft(void) {
    cmd_cursor_data();

    if (left_col_no + screen_x < MAX_BUFFER_LEN-1) {
	unsigned int  org = left_col_no;

	cmd_right();

	if (org == left_col_no) {
	    left_col_no++;
	    cursor_x--;
	    refresh_clientarea(1);
	}
    } else {
	cmd_right();
    }
    return 0;
}

int filebuffer::cmd_scrollright(void) {
    cmd_cursor_data();
    if (left_col_no > 0) {
	unsigned int  org = left_col_no;

	cmd_left();

	if (org == left_col_no) {
	    left_col_no--;
	    cursor_x++;
	    refresh_clientarea(1);
	}
    } else {
	cmd_left();
    }
    return 0;
}

int filebuffer::cmd_wordend(void) {
    cmd_cursor_data();

    if (workbuffer[workbufidx] != ' ') {
	for (; workbufidx < workbuflen; workbufidx++)
	    if (workbuffer[workbufidx + 1] == ' ') break;
	moveto(buffer_y, workbufidx);
    } else {
	int  found = 0;

	for (int i = workbufidx; i < workbuflen; i++) {
	    if (workbuffer[i] != ' ') {
		found = 1;
		workbufidx = i;
		cmd_wordend();
		break;
	    }
	}
	if (! found) {
	    for (int i = workbufidx; i >= 0; i--) {
		if (workbuffer[i] != ' ') {
		    workbufidx = i;
		    moveto(buffer_y, workbufidx);
		    break;
		}
	    }
	}
    }
    return 0;
}

int filebuffer::cmd_wordbegin(void) {
    cmd_cursor_data();

    if (workbuffer[workbufidx] != ' ') {
	for (; workbufidx > 0; workbufidx--)
	    if (workbuffer[workbufidx - 1] == ' ') break;
	moveto(buffer_y, workbufidx);
    } else {
	int  found = 0;

	for (int i = workbufidx; i >= 0; i--) {
	    if (workbuffer[i] != ' ') {
		found = 1;
		workbufidx = i;
		cmd_wordbegin();
		break;
	    }
	}
	if (! found) {
	    for (int i = workbufidx; i < workbuflen; i++) {
		if (workbuffer[i] != ' ') {
		    workbufidx = i;
		    moveto(buffer_y, workbufidx);
		    break;
		}
	    }
	}
    }
    return 0;
}

int filebuffer::cmd_centerline(void) {
    int    cl = (screen_y - 3) / 2;
    int    oldy = cursor_y;

    cmd_cursor_data();

    while (topline != head && cursor_y < cl) {
	topline = topline->previous;
	top_line_no--;
	cursor_y++;
    }

    while (cursor_y > cl) {
	topline = topline->next;
	top_line_no++;
	cursor_y--;
    }
    if (oldy != cursor_y) refresh_clientarea(1);
    return 0;
}

int filebuffer::cmd_backword(void) {
    int  i, found;

    for (i = buffer_x; i >= 0; i--)
	if (workbuffer[i] == ' ') break;

    for (found = 0; i >= 0; i--) {
	if (workbuffer[i] != ' ') {
	    found = 1;
	    break;
	}
    }

    if (found) {
	int need_refresh;

	for (; i > 0; i--)
	    if (workbuffer[i-1] == ' ') break;

	for (need_refresh = 0; workbufidx > i; workbufidx--, buffer_x--) {
	    if (cursor_x > 0) {
		cursor_x--;
	    } else {
		left_col_no--;
		need_refresh = 1;
	    }
	}
	if (need_refresh) refresh_clientarea(1);
    } else if (current->previous != head) {
	cmd_up();
	cmd_endline();
	return cmd_backword();
    }

    return 0;
}

int filebuffer::cmd_tabword(void) {
    static  int   level = 0;
    int 	  i, found;

    i = buffer_x;

    if (level++ == 0)
	for (; i < workbuflen; i++)
	    if (workbuffer[i] == ' ')
		break;

    for (found = 0; i < workbuflen; i++) {
	if (workbuffer[i] != ' ') {
	    found = 1;
	    break;
	}
    }

    if (found) {
	int need_refresh;

	for (need_refresh = 0; workbufidx < i; workbufidx++, buffer_x++) {
	    if (cursor_x < screen_x - 1) {
		cursor_x++;
	    } else {
		left_col_no++;
		need_refresh = 1;
	    }
	}
	if (need_refresh) refresh_clientarea(1);
    } else if (current->next != tail) {
	cmd_down();
	cmd_beginline();
	cmd_tabword();
    }
    level--;
    return 0;
}

void filebuffer::moveto(const int y, const int x) {
    int  need_refresh = 0;

    write_workbuffer(QE_IF_NECESSARY);

    if (y < top_line_no) {
	int yy = maximum(y, 0);

	for (; top_line_no > -1 && buffer_y > yy; buffer_y--) {
	    topline = topline->previous;
	    current = current->previous;
	    top_line_no--;
	}

	for (; buffer_y > yy; buffer_y--) {
	    current = current->previous;
	    cursor_y--;
	}

	need_refresh = 1;
    } else if (y > top_line_no + screen_y - 4) {
	int  yy = minimum(y, total_line-1);

	for (; buffer_y < yy; buffer_y++) {
	    topline = topline->next;
	    current  = current->next;
	    top_line_no++;
	}
	need_refresh = 1;
    } else {
	for (; buffer_y > y; buffer_y--) {
	    current = current->previous;
	    cursor_y--;
	}
	for (; buffer_y < y; buffer_y++) {
	    current = current->next;
	    cursor_y++;
	}
    }

    unsigned int xx = minimum(maximum(x, 0), MAX_BUFFER_LEN-1);

    if (xx < left_col_no || xx > left_col_no + screen_x - 1) {
	left_col_no = 0;

	for (left_col_no = 0; xx > left_col_no + screen_x - 1; left_col_no++)
	    ;
	need_refresh = 1;
    }

    buffer_x = xx;
    cursor_x = buffer_x - left_col_no;

    load_workbuffer();

    if (need_refresh) refresh_clientarea(0);
}


int filebuffer::cmd_gotoline(void) {
    WINDOW     *mw;
    char       buffer[MAX_BUFFER_LEN+1];
    int        key, idx = 0, len = 0;
    int        finish = 0;
    const char linenumstr[] = "Line Number: ";
    char       fmt[10];
    int        startpos = strlen(linenumstr) + 2;
    const int  numlen = 12;


    int width = startpos + numlen + 2;

    mw = newwin(3, width, screen_y / 2 - 2, (screen_x - width)/ 2 );

    setattr(mw, ATTRIBUTE_goto_text);

    sprintf(fmt, " %%-%ds", width-3);

    box(mw, 0, 0);
    mvwprintw(mw, 1, 1, fmt, linenumstr);
    keypad(mw, TRUE);
    nodelay(mw, FALSE);

    sprintf(fmt, "%%-%ds", numlen);

    wmove(mw, 1, startpos);

    while (! finish) {
	switch(key = wgetch(mw)) {
	case KEY_LEFT:
	    if (idx > 0) {
		idx--;
		wmove(mw, 1, startpos + idx);
	    }
	    break;
	case KEY_RIGHT:
	    if (idx < len) {
		idx++;
		wmove(mw, 1, startpos + idx);
	    }
	    break;
	case KEY_BACKSPACE:
	case KEY_DC:
	    if (idx < len) {
		for (int i = idx; i < len; i++)
		    buffer[i] = buffer[i+1];
		buffer[--len] = '\0';
		mvwprintw(mw, 1, startpos, fmt, buffer);
		wmove(mw, 1, startpos + idx);
	    }
	    break;
	case ' ':
	    finish = 1;
	    break;
	case KEY_ENTER:
	case KEY_RETURN:
	case KEY_LINEFEED:
	    finish = 2;
	    break;
	default:
	    if (key >= '0' && key <= '9') {
		if (idx < numlen) {
		    buffer[idx] = key;
		    mvwaddch(mw, 1, startpos + idx, key);
		    idx++;
		    len = maximum(len, idx);
		} else {
		    beep();
		}
	    }
	    break;
	}
    }

    delwin(mw);
    touchwin(win);
    wrefresh(win);

    if (finish == 2 && len > 0) {
	buffer[len] = '\0';
	moveto(atoi(buffer)-1, buffer_x);
    }

    return 0;
}

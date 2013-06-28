/*
 *  fbcmdmark.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "filebuffer.h"
#include "colors.h"
#include "mark.h"
#include "messages.h"


static int  copyok = 0;

void filebuffer::ca_attrset(void) {
    if (current_mark != UNKNOW_MARK && current_mark.fb == this) {
	int u = minimum(current_mark.fn, current_mark.ln);
	int d = maximum(current_mark.fn, current_mark.ln);
	if (buffer_y >= u && buffer_y <= d) {
	    if (current_mark == BLOCK_MARK) {
		int bml = minimum(current_mark.fc, current_mark.lc);
		int bmr = maximum(current_mark.fc, current_mark.lc);
		setattr(win, (buffer_x >= bml && buffer_x <= bmr) ?
			       ATTRIBUTE_mark_text : ATTRIBUTE_normal_text);
	    } else {
		setattr(win, ATTRIBUTE_mark_text);
	    }
	} else {
	    setattr(win, ATTRIBUTE_normal_text);
	}
    } else {
	setattr(win, ATTRIBUTE_normal_text);
    }
}

static void substrncpy_with_blankfill(char *buffer, const char *str,
		 const int n, const int len) {

    int l = minimum(len, MAX_BUFFER_LEN-1);
    strncpy(buffer, (strlen(str) < (unsigned int) n) ? " " : &str[n], l);
    for (int i = strlen(buffer); i < l; i++)
	buffer[i] = ' ';
    buffer[l] = '\0';
}

int filebuffer::cmd_clear_marks(void) {
    marksp = -1;
    return 0;
}

int filebuffer::cmd_pushmark(void) {
    int  need_redraw = (current_mark.fb == this);

    if (push_mark()) {
	if (need_redraw) refresh_clientarea(1);
    } else {
	display_messages(MSG_TOO_MANY_MARK_SAVE);
    }
    return 0;
}

int filebuffer::cmd_popmark(void) {
    int  need_redraw = (current_mark.fb == this);

    if (pop_mark()) {
	if (need_redraw || current_mark.fb == this) refresh_clientarea(1);
    } else {
	display_messages(MSG_NO_MARK_SAVED);
    }
    return 0;
}

int filebuffer::cmd_markline(void) {
    if (current_mark == UNKNOW_MARK) {
	/* mark line */
	current_mark.fb    = this;
	current_mark.first = current;
	current_mark.last  = current;
	current_mark.fn    = buffer_y;
	current_mark.ln    = buffer_y;
	current_mark.settype(LINE_MARK);
	refresh_clientarea(1);
    } else if (current_mark.fb != this) {
	display_messages(MSG_CONFLICT_EXIST_MARK);
    } else if (current_mark == LINE_MARK) {
	current_mark.last  = current;
	current_mark.ln    = buffer_y;
	refresh_clientarea(1);
    } else {
	display_messages(MSG_CONFLICT_EXIST_MARK);
    }
    return 0;
}

int filebuffer::cmd_markblock(void) {
    if (current_mark == UNKNOW_MARK) {
	current_mark.fb    = this;
	current_mark.first = current;
	current_mark.fn    = buffer_y;
	current_mark.fc    = buffer_x;
	current_mark.last  = current;
	current_mark.ln    = buffer_y;
	current_mark.lc    = buffer_x;
	current_mark.settype(BLOCK_MARK);
	refresh_clientarea(1);
    } else if (current_mark.fb != this) {
	display_messages(MSG_CONFLICT_EXIST_MARK);
    } else if (current_mark == BLOCK_MARK) {
       current_mark.last  = current;
	current_mark.ln    = buffer_y;
	current_mark.lc    = buffer_x;
	refresh_clientarea(1);
    } else {
	display_messages(MSG_CONFLICT_EXIST_MARK);
    }
    return 0;
}

int filebuffer::cmd_markchar(void) {
    return 0;
}

int filebuffer::cmd_unmark(void) {
    current_mark.settype(UNKNOW_MARK);
    if (current_mark.fb == this) refresh_clientarea(1);
    current_mark.fb = NULL;
    return 0;
}

int filebuffer::cmd_copymark(void) {
    copyok = 0;

    if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
	return 0;
    }

    if (read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }


    write_workbuffer(QE_IF_NECESSARY);
    int  u = minimum(current_mark.fn, current_mark.ln);
    int  d = maximum(current_mark.fn, current_mark.ln);

    if (current_mark == LINE_MARK) {
	if (buffer_y < u || buffer_y >= d || current_mark.fb != this) {
	    linebuffer *ptr;
	    linebuffer *q = new linebuffer("");
	    linebuffer *f = q;

	    ptr = (current_mark.fn < current_mark.ln) ? current_mark.first :
							current_mark.last;

	    for (int i = u; i <= d; i++, ptr = ptr->next) {
		q->next 	  = new linebuffer(*ptr);
		q->next->previous = q;
		q		  = q->next;
	    }
	    q->next		    = current->next;
	    q->next->previous	    = q;
	    current->next	    = f->next;
	    current->next->previous = current;
	    delete f;
	    total_line = total_line + d - u + 1;
	    current_mark.renumber();
	    // newline_flag = 1;
	    dirty_buffer(1);
	    refresh_clientarea(0);
	    copyok = 1;
	} else {
	    display_messages(MSG_S_T_CONFLICT);
	}
    } else if (current_mark == BLOCK_MARK) {
	int  from = minimum(current_mark.fc, current_mark.lc);
	int  to   = maximum(current_mark.fc, current_mark.lc);

	if (current_mark.fb == this &&
	   (buffer_y <= d) && (buffer_y + (d - u + 1) > u) &&
	   ! (buffer_y == u && (buffer_x > to || buffer_x <= from))) {
	    display_messages(MSG_S_T_CONFLICT);
	} else {
	    char	  wkbf[MAX_BUFFER_LEN+MAX_BUFFER_LEN+1];
	    linebuffer	  *p, *q = current;
	    int 	  mklen = to - from + 1;

	    p = (current_mark.fn < current_mark.ln) ? current_mark.first :
						      current_mark.last;

	    for (int i = u; i <= d; i++, p = p->next) {
		if (q == tail) {
		    q = new linebuffer("");
		    q->previous = tail->previous;
		    tail->previous->next = q;
		    tail->previous = q;
		    q->next = tail;
		    total_line++;
		}

		substrncpy_with_blankfill(wkbf, q->getString(), 0, buffer_x);
		substrncpy_with_blankfill(&wkbf[buffer_x], p->getString(),
								 from, mklen);
		if (q->getLength() > (unsigned int) buffer_x)
		    strcpy(&wkbf[buffer_x + mklen], &((*q)[buffer_x]));
		wkbf[MAX_BUFFER_LEN] = '\0';
		// q->assign(wkbf);
		*q = wkbf;
		q = q->next;
	    }

	    if (buffer_y == u && buffer_x < from) {
		current_mark.fc += mklen;
		current_mark.lc += mklen;
	    }
	    dirty_buffer(1);
	    refresh_clientarea(0);
	    copyok = 1;
	}
    }
    load_workbuffer();
    return 0;
}


int filebuffer::cmd_overlayblock(void) {
    if (read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }

    if (current_mark == BLOCK_MARK) {
	int  u = minimum(current_mark.fn, current_mark.ln);
	int  d = maximum(current_mark.fn, current_mark.ln);
	int  from = minimum(current_mark.fc, current_mark.lc);
	int  to   = maximum(current_mark.fc, current_mark.lc);
	int  mklen = to - from + 1;

	write_workbuffer(QE_IF_NECESSARY);

	if (current_mark.fb == this &&
	   (buffer_y <= d) && (buffer_y + (d - u + 1) > u) &&
	   ! (buffer_x > to || buffer_x + mklen <= from)) {
	    display_messages(MSG_S_T_CONFLICT);
	} else {
	    char	  wkbf[MAX_BUFFER_LEN+MAX_BUFFER_LEN+1];
	    linebuffer	  *p, *q = current;

	    p = (current_mark.fn < current_mark.ln) ? current_mark.first :
						      current_mark.last;

	    for (int i = u; i <= d; i++, p = p->next) {
		if (q == tail) {
		    q = new linebuffer("");
		    q->previous = tail->previous;
		    tail->previous->next = q;
		    tail->previous = q;
		    q->next = tail;
		    total_line++;
		}

		substrncpy_with_blankfill(wkbf, q->getString(), 0, buffer_x);
		substrncpy_with_blankfill(&wkbf[buffer_x], p->getString(),
								  from, mklen);
		if (q->getLength() > (unsigned int) buffer_x + mklen)
		    strcpy(&wkbf[buffer_x + mklen], &((*q)[buffer_x + mklen]));
		wkbf[MAX_BUFFER_LEN] = '\0';
		// q->assign(wkbf);
		*q = wkbf;
		q = q->next;
	    }

	    if (buffer_y == u && buffer_x < from) {
		current_mark.fc += mklen;
		current_mark.lc += mklen;
	    }
	    dirty_buffer(1);
	    refresh_clientarea(0);
	}

	load_workbuffer();
    } else {
	display_messages(MSG_BLOCK_MARK_REQ);
    }
    return 0;
}

int filebuffer::cmd_movemark(void) {
    if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
	return 0;
    }

    if (read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }

    cmd_copymark();
    if (copyok) cmd_deletemark();
    return 0;
}

int filebuffer::cmd_deletemark(void) {
    // wprintw(win, "[delete mark]");
    if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
	return 0;
    }

    if (current_mark.fb->read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }

    filebuffer *ptr = current_mark.fb;
    int        u    = minimum(current_mark.fn, current_mark.ln);
    int        d    = maximum(current_mark.fn, current_mark.ln);
    linebuffer *lbf = (current_mark.fn < current_mark.ln) ?
					current_mark.first : current_mark.last;

    current_mark.fb->write_workbuffer(QE_IF_NECESSARY);

    if (current_mark == LINE_MARK) {
	int yy = ptr->buffer_y;

	if (ptr->buffer_y >= u && ptr->buffer_y <= d) {
	    if ((yy = u - 1) < 0) yy = 0;
	} else if (ptr->buffer_y > d) {
	    yy = yy - (d - u + 1);
	}

	quiet_flag = 1;
	ptr->moveto(u, ptr->buffer_x);
	for (int i = u; i <= d; i++) ptr->cmd_deleteline();
	ptr->moveto(yy, ptr->buffer_x);
	quiet_flag = 0;

    } else if (current_mark == BLOCK_MARK) {
	int  from  = minimum(current_mark.fc, current_mark.lc);
	int  to    = maximum(current_mark.fc, current_mark.lc);
	int  mklen = to - from + 1;

	for (int i = u; i <= d; i++, lbf = lbf->next) {
	    int  len = lbf->getLength();

	    if (from <= len) {
		if (to >= len) {
		    (*lbf)[from] = '\0';
		} else {
		    for (int j = to+1; j <= len; j++)
			(*lbf)[j - mklen] = (*lbf)[j];
		}
		lbf->setLength();
	    }
	}
    }

    current_mark.fb->dirty_buffer(1);
    current_mark.settype();

    if (current_mark.fb == this) {
	// newline_flag = 1;
	load_workbuffer();
	refresh_clientarea(0);
    }

    return 0;
}

int filebuffer::cmd_fillmark(void) {
    char  tmpbuf[MAX_BUFFER_LEN+1];
    int   i, magic;


    if (current_mark != UNKNOW_MARK && current_mark.fb->read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }

    if (current_mark == BLOCK_MARK) {
	display_messages(MSG_TYPE_A_CHARACTER);
	if ((magic = getfirstkey()) >= ' ' && magic <= 126) {
	    int  from = minimum(current_mark.fc, current_mark.lc);
	    int  to   = maximum(current_mark.fc, current_mark.lc);

	    int  lf = minimum(current_mark.fn, current_mark.ln);
	    int  lt = maximum(current_mark.fn, current_mark.ln);

	    current_mark.fb->write_workbuffer(QE_IF_NECESSARY);

	    linebuffer *lb = (current_mark.fn < current_mark.ln) ?
			      current_mark.first : current_mark.last;

/*
	    if (current_mark.fb == this)
		if (buffer_y >= lf && buffer_y <= lt)
		    write_workbuffer();
*/

	    for (int j = lf; j <= lt; j++, lb = lb->next) {
		strncpy(tmpbuf, lb->getString(), MAX_BUFFER_LEN);
		int  len = strlen(tmpbuf);

		for (i = len; i < from; i++)
		    tmpbuf[i] = ' ';
		for (i = from; i <= to; i++)
		    tmpbuf[i] = magic;
		tmpbuf[maximum(i, len)] = '\0';
		// lb->assign(tmpbuf);
		*lb = tmpbuf;
	    }
	    // current_mark.fb->newline_flag = 1;
	    current_mark.fb->dirty_buffer(1);
	    if (current_mark.fb == this) {
		if (buffer_y >= lf && buffer_y <= lt)
		    load_workbuffer();
		//  newline_flag = 1;
		refresh_clientarea(0);
		// refresh_bufferline();
	    }
	    display_messages("mark filled: %c", magic);
	    // display_messages("%d %d", lf, lt);
	} else {
	    display_messages("canceled");
	}
    } else if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
    } else {
	display_messages(MSG_BLOCK_MARK_REQ);
    }
    return 0;
}

int filebuffer::cmd_beginmark(void) {
    if (current_mark != UNKNOW_MARK) {
	filebuffer *ptr = current_mark.fb;
	linebuffer *ltr;
	int	   line;
	int	   need_redraw = 0;

	write_workbuffer(QE_IF_NECESSARY);

	if (current_mark.fn < current_mark.ln) {
	    line = current_mark.fn;
	    ltr  = current_mark.first;
	} else {
	    line = current_mark.ln;
	    ltr  = current_mark.last;
	}

	ptr->buffer_y = line;
	ptr->current  = ltr;

	if (line < ptr->top_line_no ||
	    line > ptr->top_line_no + ptr->screen_y - 4) {
	    ptr->top_line_no = line - 1;
	    ptr->cursor_y    = 1;
	    ptr->topline     = ltr->previous;
	    need_redraw      = 1;
	} else {
	    ptr->cursor_y = ptr->buffer_y - ptr->top_line_no;
	}

	if (current_mark == BLOCK_MARK) {
	    int raw = minimum(current_mark.fc, current_mark.lc);
	    if (raw < (int) ptr->left_col_no ||
		raw > (int) ptr->left_col_no + ptr->screen_x) {
		if (raw < ptr->screen_x) {
		    ptr->left_col_no = 0;
		} else {
		    ptr->left_col_no = raw - ptr->screen_x + 1;
		}
		need_redraw	 = 1;
	    }
	    ptr->buffer_x = raw;
	    ptr->cursor_x = ptr->buffer_x - ptr->left_col_no;
	}

	ptr->load_workbuffer();

	if (ptr != this) {
	    current_buffer = ptr;
	    return 1;
	} else if (need_redraw) {
	    refresh_clientarea(0);
	    // refresh_bufferline();
	}
    } else {
	display_messages(MSG_NO_MARKED_AREA);
    }
    return 0;
}

int filebuffer::cmd_endmark(void) {
    if (current_mark != UNKNOW_MARK) {
	filebuffer *ptr = current_mark.fb;
	linebuffer *ltr;
	int	   line;
	int	   need_redraw = 0;

	write_workbuffer(QE_IF_NECESSARY);

	if (current_mark.fn > current_mark.ln) {
	    line = current_mark.fn;
	    ltr  = current_mark.first;
	} else {
	    line = current_mark.ln;
	    ltr  = current_mark.last;
	}

	ptr->buffer_y = line;
	ptr->current  = ltr;

	if (line < ptr->top_line_no ||
	    line > ptr->top_line_no + ptr->screen_y - 4) {
	    ptr->top_line_no = line;
	    ptr->topline     = ltr;
	    ptr->cursor_y    = 0;
	    for (ptr->cursor_y = 0; ptr->cursor_y < ptr->screen_y - 5 &&
				    ptr->top_line_no != -1; ptr->cursor_y++) {
		ptr->top_line_no--;
		ptr->topline = ptr->topline->previous;
	    }
	    need_redraw      = 1;
	} else {
	    ptr->cursor_y = ptr->buffer_y - ptr->top_line_no;
	}

	if (current_mark == BLOCK_MARK) {
	    int raw = maximum(current_mark.fc, current_mark.lc);
	    if (raw < (int) ptr->left_col_no ||
		raw > (int) ptr->left_col_no + ptr->screen_x) {
		if (raw < ptr->screen_x) {
		    ptr->left_col_no = 0;
		} else {
		    ptr->left_col_no = raw - ptr->screen_x + 1;
		}
		need_redraw	 = 1;
	    }
	    ptr->buffer_x = raw;
	    ptr->cursor_x = ptr->buffer_x - ptr->left_col_no;
	}

	ptr->load_workbuffer();

	if (ptr != this) {
	    current_buffer = ptr;
	    return 1;
	} else if (need_redraw) {
	    refresh_clientarea(0);
	}
    } else {
	display_messages(MSG_NO_MARKED_AREA);
    }
    return 0;
}

int filebuffer::cmd_shiftleft(void) {
    char  tmpbuf[MAX_BUFFER_LEN+1];
    int   i;

    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    if (current_mark == BLOCK_MARK || current_mark == LINE_MARK) {
	if (current_mark.fb->read_only) {
	    display_messages(MSG_READ_ONLY_FILE);
	    return 0;
	}

	int  from = minimum(current_mark.fc, current_mark.lc);
	int  lf = minimum(current_mark.fn, current_mark.ln);
	int  lt = maximum(current_mark.fn, current_mark.ln);

	if (current_mark == LINE_MARK) from = 0;

	linebuffer *lb = (current_mark.fn < current_mark.ln) ?
			  current_mark.first : current_mark.last;

	if (current_mark.fb == this)
	    if (buffer_y >= lf && buffer_y <= lt)
		write_workbuffer(QE_IF_NECESSARY);

	for (int j = lf; j <= lt; j++, lb = lb->next) {
	    strncpy(tmpbuf, lb->getString(), MAX_BUFFER_LEN-1);
	    int  len = strlen(tmpbuf);

	    for (i = from; i < len; i++)
		tmpbuf[i] = tmpbuf[i+1];
	    tmpbuf[maximum(from, len)] = '\0';
	    // lb->assign(tmpbuf);
	    *lb = tmpbuf;
	}
	// current_mark.fb->newline_flag = 1;
	current_mark.fb->dirty_buffer(1);
	if (current_mark.fb->buffer_y >= lf && current_mark.fb->buffer_y <= lt)
	    current_mark.fb->load_workbuffer();

	if (current_mark.fb == this) {
	    refresh_clientarea(0);
	    if (buffer_y < lf || buffer_y > lt) refresh_bufferline();
	}
    } else if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
    } else {
	display_messages(MSG_BLOCK_MARK_REQ);
    }
    return 0;
}

int filebuffer::cmd_shiftright(void) {
    char  tmpbuf[MAX_BUFFER_LEN+1];
    int   i;

    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    if (current_mark == BLOCK_MARK || current_mark == LINE_MARK) {
	if (current_mark.fb->read_only) {
	    display_messages(MSG_READ_ONLY_FILE);
	    return 0;
	}

	int  from = minimum(current_mark.fc, current_mark.lc);
	int  lf = minimum(current_mark.fn, current_mark.ln);
	int  lt = maximum(current_mark.fn, current_mark.ln);

	if (current_mark == LINE_MARK) from = 0;

	linebuffer *lb = (current_mark.fn < current_mark.ln) ?
			  current_mark.first : current_mark.last;

	if (current_mark.fb == this)
	    if (buffer_y >= lf && buffer_y <= lt)
		write_workbuffer(QE_IF_NECESSARY);

	for (int j = lf; j <= lt; j++, lb = lb->next) {
	    strncpy(tmpbuf, lb->getString(), MAX_BUFFER_LEN-3);
	    tmpbuf[MAX_BUFFER_LEN-3] = '\0';
	    int  len = strlen(tmpbuf);

	    for (i = len; i < from; i++)
		tmpbuf[i] = ' ';

	    tmpbuf[len = (maximum(from, len))] = '\0';

	    for (i = len; i > from; i--)
		tmpbuf[i] = tmpbuf[i-1];
	    tmpbuf[from] = ' ';
	    // lb->assign(tmpbuf);
	    *lb = tmpbuf;
	}
	current_mark.fb->dirty_buffer(1);
	if (current_mark.fb->buffer_y >= lf && current_mark.fb->buffer_y <= lt)
	    current_mark.fb->load_workbuffer();

	if (current_mark.fb == this) {
	    refresh_clientarea(0);
	    if (buffer_y < lf || buffer_y > lt) refresh_bufferline();
	}
    } else if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
    } else {
	display_messages(MSG_BLOCK_MARK_REQ);
    }
    return 0;
}

void filebuffer::unmark_all_mark(void) {
    if (current_mark.fb == this) current_mark.settype(UNKNOW_MARK);

    for (int i = 0; i <= marksp; i++)
	if (markstack[i].fb == this) markstack[i].settype(UNKNOW_MARK);
}

void filebuffer::fix_all_mark_position(const int tp) {
    fix_mark_position(current_mark, tp);

    for (int i = 0; i <= marksp; i++)
	fix_mark_position(markstack[i], tp);
}

void filebuffer::fix_mark_position(mark &mk, const int tp) {
    if (mk != UNKNOW_MARK && mk.fb == this) {
	int	    *un,  *dn;
	linebuffer  **ul, **dl;

	if (mk.fn < mk.ln) {
	    un = &mk.fn;
	    ul = &mk.first;
	    dn = &mk.ln;
	    dl = &mk.last;
	} else {
	    dn = &mk.fn;
	    dl = &mk.first;
	    un = &mk.ln;
	    ul = &mk.last;
	}

	switch (tp) {
	case FIX_MARK_INSERT_LINE:
	    // mk.renumber();
	    if (buffer_y <= *un) (*un)++;
	    if (buffer_y <= *dn) (*dn)++;
	    break;
	case FIX_MARK_DELETE_LINE:
	    if (buffer_y < *un) {
		(*un)--;
		(*dn)--;
	    } else if (buffer_y <= *dn) {
		if (buffer_y == *un && buffer_y == *dn) {
		    mk.settype(UNKNOW_MARK);
		} else if (buffer_y == *un) {
		    *ul = (*ul)->next;
		} else if (buffer_y == *dn) {
		    *dl = (*dl)->previous;
		}
		(*dn)--;
	    }
	    break;
	case FIX_MARK_JOIN_LINE:
	    break;
	}
    }
}

int filebuffer::push_mark(void) {
    if (marksp >= markstacksize - 1) return 0;

    markstack[++marksp] = current_mark;
    current_mark.settype(UNKNOW_MARK);
    return 1;
}

int filebuffer::pop_mark(void) {
    if (marksp < 0) return 0;

    current_mark = markstack[marksp--];
    return 1;
}

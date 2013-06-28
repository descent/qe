/*
 *  fbcmdconv.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "filebuffer.h"
#include "messages.h"
#include "misc.h"

int filebuffer::cmd_uppercase(void) {
    if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
	return 0;
    }

    int        u = minimum(current_mark.fn, current_mark.ln);
    int        d = maximum(current_mark.fn, current_mark.ln);
    int        f = 0;
    int        t = MAX_BUFFER_LEN;

    if (current_mark == BLOCK_MARK) {
	f = minimum(current_mark.fc, current_mark.lc);
	t = maximum(current_mark.fc, current_mark.lc);
    }

    linebuffer *ptr = (current_mark.fn < current_mark.ln) ?
					current_mark.first : current_mark.last;

    if (current_mark.fb == this && buffer_y >= u && buffer_y <= d)
	write_workbuffer(QE_FORCE);

    int  i, j, len;

    for (i = u; i <= d; i++, ptr = ptr->next) {
	len = ptr->getLength();
	for (j = minimum(t, len); j >= f; j--) {
	    if ((*ptr)[j] >= 'a' && (*ptr)[j] <= 'z') {
		(*ptr)[j] = (*ptr)[j] - 'a' + 'A';
	    }
	}
    }

    if (current_mark.fb == this && buffer_y >= u && buffer_y <= d)
	load_workbuffer();

    refresh_clientarea(1);

    return 0;
}

int filebuffer::cmd_lowercase(void) {
    if (current_mark == UNKNOW_MARK) {
	display_messages(MSG_NO_MARKED_AREA);
	return 0;
    }

    int        u = minimum(current_mark.fn, current_mark.ln);
    int        d = maximum(current_mark.fn, current_mark.ln);
    int        f = 0;
    int        t = MAX_BUFFER_LEN;

    if (current_mark == BLOCK_MARK) {
	f = minimum(current_mark.fc, current_mark.lc);
	t = maximum(current_mark.fc, current_mark.lc);
    }

    linebuffer *ptr = (current_mark.fn < current_mark.ln) ?
					current_mark.first : current_mark.last;

    if (current_mark.fb == this && buffer_y >= u && buffer_y <= d)
	write_workbuffer(QE_FORCE);

    int  i, j, len;

    for (i = u; i <= d; i++, ptr = ptr->next) {
	len = ptr->getLength();
	for (j = minimum(t, len); j >= f; j--) {
	    if ((*ptr)[j] >= 'A' && (*ptr)[j] <= 'Z') {
		(*ptr)[j] = (*ptr)[j] - 'A' + 'a';
	    }
	}
    }

    if (current_mark.fb == this && buffer_y >= u && buffer_y <= d)
	load_workbuffer();

    refresh_clientarea(1);

    return 0;
}

//  int filebuffer::cmd_decodebase64(void) {
//	if (current_mark != LINE_MARK) {
//	    display_messages(MSG_NO_MARKED_AREA);
//	    return 0;
//	}
//
//	int	   u = minimum(current_mark.fn, current_mark.ln);
//	int	   d = maximum(current_mark.fn, current_mark.ln);
//	char	   *xbuf;
//
//	linebuffer *ptr = (current_mark.fn < current_mark.ln) ?
//					    current_mark.first : current_mark.last;
//
//	if (current_mark.fb == this && buffer_y >= u && buffer_y <= d)
//	    write_workbuffer(QE_FORCE);
//
//	for (int i = u; i <= d; i++, ptr = ptr->next) {
//	    xbuf = decode_base64(ptr->getString());
//	    *ptr = xbuf;
//	}
//
//	if (current_mark.fb == this && buffer_y >= u && buffer_y <= d)
//	    load_workbuffer();
//
//	refresh_clientarea(1);
//
//	return 0;
//  }

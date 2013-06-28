/*
 *  fbcmddel.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "filebuffer.h"
#include "messages.h"


int filebuffer::cmd_erase_end(void) {
    if (mode == FB_DATAAREA_MODE) {
	if (read_only) {
	    display_messages(MSG_READ_ONLY_FILE);
	} else {
	    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

	    if (buffer_x < workbuflen) {
		dirty_line = 1;
		workbuffer[(workbuflen = buffer_x)] = '\0';
		refresh_bufferline();
		// wclrtoeol(win);
	    }
	}
    } else {
	if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

	if ((unsigned int) cmdline_idx != strlen(cmdline)) {
	    cmdline[cmdline_idx] = '\0';
	    refresh_cmdarea();
	    // wclrtoeol(win);
	}
    }
    return 0;
}

int filebuffer::cmd_erase_begin(void) {
    if (mode == FB_DATAAREA_MODE) {
	if (read_only) {
	    display_messages(MSG_READ_ONLY_FILE);
	} else {
	    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

	    for (int i = 0; i < workbufidx; i++)
		workbuffer[i] = ' ';
	    dirty_line = 1;
	    refresh_bufferline();
	}
    } else {
	if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

	for (int i = 0; i < cmdline_idx; i++)
	    cmdline[i] = ' ';
	dirty_line = 1;
	refresh_cmdarea();
    }
    return 0;
}

int filebuffer::cmd_delchar(void) {
    if (mode == FB_DATAAREA_MODE) {
	if (read_only) {
	    display_messages(MSG_READ_ONLY_FILE);
	} else {
	    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

	    if (buffer_x < workbuflen) {
		dirty_line = 1;
		for (int i = buffer_x; i < workbuflen; i++)
		    workbuffer[i] = workbuffer[i+1];
		workbuffer[--workbuflen] = '\0';
		refresh_bufferline();
//	    } else {
//		wprintw(win, "[rubout]");
	    }
	}
    } else {
	int   len = strlen(cmdline);

	if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;
	if (cmdline_idx < len) {
	    for (int i = cmdline_idx; i < len; i++)
		cmdline[i] = cmdline[i+1];
	    refresh_cmdarea();
	}
    }
    return 0;
}

int filebuffer::cmd_rubout(void) {
    if (mode == FB_DATAAREA_MODE) {
	if (read_only) {
	    display_messages(MSG_READ_ONLY_FILE);
	} else {
	    if (buffer_x > 0) {
		buffer_x--;
		workbufidx--;
		cursor_x = cursor_x > 0 ? cursor_x - 1 : 0;
	    }
	}
    } else {
	if (cmdline_idx > 0) cmdline_idx--;
    }
    return cmd_delchar();
}

int filebuffer::cmd_deleteline(void) {
    if (read_only) {
	display_messages(MSG_READ_ONLY_FILE);
    } else {
	if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

	fix_all_mark_position(FIX_MARK_DELETE_LINE);

	if (total_line > 1) {
	    if (current == topline) topline = topline->next;
	    current->previous->next = current->next;
	    current->next->previous = current->previous;
	    total_line--;
	    linebuffer *ptr = current;
	    if (current->next == tail) {
		current = current->previous;
		load_workbuffer();
		cmd_bottom();
	    } else {
		current = current->next;
		load_workbuffer();
//		refresh_clientarea(0);
	    }
	    ptr->~linebuffer();
	    // newline_flag = 1;
	    dirty_buffer(1);
	    refresh_clientarea(0);
	} else {
	    // workbuffer[(workbuflen = 0)] = '\0';
	    workbuflen = 0;
	    write_workbuffer(QE_FORCE);
	    refresh_bufferline();
	}
    }
    return 0;
}

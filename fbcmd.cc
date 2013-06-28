/*
 *  fbcmd.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "filebuffer.h"
#include "colors.h"
#include "messages.h"

static int  saved_flag = 0;

int filebuffer::cmd_insert_toggle(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;
    input_mode = (input_mode + 1) % 2;
    display_status();
    return 0;
}

int filebuffer::cmd_insert_mode(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    input_mode = FB_INSERT_MODE;
    display_status();
    return 0;
}

int filebuffer::cmd_toggle_historykey(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    history_key = (history_key + 1) % 4;
    return 0;
}

int filebuffer::cmd_toggle_searchcase(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    search_case = (search_case + 1) % 2;
    return 0;
}

int filebuffer::cmd_toggle_dirsort(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    dir_sort = (dir_sort + 1) % 3;
    return 0;
}

int filebuffer::cmd_replace_mode(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    input_mode = FB_REPLACE_MODE;
    display_status();
    return 0;
}

int filebuffer::cmd_toggle_blankcompress(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    blank_compress = (blank_compress + 1) % 2;
    return 0;
}


int filebuffer::cmd_command_toggle(void) {
    return (mode == FB_DATAAREA_MODE) ? cmd_cursor_cmd() : cmd_cursor_data();
}

int filebuffer::cmd_cursor_data(void) {
    if (mode != FB_DATAAREA_MODE) load_workbuffer();
    mode = FB_DATAAREA_MODE;
    return 0;
}

int filebuffer::cmd_cursor_cmd(void) {
    if (mode != FB_COMMAND_MODE) write_workbuffer(QE_IF_NECESSARY);
    mode = FB_COMMAND_MODE;
    return 0;
}

int filebuffer::cmd_split(void) {
    if (read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }

//  if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    cmd_cursor_data();

    if (buffer_x > workbuflen-1) {
	buffer_x = workbufidx = workbuflen - 1;
	linebuffer  *nl = new linebuffer("");
	nl->next		= current->next;
	nl->previous		= current;
	current->next->previous = nl;
	current->next		= nl;
	total_line++;
	cmd_endline();
    } else {
	// wprintw(win, "[split]");
	workbuffer[workbuflen] = '\0';
	linebuffer *nl = new linebuffer(&workbuffer[workbufidx]);
	nl->next		= current->next;
	nl->previous		= current;
	current->next->previous = nl;
	current->next		= nl;
	total_line++;
	workbuffer[workbufidx] = '\0';
	workbuflen = (buffer_x = workbufidx) + 1;
    }
    write_workbuffer(QE_FORCE);
    fix_all_mark_position(FIX_MARK_INSERT_LINE);
    refresh_clientarea(0);
    load_workbuffer();
    return 0;
}

int filebuffer::cmd_join(void) {
    if (read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }

//  if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    if (current->next != tail) {
	fix_all_mark_position(FIX_MARK_JOIN_LINE);

	linebuffer  *ptr= current->next;

	workbuffer[workbuflen] = '\0';
	strncat(workbuffer, ptr->getString(), MAX_BUFFER_LEN - workbuflen - 2);
	workbuflen = strlen(workbuffer);
	current->next = ptr->next;
	ptr->next->previous = current;
	total_line--;
	ptr->~linebuffer();
	dirty_line = 1;
	write_workbuffer(QE_FORCE);
	refresh_clientarea(0);
    }
    return 0;
}

int filebuffer::cmd_new(void) {
    return cmd_edit("");
}

int filebuffer::cmd_edit(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    if (current_buffer == current_buffer->next_buffer) return 0;


    write_workbuffer(QE_IF_NECESSARY);
    current_buffer = current_buffer->next_buffer;
    if (keep_in_menu == MENU_KEEP_IN_MENU) {
	keep_in_menu = MENU_NOT_IN_MENU;
	current_buffer->keep_in_menu = MENU_BACK_TO_MENU;
    }
    return RTNVAL_NEXT_BUFFER;
}

int filebuffer::cmd_edit(const char *args, const int  ro) {
    if (current_buffer->filename.getLength() == 0 ||
		     strcmp(current_buffer->filename.getString(), args) != 0) {
	int  found = 0;

	write_workbuffer(QE_IF_NECESSARY);

	filebuffer *f = current_buffer;
	while ((f = f->next_buffer) != current_buffer) {
	    if (f->filename.getLength() != 0 &&
				 strcmp(f->filename.getString(), args) == 0) {
		found = 1;
		break;
	    }
	}
	if (! found) f = new filebuffer(args);
	f->read_only = ro;
	current_buffer = f;

	return RTNVAL_NEXT_BUFFER;
    }
    return 0;
}

int filebuffer::cmd_save_all_quit(void) {
    quit_confirm = 0;

    if (dirty_flag || dirty_line) cmd_save();
    cmd_quit();

    return RTNVAL_SAVE_ALL_AND_QUIT;
}

int filebuffer::cmd_quit_all(void) {
    quit_confirm = 0;
    cmd_quit();
    return RTNVAL_QUIT_ALL_BUFFER;
}

int filebuffer::cmd_quit(void) {

    write_workbuffer(QE_IF_NECESSARY);

    if (quit_confirm) {
	if (dirty_flag || dirty_line) {
	    int   k;

	    display_messages(MSG_REALLY_QUIT);

	    while (1) {
		k = wgetch(win);
		if (k == 'y' || k == 'Y') break;
		if (k == 'n' || k == 'N') {
		    display_messages();
		    return 0;
		}
	    }
	}
    }

    return RTNVAL_QUIT_BUFFER;
}

int filebuffer::cmd_file(void) {
    write_workbuffer(QE_IF_NECESSARY);
    saved_flag = 0;
    cmd_save();
    if (saved_flag && dirty_flag == 0) return cmd_quit();
    return 0;
}

int filebuffer::cmd_save(void) {
    if (filename.getLength() != 0) {
	FILE	     *fd;
	linebuffer   *ptr = head;
	struct stat  stbuf;
	int	     mode_changed = 0;

	write_workbuffer(QE_IF_NECESSARY);

	if (access(filename.getString(), F_OK) == 0 &&
			  access(filename.getString(), W_OK) != 0) {
	    int  k;
	    display_messages(MSG_WRITE_FORCE);
	    while (1) {
		k = wgetch(win);
		if (k == 'y' || k == 'Y') break;
		if (k == 'n' || k == 'N') {
		    display_messages();
		    return 0;
		}
	    }
	    stat(filename.getString(), &stbuf);
	    chmod(filename.getString(), stbuf.st_mode | 0222);
	    mode_changed = 1;
	}

	if ((fd = fopen(filename.getString(), "w+")) != NULL) {
	    int   i, j, k, m, len;
	    char  buffer[MAX_BUFFER_LEN+1];

	    while ((ptr = ptr->next) != tail) {
		if (blank_compress) {
		    len = ptr->getLength();
		    m	= 0;

		    for (i = TAB_SPACE; i < len; i += TAB_SPACE) {
			for (j = i - 1; j >= i - TAB_SPACE; j--)
			    if ((*ptr)[j] != ' ')
				break;

			for (k = i - TAB_SPACE; k <= j; k++)
			    buffer[m++] = (*ptr)[k];

			if (j == i-2) {
			    buffer[m++] = ' ';
			} else if (j < i-2) {
			    buffer[m++] = '\t';
			}
		    }
		    for (i = i - TAB_SPACE; i <= len; i++)
			buffer[m++] = (*ptr)[i];

		    for (m--; m >= 0; m--) {
			if (buffer[m] != ' ' && buffer[m] != '\t'
					     && buffer[m] != '\0')
			    break;
			buffer[m] = '\0';
		    }

		    fprintf(fd, "%s\n", buffer);
		} else {
		    fprintf(fd, "%s\n", ptr->getString());
		}
	    }
	    fclose(fd);
	    dirty_flag = 0;
	    display_filename();
	    // setattr(win, ATTRIBUTE_status_line);
	    // mvwprintw(win, screen_y-2, 0, "%-30s ", filename);
	    display_messages(MSG_FILE_SAVED);
	    saved_flag = 1;
	} else {
	    display_messages(MSG_ERROR_OPEN_FILE);
	}

	if (mode_changed) chmod(filename.getString(), stbuf.st_mode);
    } else {
	display_messages(MSG_MESSING_FILE_NAME);
    }
    // mode = FB_COMMAND_MODE;

    return 0;
}

int filebuffer::cmd_undo(void) {
    load_workbuffer();
    dirty_line = 0;
    refresh_bufferline();

    return 0;
}

int filebuffer::cmd_first_nonblank(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    cmd_cursor_data();

    for (workbufidx = 0; workbufidx < workbuflen; workbufidx++)
	if (workbuffer[workbufidx] != ' ')
	    break;

    buffer_x = workbufidx;

    moveto(buffer_y, buffer_x);

//  if (workbufidx < screen_x - 1) {
//	cursor_x = workbufidx;
//	if (left_col_no != 0) {
//	    left_col_no = 0;
//	    refresh_clientarea();
//	    refresh_bufferline();
//	}
//  } else {
//	if ((unsigned int) workbufidx >= left_col_no &&
//	    (unsigned int) workbufidx < left_col_no + screen_x -1) {
//	    cursor_x = workbufidx - left_col_no;
//	} else {
//	    cursor_x = screen_x - 1;
//	    left_col_no = workbufidx - cursor_x;
//	    refresh_clientarea();
//	    refresh_bufferline();
//	}
//  }
    return 0;
}

int filebuffer::cmd_insertline(void) {
    if (read_only) {
	display_messages(MSG_READ_ONLY_FILE);
	return 0;
    }

    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    write_workbuffer(QE_FORCE);

    linebuffer *newline     = new linebuffer("");
    newline->next	    = current->next;
    newline->previous	    = current;
    newline->next->previous = newline;
    current->next	    = newline;
    current		    = newline;

    buffer_x = 0;
    buffer_y++;

    cursor_x = 0;

    if (cursor_y == screen_y-4) {
	top_line_no++;
	topline = topline->next;
    } else {
	cursor_y++;
    }

    total_line++;
    fix_all_mark_position(FIX_MARK_INSERT_LINE);
    refresh_clientarea(0);
    load_workbuffer();

    return 0;
}

int filebuffer::cmd_beep(void) {
    beep();
    return 0;
}

int filebuffer::cmd_cmdquote(const char *quote) {
    cmd_execute(quote);
    return 0;
}

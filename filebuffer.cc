/*
 *  filebuffer.cc
 *
 *  Copyright (C) 1998, 1999  Jiann-Ching Liu
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "version.h"
#include "keydefine.h"
#include "filebuffer.h"
#include "history.h"
#include "misc.h"
#include "colors.h"
#include "messages.h"
#include "menu.h"

#if AUTO_TERM_SIZE_DETECT
#    include <term.h>
#endif


// const int  markstacksize = 5;
mark	    filebuffer::markstack[markstacksize];
int	    filebuffer::marksp = -1;
mark	    filebuffer::current_mark;
filebuffer* filebuffer::current_buffer = NULL;
short	    filebuffer::enable_ctrl_k  = 0;
short	    filebuffer::enable_ctrl_x  = 0;
short	    filebuffer::enable_ctrl_q  = 0;
short	    filebuffer::quit_confirm   = 1;
short	    filebuffer::history_key    = HISTORY_KEY_NONE;
short	    filebuffer::blank_compress = 0;
short	    filebuffer::search_case    = 0;
short	    filebuffer::big5msg_flag   = 0;
short	    filebuffer::dir_sort       = DIR_SORT_BY_NAME;
char	    filebuffer::current_dir[PATH_MAX] = "";
int	    filebuffer::screen_lines   = 0;
int	    filebuffer::screen_columns = 0;
keydefine*  filebuffer::kdf = NULL;
menu	    filebuffer::menubar;
cmdHistory  filebuffer::history;

// linebuffer* filebuffer::cmdhistory = NULL;
// int	       filebuffer::history_no = 0;


#ifdef USE_ANSICOLOR
int	    filebuffer::ansi_color = -1;
#endif


filebuffer::filebuffer(void) : filename() {
    init_window();
}

filebuffer::filebuffer(const char *fname) : filename(fname) {
    init_window();
    // load_file();
}

void filebuffer::load_file(void) {
    FILE	  *fd;
    unsigned int  j, len, tmplen;
    char	  tmpbuf[MAX_BUFFER_LEN+1];
    char	  linebuf[MAX_BUFFER_LEN+1];

    if (loadingOK) return;


    if (filename.getLength() != 0) {

	if (access(filename.getString(), F_OK) == 0) {
	    if (access(filename.getString(), R_OK) == 0) {
		if ((fd = fopen(filename.getString(), "r")) != NULL) {
		    linebuffer	*tmpptr, *ptr = current;

		    total_line = 0;

		    while (fgets(tmpbuf, MAX_BUFFER_LEN, fd) != NULL) {
			if (total_line % 100 == 0) {
			    display_messages("Loading %d ...", total_line);
			}
			tmplen = strlen(tmpbuf);
			for (unsigned int i = j = len = 0;
			       (i < tmplen) && (j < MAX_BUFFER_LEN); i++) {
			    switch(tmpbuf[i]) {
			    case '\r':
			    case '\n':
				linebuf[j++] = '\0';
				break;
			    case '\t':
				for (int k = j % TAB_SPACE; k < TAB_SPACE;
								      k++, j++)
				    linebuf[j] = ' ';
				break;
			    case ' ' :
				linebuf[j++] = ' ';
				break;
//			    case 127 :
//				linebuf[j++] = '^';
//				linebuf[j++] = '?';
//				len = j;
//				break;
			    case '\b':
			    case '\f':
				break;
			    default  :
				linebuf[j++] = tmpbuf[i];
				len = j;
				break;
			    }
			}
			linebuf[len] = '\0';

			tmpptr = new linebuffer(linebuf);
			ptr->next	 = tmpptr;
			tmpptr->previous = ptr;
			ptr		 = tmpptr;
			total_line++;
		    }
		    ptr->next	      = tail;
		    tail->previous    = ptr;

		    if (ptr == current) { // empty input file
			head->next	  = current;
			current->previous = head;
			total_line	  = 1;
		    } else {
			head->next	  = current->next;
			ptr		  = current;
			current 	  = current->next;
			current->previous = head;
			delete ptr;
		    }
		    fclose(fd);

		    if (access(filename.getString(), W_OK) == 0) {
			display_messages("Loading file: %s .... ok.",
					       basename(filename.getString()));
		    } else {
			display_messages("Reading file: %s .... ok.",
					       basename(filename.getString()));
		    }
		} else {
		    display_messages(MSG_NEW_FILE);
		}
	    } else {
		display_messages(MSG_READ_ERROR);
	    }
	} else { // new file
	    display_messages(MSG_NEW_FILE);
	}

	refresh_clientarea(0);
	display_status();
    }
    loadingOK = 1;
}


void filebuffer::init_window(void) {
#ifdef USE_ANSICOLOR
    if (ansi_color == -1) ansi_color = has_colors() ? 0 : 1;
#endif

    if (current_dir[0] == '\0') getcwd(current_dir, PATH_MAX);

    if (current_buffer == NULL) {
	current_buffer	= this;
	next_buffer	= this;
	previous_buffer = this;

	qeString::setHandler(filebuffer::out_of_memory);

	init_menu();
    } else {
	next_buffer		     = current_buffer->next_buffer;
	previous_buffer 	     = current_buffer;
	current_buffer->next_buffer  = this;
	next_buffer->previous_buffer = this;
    }

    head    = new linebuffer("=== Top of file ===");
    tail    = new linebuffer("=== Bottom of file ===");
    current = new linebuffer("");
    head->previous     = NULL;
    head->next	       = current;
    current->previous  = head;
    current->next      = tail;
    tail->previous     = current;
    tail->next	       = NULL;
    buffer_type        = NORMAL_TEXT_BUFFER;

    screen_x  = screen_columns;
    screen_y  = screen_lines;

    read_only	 = 0;
    loadingOK	 = 0;
    win 	 = NULL;
    keep_in_menu = MENU_NOT_IN_MENU;

    sprintf(outputformat, "%%-%ds", screen_x);

    general_init();
}

void filebuffer::general_init(void) {
    cursor_x = 0;
    cursor_y = 1;
    buffer_x = 0;
    buffer_y = 0;

    // keycommand = NULL;

    topline	       = head;
    total_line	       =  1;
    top_line_no        = -1;
    left_col_no        =  0;

    dirty_flag	       = 0;
    dirty_line	       = 0;
    quiet_flag	       = 0;
    message_dirty_flag = 0;

    mode	       = FB_DATAAREA_MODE;
    input_mode	       = FB_REPLACE_MODE;

    cmdline[(cmdline_idx = 0)] = '\0';
}

filebuffer::~filebuffer(void) {
//  FILE  *fd;
//
//  fd = fopen("log.txt", "a+");
//  fprintf(fd, "quit: %s\n", filename);
//  fclose(fd);

    // if (filename != NULL) qeFree(filename);
    filename.~qeString();
    if (win != NULL) delwin(win);

    for (linebuffer *ptr = head; ptr != NULL; ptr = current) {
	current = ptr->next;
	ptr->~linebuffer();
	delete ptr;   // version 0.0.23
    }

    // filename = NULL;
    win = NULL;
}

void filebuffer::out_of_memory(void) {
    if (current_buffer != NULL) {
	current_buffer->read_only = 1;
	current_buffer->display_messages(MSG_OUT_OF_MEMORY);
    }
}

void filebuffer::init_curses(void) {

#if AUTO_TERM_SIZE_DETECT
    if (screen_columns == 0) {
	char   entrybuf[1024];
	char   *term = getenv("TERM");
	if (term != NULL && tgetent(entrybuf, term) != -1) {   // read in entry
	    screen_columns = tgetnum("co");
	    screen_lines   = tgetnum("li");
	} else {
	    screen_columns = 80;
	    screen_lines   = 24;
//	    screen_columns = COLUMNS;
//	    screen_lines   = LINES;
	}
    }
#else
    screen_columns = 80;
    screen_lines   = 24;
#endif

    initscr();		   // initialize the curses library
    keypad(stdscr, TRUE);  // enable keyboard mapping
    nonl();		   // tell curses not to do NL->CR/NL on output
    cbreak();		   // take input chars one at a time, no wait for \n
    nocrmode(); 	   // Control input mode: ^S, ^Q, ^C, ^Y go to kernel
    noecho();		   // don't echo input
    color_setup();
    raw();		   // Raw input mode no processing by the kernel.
    intrflush(stdscr, FALSE);
    nodelay(stdscr, FALSE);
    refresh();
}

void filebuffer::end_curses(void) {
    endwin();
}


void filebuffer::init_keydefine(const char *fname) {
    kdf = new keydefine(fname);
}

void filebuffer::init_keydefine(void) {
    kdf = new keydefine();
}

// char* filebuffer::qeStrdup(const char *s) {
//     char  *ptr;
//     int   len = strlen(s) + 1;
//
//     if ((ptr = new char[len]) != NULL) {
//	   memcpy(ptr, s, len);
//     }
//     return ptr;
// }

filebuffer* filebuffer::active(void) {
    int 	finish = 0;
    static int	quit_all = 0;
    filebuffer	*fb;

    if (win == NULL) win = newwin(screen_y, screen_x, 0, 0);
    wclear(win);
    keypad(win, TRUE);
    nodelay(win, FALSE);

    paint();
    load_file();
    load_workbuffer();

    switch (quit_all) {
    case 1:
	finish = cmd_quit_all();
	break;
    case 2:
	finish = cmd_save_all_quit();
	break;
    default:
	if (keep_in_menu == MENU_BACK_TO_MENU) finish = cmd_menu();
	break;
    }

    while (finish == 0)
	finish = (mode == FB_COMMAND_MODE) ? cmdarea() : clientarea();

    if (win != NULL) delwin(win);
    win = NULL;

    quit_all = 0;

    switch (finish) {
    case RTNVAL_SHELL_ESCAPE:
	end_curses();
	system(getenv("SHELL"));
	init_curses();
	break;
    case RTNVAL_QUIT_BUFFER:
	unmark_all_mark();
	if ((current_buffer = this->previous_buffer) == this) {
	    current_buffer = NULL;
	    display_messages(MSG_QUIT);
	} else {
	    this->next_buffer->previous_buffer = current_buffer;
	    current_buffer->next_buffer        = this->next_buffer;
	}
	this->~filebuffer();

	break;
    case RTNVAL_SAVE_ALL_AND_QUIT:
	quit_all = 1;
    case RTNVAL_QUIT_ALL_BUFFER:
	quit_all++;
    case RTNVAL_QUIT_CURRENT_BUFFER:
	current_buffer->unmark_all_mark();
	fb = current_buffer;
	current_buffer = current_buffer->next_buffer;
	if (fb == current_buffer) {
	    current_buffer = NULL;
	} else {
	    current_buffer->previous_buffer = fb->previous_buffer;
	    fb->previous_buffer->next_buffer = current_buffer;
	}

	fb->~filebuffer();
	break;
    }

    return current_buffer;
}

void filebuffer::load_workbuffer(void) {
    strncpy(workbuffer, current->getString(), MAX_BUFFER_LEN);
    workbuflen = strlen(workbuffer);
    for (int i = strlen(workbuffer); i < MAX_BUFFER_LEN; i++)
	workbuffer[i] = ' ';
    workbufidx = buffer_x;
}

void filebuffer::write_workbuffer(const int  force) {
    if (force || dirty_line) {
	dirty_buffer(1);
	workbuffer[workbuflen] = '\0';
	// current->assign(workbuffer);
	*current = workbuffer;
	for (int i = workbuflen; i < MAX_BUFFER_LEN; i++)
	    workbuffer[i] = ' ';
	// workbuffer[workbuflen] = ' ';
    }
    dirty_line = 0;
}

int filebuffer::dirty_buffer(const int force) {
    if (! dirty_flag && (force || dirty_line)) {
	dirty_flag = 1;
	display_filename();
	return 1;
    }
    return 0;
}

int filebuffer::cmd_kbinput(void) {
    int  key;

    if (mode == FB_DATAAREA_MODE) {
	// setattr(win, ATTRIBUTE_normal_text);
	ca_attrset();
	wmove(win, cursor_y, cursor_x);
//	load_workbuffer(QE_IF_NECESSARY);
//	if (newline_flag) {
//	    load_workbuffer();
//	    newline_flag = 0;
//	}

	if (((key = getfirstkey()) >= ' ') && (key < 256)) {
	    ca_attrset();
	    wmove(win, cursor_y, cursor_x);
	    if (read_only) {
		display_messages(MSG_READ_ONLY_FILE);
	    } else {
		if (workbufidx >= MAX_BUFFER_LEN-2) return 0;

		if (input_mode == FB_REPLACE_MODE) {
		    mvwaddch(win, cursor_y, cursor_x, key);
		    workbuffer[workbufidx++] = key;
		    workbuflen = maximum(workbuflen, workbufidx);
		} else {
		    mvwinsch(win, cursor_y, cursor_x, key);
		    for (int i = minimum(workbuflen, MAX_BUFFER_LEN-2);
							  i > workbufidx; i--)
			workbuffer[i] = workbuffer[i-1];
		    workbuffer[workbufidx++] = key;
		    workbuflen=workbuflen>=workbufidx?workbuflen+1:workbufidx;
		    workbuflen = minimum(workbuflen, MAX_BUFFER_LEN-2);
		}
		dirty_line = 1;
		buffer_x = workbufidx;
		if (cursor_x < screen_x-1) {
		    cursor_x++;
		} else {
		    left_col_no++;
		    refresh_clientarea(1);
		}
		display_status();
	    }
	} else {
	    if (buffer_type == DIRECTORY_BUFFER) {
		if (key == KEY_ENTER || key == KEY_RETURN
				     || key == KEY_LINEFEED) {
		    // load_file();
		}
	    }
	    // if ((key = getnextkey()) != 0) return key;
	    return do_function_key(key);
	    // return getnextkey();
	}
    } else { /////////////// Command Line ////////////////////////
	setattr(win, ATTRIBUTE_command_line);
	wmove(win, screen_y-3, cmdline_idx < screen_x - 1 ? cmdline_idx :
			       screen_x -1);

	int  cmdlen = strlen(cmdline);

	if (((key = getfirstkey()) >= ' ') && (key < 256)) {
	    setattr(win, ATTRIBUTE_command_line);
	    wmove(win, screen_y-3, cmdline_idx < screen_x - 1 ? cmdline_idx :
			       screen_x -1);
	    if (cmdline_idx < MAX_BUFFER_LEN - 2) {
		if (input_mode == FB_REPLACE_MODE) {
		    cmdline[cmdline_idx++] = key;
		    cmdlen = maximum(cmdlen, cmdline_idx);
		    cmdline[cmdlen] = '\0';
		    if (cmdline_idx < screen_x-1) {
			waddch(win, key);
		    } else {
			refresh_cmdarea();
		    }
		} else {
		    winsch(win, key);
		    for (int i = cmdlen; i > cmdline_idx; i--)
			cmdline[i] = cmdline[i-1];
		    cmdline[cmdline_idx++] = key;
		    cmdlen = minimum(cmdlen + 1, MAX_BUFFER_LEN -2);
		    cmdline[cmdlen] = '\0';

		    // mvwinsch(win, cursor_y, cursor_x, key);
		    // for (int i = workbuflen; i > workbufidx; i--)
		    //	   workbuffer[i] = workbuffer[i-1];
		    // workbuffer[workbufidx++] = key;
		    // workbuflen=workbuflen>=workbufidx?workbuflen+1:workbufidx;
		    // workbuflen = minimum(workbuflen, MAX_BUFFER_LEN);
		}
	    } else {
		beep();
	    }
	} else {
	    switch (key) {
//	    case KEY_DOWN:
//		display_messages(MSG_CMD_HISTORY);
//		history.push(cmdline);
//		if (history.active(cmdline, win, screen_y, screen_x, 10, 40)) {
//		    refresh_cmdarea();
//		    cmdline_idx = 0;
//		}
//		history.pop();
//		display_messages();
//		break;
	    case KEY_RETURN:
	    case KEY_ENTER:
	    case KEY_LINEFEED:
		if (cmdlen > 0) {
		    history.push(cmdline);
		    return cmd_execute();
		}
		break;
	    default:
		// if ((key = getnextkey()) != 0) return key;
		// return getnextkey();
		if (((key == KEY_UP)   && (history_key & HISTORY_KEY_UP  )) ||
		    ((key == KEY_DOWN) && (history_key & HISTORY_KEY_DOWN))) {
		    return cmd_history();
		} else {
		    return do_function_key(key);
		}
		// waddch(win, key);
		// wprintw(win, "(%d)", key);
		break;
	    }
	}
    }
    return 0;
}

int filebuffer::cmd_history(void) {
    write_workbuffer(QE_IF_NECESSARY);
    cmd_cursor_cmd();

    display_messages(MSG_CMD_HISTORY);
    history.push(cmdline);

    if (history.active(cmdline, win, screen_y, screen_x, 10, 40)) {
	refresh_cmdarea();
	cmdline_idx = 0;
    }

    history.pop();

    display_messages();
    return 0;
}

int filebuffer::do_function_key(const int key) {
    return getnextkey();
}

int filebuffer::clientarea(void) {
    // int	modified = 0;
    // keycmdlist  *keycommand;
    int rtlval = 0;

    // newline_flag = 1;

    while (mode == FB_DATAAREA_MODE) {
	if ((rtlval = cmd_kbinput()) != 0) return rtlval;
    }
    return rtlval;
}

void filebuffer::display_status(void) {
    setattr(win, ATTRIBUTE_status_line);
    mvwprintw(win, screen_y-2, screen_x-26, "%6d/%-6d %3d  %7s",
	      buffer_y + 1, total_line,
	      mode == FB_DATAAREA_MODE ? buffer_x + 1 : cmdline_idx + 1,
	      (big5msg_flag == 1) ?
	      (input_mode == FB_INSERT_MODE  ? "<I>´¡¤J" : "<R>¨ú¥N") :
	      (input_mode == FB_INSERT_MODE  ? " Insert" : "Replace"));

}

void filebuffer::paint(void) {
    for (int i = 0; i < screen_y-3; i++) {
	setattr(win, ATTRIBUTE_normal_text);
	mvprint(i, 0, "");
    }
    setattr(win, ATTRIBUTE_command_line);
    mvprint(screen_y-3, 0, "");
    setattr(win, ATTRIBUTE_status_line);
    mvprint(screen_y-2, 0, "");
    setattr(win, ATTRIBUTE_message_line);
    mvprint(screen_y-1, 0, "");

    display_filename();
    display_status();
    refresh_clientarea(0);
    display_messages();
    wmove(win, 0, 0);
    setattr(win, ATTRIBUTE_normal_text);
    touchwin(win);
    wrefresh(win);
}


void filebuffer::display_filename(void) {

    if (filename.getLength() == 0 || current_buffer != this) return;

    char dispbuff[MAX_BUFFER_LEN];
    int  len = filename.getLength();
    int  idx = 0;

//  dispbuff[0] = '\0';
//
//  if (read_only) {
//	strcpy(dispbuff, "%% ");
//  } else if (dirty_flag) {
//	strcpy(dispbuff, "** ");
//  } else {
//	strcpy(dispbuff, "-- ");
//  }
//  idx = strlen(dispbuff);

    if (len > screen_x - 35) {
	dispbuff[idx] = filename[0] == '[' ? '[' : '.';
//	sprintf(&dispbuff[idx+1], ".. %s", &filename[len + 35 - screen_x]);
	sprintf(&dispbuff[idx+1], ".. %s",
				    filename.getString(len + 35 - screen_x));
    } else {
	strcpy(&dispbuff[idx], filename.getString());
    }

    setattr(win, dirty_flag ? ATTRIBUTE_modified_file_line : ATTRIBUTE_status_line);
    mvwprintw(win, screen_y-2, 0, "%s", dispbuff);
}



void filebuffer::refresh_bufferline(void) {
    int 	  mlnos = -2, mlnoe = -2;
    unsigned int  bml = MAX_BUFFER_LEN + 1;
    unsigned int  bmr = MAX_BUFFER_LEN + 1;
    int 	  bmfix = 0;

    if (quiet_flag || current_buffer != this) return;

    if (current_mark != UNKNOW_MARK && current_mark.fb == this) {
	mlnos = minimum(current_mark.fn, current_mark.ln);
	mlnoe = maximum(current_mark.fn, current_mark.ln);

	if (current_mark == BLOCK_MARK) {
	    bml = minimum(current_mark.fc, current_mark.lc);
	    bmr = maximum(current_mark.fc, current_mark.lc);
	}
    }

    if (buffer_y >= mlnos && buffer_y <= mlnoe) {
	if (current_mark == BLOCK_MARK) {
	    if (bmr >= left_col_no && bml <= left_col_no + screen_x)
		bmfix = 1;
	    setattr(win, ATTRIBUTE_normal_text);
	} else {
	    setattr(win, ATTRIBUTE_mark_text);
	}
    } else {
	setattr(win, ATTRIBUTE_normal_text);
    }

    mvprint(cursor_y, 0,
     ((unsigned int)workbuflen > left_col_no) ?  &workbuffer[left_col_no] : "");

    if (bmfix) {
	setattr(win, ATTRIBUTE_mark_text);
	int   leftmax = maximum(bml, left_col_no);
/*
	mvprint(cursor_y, (bml > left_col_no ? bml - left_col_no : 0),
		   minimum(minimum(bmr+1, screen_x + left_col_no) -
			   maximum(bml, left_col_no),
			   (unsigned int) screen_x),
		   ((unsigned int) workbuflen > bml) ?	&workbuffer[bml] : "");
*/
	mvprint(cursor_y, leftmax - left_col_no,
	    minimum(minimum(bmr+1, screen_x+left_col_no) - leftmax,
			  (unsigned int) screen_x),
	    (workbuflen > leftmax) ? &workbuffer[leftmax] : "");
    }
    setattr(win, ATTRIBUTE_normal_text);
}

void filebuffer::refresh_clientarea(const int bufflag) {
    int 	  i, mlnos = -2, mlnoe = -2, lno = top_line_no;
    int 	  blockmark_need_fix = 0;
    unsigned int  bml = MAX_BUFFER_LEN + 1;
    unsigned int  bmr = MAX_BUFFER_LEN + 1;
    linebuffer	  *ptr = topline;

    if (quiet_flag || current_buffer != this) return;

    if (current_mark != UNKNOW_MARK && current_mark.fb == this) {
	mlnos = minimum(current_mark.fn, current_mark.ln);
	mlnoe = maximum(current_mark.fn, current_mark.ln);

	if (current_mark == BLOCK_MARK) {
	    bml = minimum(current_mark.fc, current_mark.lc);
	    bmr = maximum(current_mark.fc, current_mark.lc);
	}
    }

    setattr(win, ATTRIBUTE_normal_text);

    for (i = 0; i < screen_y-3 && ptr != NULL; i++, ptr = ptr->next, lno++) {
	if (lno >= mlnos && lno <= mlnoe) {
	    if (current_mark == BLOCK_MARK) {
		if (bmr >= left_col_no && bml <= left_col_no + screen_x)
		    blockmark_need_fix = 1;
		setattr(win, ATTRIBUTE_normal_text);
	    } else {
		setattr(win, ATTRIBUTE_mark_text);
	    }
	} else {
	    setattr(win, ATTRIBUTE_normal_text);
	}

	if (ptr == head || ptr == tail) {
	    mvprint(i, 0, ptr->getString());
	} else {
	    mvprint(i, 0, (ptr->getLength() > left_col_no) ?
			   ptr->getString(left_col_no) : "");
//			   &((*ptr)[left_col_no]) : "");
	}

	if (blockmark_need_fix) {
	    blockmark_need_fix = 0;
	    setattr(win, ATTRIBUTE_mark_text);
	    unsigned int   leftmax = maximum(bml, left_col_no);
	    mvprint(i, leftmax - left_col_no,
		  minimum(minimum(bmr+1, screen_x+left_col_no) - leftmax,
			  (unsigned int) screen_x),
		  (ptr->getLength() > leftmax ? ptr->getString(leftmax) : ""));
//		  (ptr->getLength() > leftmax ? &((*ptr)[leftmax]) : ""));
	}
    }
    setattr(win, ATTRIBUTE_normal_text);

    for (; i < screen_y-3; i++) mvprint(i, 0, "");

    if (bufflag) refresh_bufferline();
}

void filebuffer::mvprint(const int y, const int x, const char *str) {
    char buffer[MAX_BUFFER_LEN+1];

    if (current_buffer != this) return;

    sprintf(buffer, outputformat, str);
    mvwaddnstr(win, y, x, buffer, screen_x);
}

void filebuffer::mvprint(const int y, const int x, const int len,
						   const char *str) {
    char buffer[MAX_BUFFER_LEN+1];
    char fmt[20];

    if (current_buffer != this) return;
    sprintf(fmt, "%%-%ds", len);
    sprintf(buffer, fmt, str);
    mvwaddnstr(win, y, x, buffer, len);
}

void filebuffer::display_messages(const char *fmt, ...) {
    // char	msg[MAX_BUFFER_LEN+1];
    char     buffer[MAX_BUFFER_LEN+1];
    va_list  ap;
    // attr_t	attr = wattr_get(win);

    if (current_buffer != this) return;

    message_dirty_flag = 1;

    va_start(ap, fmt);
#ifndef HAS_VSNPRINTF
    vsprintf(buffer, fmt, ap);
#else
    vsnprintf(buffer, 80, fmt, ap);
#endif
    va_end(ap);
    // sprintf(msg, outputformat, buffer);
    setattr(win, ATTRIBUTE_message_line);
    mvwprintw(win, screen_y-1, 0, outputformat, "");
    mvwaddnstr(win, screen_y-1, 0, buffer, screen_x);
    // setattr(win, attr);
    touchwin(win);
    wrefresh(win);
}

void filebuffer::display_messages(const int msgno) {
    display_messages(big5msg_flag ? chinese_message[msgno]
				  : english_message[msgno]);
}

void filebuffer::display_messages(void) {
    static char *buffer =
    "  qe v" QE_VERSION
    " (c) written by  Jiann-Ching Liu,	 !!! ABSOLUTELY NO WARRANTY !!!";

    if (current_buffer != this) return;

    message_dirty_flag = 0;

    // attr_t	attr = wattr_get(win);

    setattr(win, ATTRIBUTE_copyright_line);
    mvwaddnstr(win, screen_y-1, 0, buffer, screen_x);
    // setattr(win, attr);

    touchwin(win);
    wrefresh(win);
}

int filebuffer::is_big5(void) {
    return big5msg_flag == 0 ? 0 : 1;
}


int filebuffer::cmd_lang_big5(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    big5msg_flag = 1;
    display_status();
    return 0;
}

int filebuffer::cmd_lang_english(void) {
    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_KEEP_IN_MENU;

    big5msg_flag = 0;
    display_status();
    return 0;
}

#ifdef USE_ANSICOLOR

void filebuffer::setattr(WINDOW *w, int color) {
    if (ansi_color) {
	switch(color) {
	case ATTRIBUTE_normal_text:
	    printf("%c[37m%c[44m", 27, 27);
	    break;
	case ATTRIBUTE_mark_text:
	    printf("%c[37m%c[41m", 27, 27);
	    break;
	case ATTRIBUTE_command_line:
	    printf("%c[33m%c[42m", 27, 27);
	    break;
	case ATTRIBUTE_status_line:
	    printf("%c[37m%c[40m", 27, 27);
	    break;
	case ATTRIBUTE_message_line:
	    printf("%c[32m%c[40m", 27, 27);
	    break;
	case ATTRIBUTE_modified_file_line:
	    printf("%c[32m%c[40m", 27, 27);
	    break;
	case ATTRIBUTE_copyright_line:
	    printf("%c[33m%c[40m", 27, 27);
	    break;
	}
    } else {
	wattrset(w, attributes[color]);
    }
}

#endif

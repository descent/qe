/*
 *  about.cc
 *
 *  Copyright (C) 1998, 1999  Jiann-Ching Liu
 */


#include "config.h"
#include "version.h"
#include "filebuffer.h"
#include "colors.h"
#include "keymap.h"

int filebuffer::cmd_about(void) {
    WINDOW *abwin;

    abwin = newwin(9, 59, 6, 10);

    setattr(abwin, ATTRIBUTE_about_box_border);
//  if (filebuffer::is_big5()) {
//	box(abwin, '|', '-');
//  } else {
//	box(abwin, 0, 0);
//  }

    box(abwin, 0, 0);

    setattr(abwin, ATTRIBUTE_about_box_text);
    mvwprintw(abwin, 1,1," %-55s ", "");
    mvwprintw(abwin, 2,1," %-55s ", "QE comes with ABSOLUTELY NO WARRANTY !!");
    mvwprintw(abwin, 3,1," %-55s ", "");
    mvwprintw(abwin, 4,1," %-55s ", "Copyright (c) 1998, 1999 written by  Jiann-Ching Liu");
    mvwprintw(abwin, 5,1," %-55s ", "URL:  http://www.cc.ncu.edu.tw/~center5/product/qe/");
    mvwprintw(abwin, 6,1," %-55s ", "Email: center5@cc.ncu.edu.tw");
    mvwprintw(abwin, 7,1," %-55s ", "");
    wmove(abwin, 7,57);

    touchwin(abwin);
    wrefresh(abwin);

    while (1) {
	switch(wgetch(win)) {
	case ' ':
	case KEY_RETURN:
	case KEY_ENTER:
	case KEY_LINEFEED:
	    delwin(abwin);
	    touchwin(win);
	    wrefresh(win);
	    return 1;
	}
    }
    return 0;
}

int filebuffer::cmd_help(void) {
    return cmd_edit("/usr/local/etc/qe.hlp", 1);
}

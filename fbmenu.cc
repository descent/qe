/*
 *  fbmenu.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "filebuffer.h"
#include "colors.h"
#include "messages.h"
#include "menu.h"


menu  e_menubar;
menu  c_menubar;

void filebuffer::init_menu(void) {
    e_menubar.add_menu("File");    //  c_menubar.add_menu("F)�ɮ�");
    e_menubar.add_menu("Edit");    //  c_menubar.add_menu("E)�s��");
    e_menubar.add_menu("Mark");    //  c_menubar.add_menu("M)�аO");
    e_menubar.add_menu("Search");  //  c_menubar.add_menu("S)�j�M");
    e_menubar.add_menu("Goto");    //  c_menubar.add_menu("G)����");
//  e_menubar.add_menu("Option");  //  c_menubar.add_menu("G)�ﶵ");
    e_menubar.add_menu("Window");  //  c_menubar.add_menu("W)����");
    e_menubar.add_menu("Help");    //  c_menubar.add_menu("H)���U");

    e_menubar.add_item("File", "&Dir (Load file)", &filebuffer::cmd_dir);
    e_menubar.add_item("File", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("File", "&New file",	  &filebuffer::cmd_new);
    e_menubar.add_item("File", "&Save file",	   &filebuffer::cmd_save);
    e_menubar.add_item("File", "&Quit file (Close file)",	&filebuffer::cmd_quit);
    e_menubar.add_item("File", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("File", "&Blankcompress:", &filebuffer::cmd_toggle_blankcompress, &filebuffer::menu_blankcompress);
    e_menubar.add_item("File", "Dir so&rt by: ",  &filebuffer::cmd_toggle_dirsort, &filebuffer::menu_dirsort);
    e_menubar.add_item("File", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("File", "S&hell ...",	   &filebuffer::cmd_shell);
    e_menubar.add_item("File", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("File", "&Exit QE (save all and quit)", &filebuffer::cmd_save_all_quit);
    e_menubar.add_item("File", "E&xit QE (quit all without save)", &filebuffer::cmd_quit_all);

    int  i = c_menubar.add_menu("F)�ɮ�");
    c_menubar.add_item(i, "&D)�ؿ��BŪ��", &filebuffer::cmd_dir);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&N)�s��",	   &filebuffer::cmd_new);
    c_menubar.add_item(i, "&S)�s��",	   &filebuffer::cmd_save);
    c_menubar.add_item(i, "&Q)����",	   &filebuffer::cmd_quit);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&B)�s�ɮɪť����� TAB:",   &filebuffer::cmd_toggle_blankcompress, &filebuffer::menu_blankcompress);
    c_menubar.add_item(i, "&R)DIR �ɱƧǤ覡: ",  &filebuffer::cmd_toggle_dirsort, &filebuffer::menu_dirsort);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&H)���� Shell ...",	     &filebuffer::cmd_shell);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&E)���X QE (�����s��)", &filebuffer::cmd_save_all_quit);
    c_menubar.add_item(i, "&X)���X QE (���s��)", &filebuffer::cmd_quit_all);


    e_menubar.add_item("Edit", "&Edit", &filebuffer::cmd_cursor_data);
    e_menubar.add_item("Edit", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Edit", "Insert Line", &filebuffer::cmd_insertline);
    e_menubar.add_item("Edit", "Delete Line", &filebuffer::cmd_deleteline);
    e_menubar.add_item("Edit", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Edit", "Erase Begin Line", &filebuffer::cmd_erase_begin);
    e_menubar.add_item("Edit", "Erase End Line", &filebuffer::cmd_erase_end);
    e_menubar.add_item("Edit", "Delete Character", &filebuffer::cmd_delchar);
    e_menubar.add_item("Edit", "Rubout Character", &filebuffer::cmd_rubout);
    e_menubar.add_item("Edit", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Edit", "Split", &filebuffer::cmd_split);
    e_menubar.add_item("Edit", "Join",	&filebuffer::cmd_join);

    i = c_menubar.add_menu("E)�s��");

    c_menubar.add_item(i, "&E)�s��", &filebuffer::cmd_cursor_data);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)���J�@��", &filebuffer::cmd_insertline);
    c_menubar.add_item(i, "?)�R������", &filebuffer::cmd_deleteline);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)�R����Х��䪺�r", &filebuffer::cmd_erase_begin);
    c_menubar.add_item(i, "?)�R����Хk�䪺�r", &filebuffer::cmd_erase_end);
    c_menubar.add_item(i, "?)�R���@�Ӧr��", &filebuffer::cmd_delchar);
    c_menubar.add_item(i, "?)�˰h�@�Ӥl��", &filebuffer::cmd_rubout);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)����", &filebuffer::cmd_split);
    c_menubar.add_item(i, "?)����U�@��", &filebuffer::cmd_join);

    e_menubar.add_item("Mark", "Mark &Line",   &filebuffer::cmd_markline);
    e_menubar.add_item("Mark", "Mark &Block",  &filebuffer::cmd_markblock);
    e_menubar.add_item("Mark", "&Un mark",     &filebuffer::cmd_unmark);
    e_menubar.add_item("Mark", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Mark", "&Copy Mark",   &filebuffer::cmd_copymark);
    e_menubar.add_item("Mark", "O&verlay Block", &filebuffer::cmd_overlayblock);
    e_menubar.add_item("Mark", "&Move Mark",   &filebuffer::cmd_movemark);
    e_menubar.add_item("Mark", "&Delete Mark", &filebuffer::cmd_deletemark);
    e_menubar.add_item("Mark", "&Push mark",   &filebuffer::cmd_pushmark);
    e_menubar.add_item("Mark", "P&op mark",    &filebuffer::cmd_popmark);
    e_menubar.add_item("Mark", "Clear mar&k",  &filebuffer::cmd_clear_marks);
    e_menubar.add_item("Mark", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Mark", "&Shift Left",  &filebuffer::cmd_shiftleft);
    e_menubar.add_item("Mark", "Shift &Right", &filebuffer::cmd_shiftright);
    e_menubar.add_item("Mark", "&Fill mark",   &filebuffer::cmd_fillmark);
    e_menubar.add_item("Mark", "Uppercase",   &filebuffer::cmd_uppercase);
    e_menubar.add_item("Mark", "Lowercase",   &filebuffer::cmd_lowercase);

    i = c_menubar.add_menu("M)�аO");
    c_menubar.add_item(i, "&L)�榡�аO",   &filebuffer::cmd_markline);
    c_menubar.add_item(i, "&B)�϶����аO", &filebuffer::cmd_markblock);
    c_menubar.add_item(i, "&U)�����аO",   &filebuffer::cmd_unmark);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&C)�ƻs�аO��",   &filebuffer::cmd_copymark);
    c_menubar.add_item(i, "&V)�ƻ\\�аO��", &filebuffer::cmd_overlayblock);
    c_menubar.add_item(i, "&M)�h���аO��",   &filebuffer::cmd_movemark);
    c_menubar.add_item(i, "&D)�R���аO��", &filebuffer::cmd_deletemark);
    c_menubar.add_item(i, "&P)��i���|",   &filebuffer::cmd_pushmark);
    c_menubar.add_item(i, "&O)�q���|�����X",	&filebuffer::cmd_popmark);
    c_menubar.add_item(i, "&K)�M�����|", &filebuffer::cmd_clear_marks);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&S)�V�� Shift", &filebuffer::cmd_shiftleft);
    c_menubar.add_item(i, "&R)�V�k Shift", &filebuffer::cmd_shiftright);
    c_menubar.add_item(i, "&F)��J�аO��",   &filebuffer::cmd_fillmark);
    c_menubar.add_item(i, "?)�ন�j�g",   &filebuffer::cmd_uppercase);
    c_menubar.add_item(i, "?)�ন�p�g",   &filebuffer::cmd_lowercase);

    e_menubar.add_item("Search", "&Find ...",  &filebuffer::cmd_search);
    e_menubar.add_item("Search", "",	 (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Search", "&Search case: ", &filebuffer::cmd_toggle_searchcase, &filebuffer::menu_searchcase);

    i = c_menubar.add_menu("S)�j�M");
    c_menubar.add_item(i, "&F)�M�� ...",  &filebuffer::cmd_search);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&S)�j�M�ɤj�p�g: ", &filebuffer::cmd_toggle_searchcase, &filebuffer::menu_searchcase);

    e_menubar.add_item("Goto", "Cursor &Command",  &filebuffer::cmd_cursor_cmd);
    e_menubar.add_item("Goto", "Cursor &Data",	&filebuffer::cmd_cursor_data);
    e_menubar.add_item("Goto", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Goto", "&Goto ....",  &filebuffer::cmd_gotoline);
    e_menubar.add_item("Goto", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Goto", "&Begin of file",  &filebuffer::cmd_top);
    e_menubar.add_item("Goto", "&End of file",	  &filebuffer::cmd_bottom);
    e_menubar.add_item("Goto", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Goto", "&Top of screen",  &filebuffer::cmd_top_edge);
    e_menubar.add_item("Goto", "B&ottom of screen",  &filebuffer::cmd_bottom_edge);
    e_menubar.add_item("Goto", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Goto", "Be&gin of mark",  &filebuffer::cmd_beginmark);
    e_menubar.add_item("Goto", "End of &mark",	&filebuffer::cmd_endmark);
    e_menubar.add_item("Goto", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Goto", "Begin of line",  &filebuffer::cmd_beginline);
    e_menubar.add_item("Goto", "End of line",  &filebuffer::cmd_endline);

    i = c_menubar.add_menu("G)����");
    c_menubar.add_item(i, "&C)��и��ܩR�O�C",	&filebuffer::cmd_cursor_cmd);
    c_menubar.add_item(i, "&D)��и��ܸ�ư�",	&filebuffer::cmd_cursor_data);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&B)�ɮ׶}�Y",  &filebuffer::cmd_top);
    c_menubar.add_item(i, "&E)�ɮ׵���",    &filebuffer::cmd_bottom);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&T)�ù��Ĥ@��",  &filebuffer::cmd_top_edge);
    c_menubar.add_item(i, "&O)�ù�����",  &filebuffer::cmd_bottom_edge);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&G)�аO�ҩl",  &filebuffer::cmd_beginmark);
    c_menubar.add_item(i, "&M)�аO����",  &filebuffer::cmd_endmark);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)�檺�}�l",  &filebuffer::cmd_beginline);
    c_menubar.add_item(i, "?)���",  &filebuffer::cmd_endline);


    e_menubar.add_item("Window", "&Next file",	&filebuffer::cmd_edit);
    e_menubar.add_item("Window", "",	 (int (filebuffer::*)(void)) NULL);
//  e_menubar.add_item("Window", "&English Mode",  &filebuffer::cmd_lang_english);
//  e_menubar.add_item("Window", "&Chinese Mode",  &filebuffer::cmd_lang_big5);
    e_menubar.add_item("Window", "&Language: ",  &filebuffer::cmd_lang_big5, &filebuffer::menu_language);
    e_menubar.add_item("Window", "",	 (int (filebuffer::*)(void)) NULL);
//  e_menubar.add_item("Window", "&Insert Mode",  &filebuffer::cmd_insert_mode);
//  e_menubar.add_item("Window", "&Replace Mode",  &filebuffer::cmd_replace_mode);
    e_menubar.add_item("Window", "&Input Mode: ",  &filebuffer::cmd_insert_toggle, &filebuffer::menu_inputmode);
    e_menubar.add_item("Window", "",	 (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Window", "Command &History",  &filebuffer::cmd_history);
    e_menubar.add_item("Window", "History &Key: ",  &filebuffer::cmd_toggle_historykey, &filebuffer::menu_historykey);

    i = c_menubar.add_menu("W)����");
    c_menubar.add_item(i, "&N)�U���ɮ�",  &filebuffer::cmd_edit);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
//  c_menubar.add_item(i, "&E)nglish �^��Ҧ�",  &filebuffer::cmd_lang_english);
//  c_menubar.add_item(i, "&C)hinese ����Ҧ�",  &filebuffer::cmd_lang_big5);
    c_menubar.add_item(i, "&L)anguage: ",  &filebuffer::cmd_lang_english, &filebuffer::menu_language);
    c_menubar.add_item(i, "&I)��J�Ҧ�: ",  &filebuffer::cmd_insert_toggle, &filebuffer::menu_inputmode);
//  c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
//  c_menubar.add_item(i, "&I)���J�Ҧ�",  &filebuffer::cmd_insert_mode);
//  c_menubar.add_item(i, "&R)���N�Ҧ�",  &filebuffer::cmd_replace_mode);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&H)�R�O�C�����v�O��",  &filebuffer::cmd_history);
    c_menubar.add_item(i, "&K)�s�X���v�O������: ",  &filebuffer::cmd_toggle_historykey, &filebuffer::menu_historykey);

    e_menubar.add_item("Help", "&About ...",  &filebuffer::cmd_about);
    e_menubar.add_item("Help", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Help", "&Help ...",   &filebuffer::cmd_help);

    i = c_menubar.add_menu("H)���U");
    c_menubar.add_item(i, "&A)���� ...",  &filebuffer::cmd_about);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&H)�D�U ...",   &filebuffer::cmd_help);
}

int filebuffer::cmd_menu(void) {
    int   rtnval = 0;

    write_workbuffer(QE_IF_NECESSARY);

    do {
	display_messages(MSG_MENU_USAGE);

	keep_in_menu = MENU_IN_MENU;

	if (big5msg_flag == 1) {
	    rtnval = c_menubar.active(win, screen_y, screen_x, this);
	} else {
	    rtnval = e_menubar.active(win, screen_y, screen_x, this);
	}
    } while (keep_in_menu == MENU_KEEP_IN_MENU);

    if (keep_in_menu == MENU_IN_MENU) keep_in_menu = MENU_NOT_IN_MENU;

//  display_messages();

    return rtnval;
}

static char mbuf[50];

char *filebuffer::menu_inputmode(const char *str) {
    if (input_mode == 1) {
	sprintf(mbuf, "%s %s", str, (big5msg_flag == 1) ? "���J" : "Insert");
    } else {
	sprintf(mbuf, "%s %s", str, (big5msg_flag == 1) ? "�л\\" : "Replace");
    }
    return mbuf;
}

char *filebuffer::menu_blankcompress(const char *str) {
    if (blank_compress == 1) {
	sprintf(mbuf, "%s %s", str, "on");
    } else {
	sprintf(mbuf, "%s %s", str, "off");
    }
    return mbuf;
}

char *filebuffer::menu_historykey(const char *str) {
    if (history_key == HISTORY_KEY_NONE) {
	sprintf(mbuf, "%s%s", str, "NONE");
    } else if (history_key == HISTORY_KEY_UP) {
	sprintf(mbuf, "%s%s", str, "UP");
    } else if (history_key == HISTORY_KEY_DOWN) {
	sprintf(mbuf, "%s%s", str, "DOWN");
    } else if (history_key == HISTORY_KEY_BOTH) {
	sprintf(mbuf, "%s%s", str, "BOTH");
    }
    return mbuf;
}

char *filebuffer::menu_dirsort(const char *str) {
    if (dir_sort == DIR_SORT_BY_NAME) {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "�ɮצW��" : "NAME");
    } else if (dir_sort == DIR_SORT_BY_TIME) {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "�ɮ׮ɶ�" : "TIME");
    } else if (dir_sort == DIR_SORT_BY_SIZE) {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "�ɮפj�p" : "SIZE");
    }
    return mbuf;
}

char *filebuffer::menu_searchcase(const char *str) {
    if (search_case == 1) {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "�Ϥ�" : "EXACT");
    } else {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "���Ϥ�" : "ANY");
    }
    return mbuf;
}

char *filebuffer::menu_language(const char *str) {
    if (big5msg_flag == 1) {
	sprintf(mbuf, "%s%s", str, "ENGLISH");
    } else {
	sprintf(mbuf, "%s%s", str, "CHINESE");
    }
    return mbuf;
}

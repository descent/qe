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
    e_menubar.add_menu("File");    //  c_menubar.add_menu("F)檔案");
    e_menubar.add_menu("Edit");    //  c_menubar.add_menu("E)編輯");
    e_menubar.add_menu("Mark");    //  c_menubar.add_menu("M)標記");
    e_menubar.add_menu("Search");  //  c_menubar.add_menu("S)搜尋");
    e_menubar.add_menu("Goto");    //  c_menubar.add_menu("G)跳至");
//  e_menubar.add_menu("Option");  //  c_menubar.add_menu("G)選項");
    e_menubar.add_menu("Window");  //  c_menubar.add_menu("W)視窗");
    e_menubar.add_menu("Help");    //  c_menubar.add_menu("H)輔助");

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

    int  i = c_menubar.add_menu("F)檔案");
    c_menubar.add_item(i, "&D)目錄、讀檔", &filebuffer::cmd_dir);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&N)新檔",	   &filebuffer::cmd_new);
    c_menubar.add_item(i, "&S)存檔",	   &filebuffer::cmd_save);
    c_menubar.add_item(i, "&Q)關檔",	   &filebuffer::cmd_quit);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&B)存檔時空白壓成 TAB:",   &filebuffer::cmd_toggle_blankcompress, &filebuffer::menu_blankcompress);
    c_menubar.add_item(i, "&R)DIR 時排序方式: ",  &filebuffer::cmd_toggle_dirsort, &filebuffer::menu_dirsort);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&H)跳至 Shell ...",	     &filebuffer::cmd_shell);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&E)跳出 QE (全部存檔)", &filebuffer::cmd_save_all_quit);
    c_menubar.add_item(i, "&X)跳出 QE (不存檔)", &filebuffer::cmd_quit_all);


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

    i = c_menubar.add_menu("E)編輯");

    c_menubar.add_item(i, "&E)編輯", &filebuffer::cmd_cursor_data);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)插入一行", &filebuffer::cmd_insertline);
    c_menubar.add_item(i, "?)刪除本行", &filebuffer::cmd_deleteline);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)刪除游標左邊的字", &filebuffer::cmd_erase_begin);
    c_menubar.add_item(i, "?)刪除游標右邊的字", &filebuffer::cmd_erase_end);
    c_menubar.add_item(i, "?)刪除一個字元", &filebuffer::cmd_delchar);
    c_menubar.add_item(i, "?)倒退一個子元", &filebuffer::cmd_rubout);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)拆成兩行", &filebuffer::cmd_split);
    c_menubar.add_item(i, "?)接續下一行", &filebuffer::cmd_join);

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

    i = c_menubar.add_menu("M)標記");
    c_menubar.add_item(i, "&L)行式標記",   &filebuffer::cmd_markline);
    c_menubar.add_item(i, "&B)區塊式標記", &filebuffer::cmd_markblock);
    c_menubar.add_item(i, "&U)取消標記",   &filebuffer::cmd_unmark);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&C)複製標記區",   &filebuffer::cmd_copymark);
    c_menubar.add_item(i, "&V)複蓋\標記區", &filebuffer::cmd_overlayblock);
    c_menubar.add_item(i, "&M)搬移標記區",   &filebuffer::cmd_movemark);
    c_menubar.add_item(i, "&D)刪除標記區", &filebuffer::cmd_deletemark);
    c_menubar.add_item(i, "&P)丟進堆疊",   &filebuffer::cmd_pushmark);
    c_menubar.add_item(i, "&O)從堆疊中取出",	&filebuffer::cmd_popmark);
    c_menubar.add_item(i, "&K)清除堆疊", &filebuffer::cmd_clear_marks);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&S)向左 Shift", &filebuffer::cmd_shiftleft);
    c_menubar.add_item(i, "&R)向右 Shift", &filebuffer::cmd_shiftright);
    c_menubar.add_item(i, "&F)填入標記區",   &filebuffer::cmd_fillmark);
    c_menubar.add_item(i, "?)轉成大寫",   &filebuffer::cmd_uppercase);
    c_menubar.add_item(i, "?)轉成小寫",   &filebuffer::cmd_lowercase);

    e_menubar.add_item("Search", "&Find ...",  &filebuffer::cmd_search);
    e_menubar.add_item("Search", "",	 (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Search", "&Search case: ", &filebuffer::cmd_toggle_searchcase, &filebuffer::menu_searchcase);

    i = c_menubar.add_menu("S)搜尋");
    c_menubar.add_item(i, "&F)尋找 ...",  &filebuffer::cmd_search);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&S)搜尋時大小寫: ", &filebuffer::cmd_toggle_searchcase, &filebuffer::menu_searchcase);

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

    i = c_menubar.add_menu("G)跳至");
    c_menubar.add_item(i, "&C)游標跳至命令列",	&filebuffer::cmd_cursor_cmd);
    c_menubar.add_item(i, "&D)游標跳至資料區",	&filebuffer::cmd_cursor_data);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&B)檔案開頭",  &filebuffer::cmd_top);
    c_menubar.add_item(i, "&E)檔案結尾",    &filebuffer::cmd_bottom);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&T)螢幕第一行",  &filebuffer::cmd_top_edge);
    c_menubar.add_item(i, "&O)螢幕末行",  &filebuffer::cmd_bottom_edge);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&G)標記啟始",  &filebuffer::cmd_beginmark);
    c_menubar.add_item(i, "&M)標記結束",  &filebuffer::cmd_endmark);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "?)行的開始",  &filebuffer::cmd_beginline);
    c_menubar.add_item(i, "?)行尾",  &filebuffer::cmd_endline);


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

    i = c_menubar.add_menu("W)視窗");
    c_menubar.add_item(i, "&N)下個檔案",  &filebuffer::cmd_edit);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
//  c_menubar.add_item(i, "&E)nglish 英文模式",  &filebuffer::cmd_lang_english);
//  c_menubar.add_item(i, "&C)hinese 中文模式",  &filebuffer::cmd_lang_big5);
    c_menubar.add_item(i, "&L)anguage: ",  &filebuffer::cmd_lang_english, &filebuffer::menu_language);
    c_menubar.add_item(i, "&I)輸入模式: ",  &filebuffer::cmd_insert_toggle, &filebuffer::menu_inputmode);
//  c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
//  c_menubar.add_item(i, "&I)插入模式",  &filebuffer::cmd_insert_mode);
//  c_menubar.add_item(i, "&R)取代模式",  &filebuffer::cmd_replace_mode);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&H)命令列的歷史記錄",  &filebuffer::cmd_history);
    c_menubar.add_item(i, "&K)叫出歷史記錄的鍵: ",  &filebuffer::cmd_toggle_historykey, &filebuffer::menu_historykey);

    e_menubar.add_item("Help", "&About ...",  &filebuffer::cmd_about);
    e_menubar.add_item("Help", "",     (int (filebuffer::*)(void)) NULL);
    e_menubar.add_item("Help", "&Help ...",   &filebuffer::cmd_help);

    i = c_menubar.add_menu("H)輔助");
    c_menubar.add_item(i, "&A)關於 ...",  &filebuffer::cmd_about);
    c_menubar.add_item(i, "",	  (int (filebuffer::*)(void)) NULL);
    c_menubar.add_item(i, "&H)求助 ...",   &filebuffer::cmd_help);
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
	sprintf(mbuf, "%s %s", str, (big5msg_flag == 1) ? "插入" : "Insert");
    } else {
	sprintf(mbuf, "%s %s", str, (big5msg_flag == 1) ? "覆蓋\" : "Replace");
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
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "檔案名稱" : "NAME");
    } else if (dir_sort == DIR_SORT_BY_TIME) {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "檔案時間" : "TIME");
    } else if (dir_sort == DIR_SORT_BY_SIZE) {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "檔案大小" : "SIZE");
    }
    return mbuf;
}

char *filebuffer::menu_searchcase(const char *str) {
    if (search_case == 1) {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "區分" : "EXACT");
    } else {
	sprintf(mbuf, "%s%s", str, big5msg_flag ? "不區分" : "ANY");
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

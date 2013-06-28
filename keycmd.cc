/*
 *  keycmd.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "keycmd.h"
#include "misc.h"
// #include "filebuffer.h"

struct command_mapping	cmdmap[] = {
    { "left"		  , &filebuffer::cmd_left,	     QE_CMD_LEFT    },
    { "right"		  , &filebuffer::cmd_right,	     QE_CMD_RIGHT   },
    { "il"		  , &filebuffer::cmd_insertline,     QE_CMD_IL	    },
    { "insert line"	  , &filebuffer::cmd_insertline,     QE_CMD_IL	    },
    { "up"		  , &filebuffer::cmd_up,	     QE_CMD_UP	    },
    { "down"		  , &filebuffer::cmd_down,	     QE_CMD_DOWN    },
    { "bl"		  , &filebuffer::cmd_beginline,      QE_CMD_BL	    },
    { "begin line"	  , &filebuffer::cmd_beginline,      QE_CMD_BL	    },
    { "pu"		  , &filebuffer::cmd_pgup,	     QE_CMD_PGUP    },
    { "page up" 	  , &filebuffer::cmd_pgup,	     QE_CMD_PGUP    },
    { "pd"		  , &filebuffer::cmd_pgdn,	     QE_CMD_PGDN    },
    { "page down"	  , &filebuffer::cmd_pgdn,	     QE_CMD_PGDN    },
    { "el"		  , &filebuffer::cmd_endline,	     QE_CMD_EL	    },
    { "end line"	  , &filebuffer::cmd_endline,	     QE_CMD_EL	    },
    { "ro"		  , &filebuffer::cmd_rubout,	     QE_CMD_RO	    },
    { "rubout"		  , &filebuffer::cmd_rubout,	     QE_CMD_RO	    },
    { "dc"		  , &filebuffer::cmd_delchar,	     QE_CMD_RO	    },
    { "delete char"	  , &filebuffer::cmd_delchar,	     QE_CMD_RO	    },
    { "ud"		  , &filebuffer::cmd_undo,	     QE_CMD_UNDO    },
    { "undo"		  , &filebuffer::cmd_undo,	     QE_CMD_UNDO    },
    { "fn"		  , &filebuffer::cmd_first_nonblank, QE_CMD_FN	    },
    { "first nonblank"	  , &filebuffer::cmd_first_nonblank, QE_CMD_FN	    },
    { "cg"		  , &filebuffer::cmd_command_toggle, QE_CMD_CG	    },
    { "command toggle"	  , &filebuffer::cmd_command_toggle, QE_CMD_CG	    },
    { "it"		  , &filebuffer::cmd_insert_toggle,  QE_CMD_IT	    },
    { "insert toggle"	  , &filebuffer::cmd_insert_toggle,  QE_CMD_IT	    },
    { "te"		  , &filebuffer::cmd_top_edge,	     QE_CMD_TE	    },
    { "top edge"	  , &filebuffer::cmd_top_edge,	     QE_CMD_TE	    },
    { "be"		  , &filebuffer::cmd_bottom_edge,    QE_CMD_BE	    },
    { "bottom edge"	  , &filebuffer::cmd_bottom_edge,    QE_CMD_BE	    },
    { "tb"		  , &filebuffer::cmd_tab,	     QE_CMD_TAB     },
    { "tab"		  , &filebuffer::cmd_tab,	     QE_CMD_TAB     },
    { "to"		  , &filebuffer::cmd_top,	     QE_CMD_TOP     },
    { "top"		  , &filebuffer::cmd_top,	     QE_CMD_TOP     },
    { "bo"		  , &filebuffer::cmd_bottom,	     QE_CMD_BOTTOM  },
    { "bottom"		  , &filebuffer::cmd_bottom,	     QE_CMD_BOTTOM  },
    { "sp"		  , &filebuffer::cmd_split,	     QE_CMD_SP	    },
    { "split"		  , &filebuffer::cmd_split,	     QE_CMD_SP	    },
    { "jo"		  , &filebuffer::cmd_join,	     QE_CMD_JO	    },
    { "join"		  , &filebuffer::cmd_join,	     QE_CMD_JO	    },
    { "dl"		  , &filebuffer::cmd_deleteline,     QE_CMD_DL	    },
    { "delete line"	  , &filebuffer::cmd_deleteline,     QE_CMD_DL	    },
    { "im"		  , &filebuffer::cmd_insert_mode,    QE_CMD_IM	    },
    { "insert mode"	  , &filebuffer::cmd_insert_mode,    QE_CMD_IM	    },
    { "rm"		  , &filebuffer::cmd_replace_mode,   QE_CMD_RM	    },
    { "replace mode"	  , &filebuffer::cmd_replace_mode,   QE_CMD_RM	    },
    { "ee"		  , &filebuffer::cmd_erase_end,      QE_CMD_EE	    },
    { "erase end line"	  , &filebuffer::cmd_erase_end,      QE_CMD_EE	    },
    { "eb"		  , &filebuffer::cmd_erase_begin,    QE_CMD_EB	    },
    { "save"		  , &filebuffer::cmd_save,	     QE_CMD_SAVE    },
    { "quit"		  , &filebuffer::cmd_quit,	     QE_CMD_QUIT    },
    { "ex"		  , &filebuffer::cmd_execute,	     QE_CMD_EX	    },
    { "execute" 	  , &filebuffer::cmd_execute,	     QE_CMD_EX	    },
    { "cc"		  , &filebuffer::cmd_cursor_cmd,     QE_CMD_CC	    },
    { "cursor command"	  , &filebuffer::cmd_cursor_cmd,     QE_CMD_CC	    },
    { "ca"		  , &filebuffer::cmd_cursor_data,    QE_CMD_CA	    },
    { "cursor data"	  , &filebuffer::cmd_cursor_data,    QE_CMD_CA	    },
    { "bp"		  , &filebuffer::cmd_beep,	     QE_CMD_BEEP    },
    { "beep"		  , &filebuffer::cmd_beep,	     QE_CMD_BEEP    },
    { "scrollup"	  , &filebuffer::cmd_scrollup,	     QE_CMD_SCUP    },
    { "scrolldown"	  , &filebuffer::cmd_scrolldown,     QE_CMD_SCDOWN  },
    { "scrollleft"	  , &filebuffer::cmd_scrollleft,     QE_CMD_SCLEFT  },
    { "scrollright"	  , &filebuffer::cmd_scrollright,    QE_CMD_SCRIGHT },
    { "ps"		  , &filebuffer::cmd_pushmark,	     QE_CMD_PS	    },
    { "push mark"	  , &filebuffer::cmd_pushmark,	     QE_CMD_PS	    },
    { "po"		  , &filebuffer::cmd_popmark,	     QE_CMD_PO	    },
    { "pop mark"	  , &filebuffer::cmd_popmark,	     QE_CMD_PO	    },
    { "ml"		  , &filebuffer::cmd_markline,	     QE_CMD_ML	    },
    { "mark line"	  , &filebuffer::cmd_markline,	     QE_CMD_ML	    },
    { "cm"		  , &filebuffer::cmd_copymark,	     QE_CMD_CM	    },
    { "copy mark"	  , &filebuffer::cmd_copymark,	     QE_CMD_CM	    },
    { "mb"		  , &filebuffer::cmd_markblock,      QE_CMD_MB	    },
    { "mark block"	  , &filebuffer::cmd_markblock,      QE_CMD_MB	    },
    { "mc"		  , &filebuffer::cmd_markchar,	     QE_CMD_MC	    },
    { "marc char"	  , &filebuffer::cmd_markchar,	     QE_CMD_MC	    },
    { "um"		  , &filebuffer::cmd_unmark,	     QE_CMD_UM	    },
    { "unmark"		  , &filebuffer::cmd_unmark,	     QE_CMD_UM	    },
    { "mm"		  , &filebuffer::cmd_movemark,	     QE_CMD_MM	    },
    { "move mark"	  , &filebuffer::cmd_movemark,	     QE_CMD_MM	    },
    { "dm"		  , &filebuffer::cmd_deletemark,     QE_CMD_DM	    },
    { "delete mark"	  , &filebuffer::cmd_deletemark,     QE_CMD_DM	    },
    { "fm"		  , &filebuffer::cmd_fillmark,	     QE_CMD_FM	    },
    { "fill mark"	  , &filebuffer::cmd_fillmark,	     QE_CMD_FM	    },
    { "bm"		  , &filebuffer::cmd_beginmark,      QE_CMD_BM	    },
    { "begin mark"	  , &filebuffer::cmd_beginmark,      QE_CMD_BM	    },
    { "em"		  , &filebuffer::cmd_endmark,	     QE_CMD_EM	    },
    { "end mark"	  , &filebuffer::cmd_endmark,	     QE_CMD_EM	    },
    { "ob"		  , &filebuffer::cmd_overlayblock,   QE_CMD_OB	    },
    { "overlay block"	  , &filebuffer::cmd_overlayblock,   QE_CMD_OB	    },
    { "sl"		  , &filebuffer::cmd_shiftleft,      QE_CMD_SL	    },
    { "shift left"	  , &filebuffer::cmd_shiftleft,      QE_CMD_SL	    },
    { "sr"		  , &filebuffer::cmd_shiftright,     QE_CMD_SR	    },
    { "shift right"	  , &filebuffer::cmd_shiftright,     QE_CMD_SR	    },
    { "e"		  , &filebuffer::cmd_edit,	     QE_CMD_EDIT    },
    { "edit"		  , &filebuffer::cmd_edit,	     QE_CMD_EDIT    },
    { "we"		  , &filebuffer::cmd_wordend,	     QE_CMD_WE	    },
    { "end word"	  , &filebuffer::cmd_wordend,	     QE_CMD_WE	    },
    { "wb"		  , &filebuffer::cmd_wordbegin,      QE_CMD_WB	    },
    { "begin word"	  , &filebuffer::cmd_wordbegin,      QE_CMD_WB	    },
    { "co"		  , &filebuffer::cmd_confirm,	     QE_CMD_CO	    },
    { "confirm change"	  , &filebuffer::cmd_confirm,	     QE_CMD_CO	    },
    { "cl"		  , &filebuffer::cmd_centerline,     QE_CMD_CL	    },
    { "center line"	  , &filebuffer::cmd_centerline,     QE_CMD_CL	    },
    { "bw"		  , &filebuffer::cmd_backword,	     QE_CMD_BW	    },
    { "backtab word"	  , &filebuffer::cmd_backword,	     QE_CMD_BW	    },
    { "tw"		  , &filebuffer::cmd_tabword,	     QE_CMD_TW	    },
    { "tab word"	  , &filebuffer::cmd_tabword,	     QE_CMD_TW	    },
    { "cf"		  , &filebuffer::cmd_copyfromcmd,    QE_CMD_CF	    },
    { "copy from command" , &filebuffer::cmd_copyfromcmd,    QE_CMD_CF	    },
    { "ct"		  , &filebuffer::cmd_copytocmd,      QE_CMD_CT	    },
    { "copy to command"   , &filebuffer::cmd_copytocmd,      QE_CMD_CT	    },
    { "lc"		  , &filebuffer::cmd_lowercase,      QE_CMD_LC	    },
    { "lowercase"	  , &filebuffer::cmd_lowercase,      QE_CMD_LC	    },
    { "uc"		  , &filebuffer::cmd_uppercase,      QE_CMD_UC	    },
    { "uppercase"	  , &filebuffer::cmd_uppercase,      QE_CMD_UC	    },
    { "ck"		  , &filebuffer::cmd_clear_marks,    QE_CMD_CK	    },
    { "clear_marks"	  , &filebuffer::cmd_clear_marks,    QE_CMD_CK	    },
    { "menu"		  , &filebuffer::cmd_menu,	     QE_CMD_MENU    },
    { "kl"		  , &filebuffer::cmd_kill_line,      QE_CMD_KL	    },
    { "kill line"	  , &filebuffer::cmd_kill_line,      QE_CMD_KL	    },
    { "search"		  , &filebuffer::cmd_search,	     QE_CMD_SEARCH  },
    { "dir"		  , &filebuffer::cmd_dir,	     QE_CMD_DIR     },
    { "history" 	  , &filebuffer::cmd_history,	     QE_CMD_HISTORY },
    { "quit all"	  , &filebuffer::cmd_quit_all,	     QE_CMD_QUITALL },
    { "fill all"	  , &filebuffer::cmd_save_all_quit,  QE_CMD_FILEALL },
    { "?b"		  , &filebuffer::query_blnkcmprs,    QE_CMD_QBCP    },
    { "?s"		  , &filebuffer::query_searchcase,   QE_CMD_QSCS    },
    { "?k"		  , &filebuffer::query_ctrl_k,	     QE_CMD_QCK     },
    { "?x"		  , &filebuffer::query_ctrl_x,	     QE_CMD_QCX     },
    { "?q"		  , &filebuffer::query_ctrl_q,	     QE_CMD_QCQ     },
    { "?5"		  , &filebuffer::query_big5,	     QE_CMD_QB5     },
//  { "decode base64"	  , &filebuffer::cmd_decodebase64,   QE_CMD_BASE64  },
#if 0
    { "rd"		  , &filebuffer::cmd_redraw,	     QE_CMD_RD	    },
    { "bt"		  , &filebuffer::cmd_backtab,	     QE_CMD_BT	    },
    { "ci"		  , &filebuffer::cmd_centermargins,  QE_CMD_CI	    },
    { "ce"		  , &filebuffer::cmd_clear_msg,      QE_CMD_CE	    },
    { "column"		  , &filebuffer::cmd_column,	     QE_CMD_COLUMN  },
    { "de"		  , &filebuffer::cmd_demo_end,	     QE_CMD_DE	    },
    { "df"		  , &filebuffer::cmd_demo_fast,      QE_CMD_DF	    },
    { "ds"		  , &filebuffer::cmd_demo_slow,      QE_CMD_DS	    },
    { "es"		  , &filebuffer::cmd_escape,	     QE_CMD_ES	    },
    { "fb"		  , &filebuffer::cmd_findblankline,  QE_CMD_FB	    },
    { "in"		  , &filebuffer::cmd_indent,	     QE_CMD_IN	    },
    { "le"		  , &filebuffer::cmd_leftedge,	     QE_CMD_LE	    },
    { "lg"		  , &filebuffer::cmd_leftmargin,     QE_CMD_LG	    },
    { "lc"		  , &filebuffer::cmd_lowercase,      QE_CMD_LC	    },
    { "uc"		  , &filebuffer::cmd_uppercase,      QE_CMD_UC	    },
    { "nv"		  , &filebuffer::cmd_nextview,	     QE_CMD_NV	    },
    { "nw"		  , &filebuffer::cmd_nextwindow,     QE_CMD_NW	    },
    { "rf"		  , &filebuffer::cmd_reflow,	     QE_CMD_RF	    },
    { "re"		  , &filebuffer::cmd_rightedge,      QE_CMD_RE	    },
    { "rg"		  , &filebuffer::cmd_rightmargin,    QE_CMD_RG	    },
    { "ss"		  , &filebuffer::cmd_split_screen,   QE_CMD_SS	    },
    { "zw"		  , &filebuffer::cmd_zoom_window,    QE_CMD_ZW	    },
#endif
    { NULL		  , NULL,			     QE_CMD_NULL    }
};

keycmdlist *keydef[QE_KEY_MAX];

int init_keycmd(void) {
    keydef[KEY_UP]	  = new keycmdlist("[up]");
    keydef[KEY_DOWN]	  = new keycmdlist("[down]");
    keydef[KEY_LEFT]	  = new keycmdlist("[left]");
    keydef[KEY_RIGHT]	  = new keycmdlist("[right]");
    keydef[KEY_ESC]	  = new keycmdlist("[cg]");
    keydef[KEY_RETURN]	  = new keycmdlist("[il][up][fn][down]");
    keydef[KEY_HOME]	  = new keycmdlist("[bl]");
//  keydef[KEY_PGUP]	  = new keycmdlist("[pgup]");
//  keydef[KEY_PGDN]	  = new keycmdlist("[pgdn]");
    keydef[KEY_PPAGE]	  = new keycmdlist("[pu]");
    keydef[KEY_NPAGE]	  = new keycmdlist("[pd]");
    keydef[KEY_PADEND]	  = new keycmdlist("[el]");
    keydef[KEY_END]	  = new keycmdlist("[el]");
    keydef[KEY_BACKSPACE] = new keycmdlist("[ro]");
    keydef[KEY_META('l')] = new keycmdlist("[ml]");
    keydef[KEY_META('L')] = new keycmdlist("[ml]");
    keydef[KEY_META('b')] = new keycmdlist("[mb]");
    keydef[KEY_META('B')] = new keycmdlist("[mb]");
    keydef[KEY_META('U')] = new keycmdlist("[um]");
    keydef[KEY_META('u')] = new keycmdlist("[um]");
    keydef[KEY_META('d')] = new keycmdlist("[ee]");
    keydef[KEY_META('D')] = new keycmdlist("[ee]");
    keydef[KEY_META('c')] = new keycmdlist("[cm]");
    keydef[KEY_META('C')] = new keycmdlist("[cm]");
    keydef[KEY_META('f')] = new keycmdlist("[fm]");
    keydef[KEY_META('F')] = new keycmdlist("[fm]");
    keydef[KEY_CTRL('a')] = new keycmdlist("[bl]");
    keydef[KEY_CTRL('e')] = new keycmdlist("[el]");
    keydef[KEY_CTRL('d')] = new keycmdlist("[dc]");
    keydef[KEY_CTRL('u')] = new keycmdlist("[ud]");
    keydef[KEY_CTRL('k')] = new keycmdlist("[scrolldown]");
    keydef[KEY_LINEFEED ] = new keycmdlist("[il][up][fn][down]");
    keydef[KEY_CTRL('l')] = new keycmdlist("[scrollright]");
    keydef[KEY_DC]	  = new keycmdlist("[dc]");
    // keydef[ESCAPE(18)] = new keycmdlist("[cg]");
    keydef[KEY_IC]	  = new keycmdlist("[it]");
    keydef[KEY_TAB]	  = new keycmdlist("[tab]");
    keydef[KEY_CTRL('y')] = new keycmdlist("[dl]");
    keydef[KEY_F(2)]	  = new keycmdlist("[cc][bl][ee]'save'[beep]");
    keydef[KEY_F(3)]	  = new keycmdlist("[cc][bl][ee]'file'[beep]");
    keydef[KEY_F(4)]	  = new keycmdlist("[cc][bl][ee]'quit'[beep]");
    keydef[KEY_F(5)]	  = new keycmdlist("[ps]");
    keydef[KEY_F(6)]	  = new keycmdlist("[po]");
    keydef[KEY_F(8)]	  = new keycmdlist("[e]");

    return 1;
}

keycmdlist::keycmdlist(const char  *cmdlist) {
    int     begin_text = 0, found;
    char    cmdbuffer[8000];
    int     idx = 0, len, error = 0;
    int     i;
    int     quotelevel = 0, maxlevel = 0;

    if ((len = strlen(cmdlist)) > 8000) {
	list = NULL;
	return;
    }

    keycmd  *ptr = new keycmd;
    list = ptr;

    for (int j = 0; !error && j < len; j++) {
	switch(cmdlist[j]) {
	case '['  :
	    if (quotelevel == 0) {
		idx	 = 0;
		maxlevel = 0;
	    } else {
		cmdbuffer[idx++] = cmdlist[j];
	    }
	    quotelevel++;
	    maxlevel = maximum(maxlevel, quotelevel);
	    break;
	case ']'  :
	    if (quotelevel == 1) {
		cmdbuffer[idx] = '\0';

		for (i = found = 0; cmdmap[i].command != NULL; i++) {
		    if (strcmp(cmdbuffer, cmdmap[i].command) == 0) {
			found = 1;
			break;
		    }
		}

		if (! found) {
		    begin_text = 0;
		    ptr->next = new keycmd;
		    ptr = ptr->next;
		    ptr->cmd	= QE_CMD_CMDQUOTE;
		    ptr->str	= qeStrdup(cmdbuffer);
		    ptr->action = (int (filebuffer::*)(void)) NULL;
		} else {
		    ptr->next	= new keycmd;
		    ptr 	= ptr->next;
		    ptr->cmd	= cmdmap[i].cmd;
		    ptr->action = cmdmap[i].action;
		    ptr->str	= NULL;
		}
	    } else {
		cmdbuffer[idx++] = cmdlist[j];
	    }
	    quotelevel--;
	    break;
	case '\'' :
	    if (quotelevel == 0) {
		if (begin_text) {
		    cmdbuffer[idx] = '\0';

		    begin_text = 0;
		    ptr->next = new keycmd;
		    ptr = ptr->next;
		    ptr->cmd	= QE_CMD_STRING;
		    ptr->str	= qeStrdup(cmdbuffer);
		    ptr->action = (int (filebuffer::*)(void)) NULL;
		} else {
		    begin_text = 1;
		    idx = 0;
		}
	    } else {
		cmdbuffer[idx++] = cmdlist[j];
	    }
	    break;
	default   :
	    if (quotelevel == 0 && !begin_text) {
		if (cmdlist[j] != ' ' && cmdlist[j] != '\t') error = 1;
	    }
	    cmdbuffer[idx++] = cmdlist[j];
	    break;
	}
    }

    ptr->next = NULL;

    if (error) {
	keycmd	*q = list->next;

	while (q != NULL) {
	    ptr = q;
	    q	= ptr->next;
	    if (ptr->str != NULL) delete ptr->str;
	    delete ptr;
	}
    }

    ptr = list;
    list = list->next;
    delete ptr;

    if (error) list = NULL;
}

keycmdlist::~keycmdlist(void) {
    keycmd  *q;

    for (keycmd *ptr = list; ptr != NULL;) {
	if (ptr->str != NULL) qeFree(ptr->str);
	q = ptr;
	ptr = ptr->next;
	delete q;
    }

    list = NULL;
}

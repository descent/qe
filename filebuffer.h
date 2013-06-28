/*
 *  filebuffer.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#ifndef __QE_FILEBUFFER_H_
#define __QE_FILEBUFFER_H_

#include <limits.h>
#include "config.h"
#include "qestring.h"
#include "linebuffer.h"
#include "history.h"
#include "mark.h"
#include "menu.h"


#ifndef TAB_SPACE
#define TAB_SPACE	(8)
#endif

#define FB_COMMAND_MODE   (0)
#define FB_DATAAREA_MODE  (1)
#define FB_REPLACE_MODE   (0)
#define FB_INSERT_MODE	  (1)

#define MENU_NOT_IN_MENU  (0)
#define MENU_IN_MENU	  (1)
#define MENU_KEEP_IN_MENU (2)
#define MENU_BACK_TO_MENU (3)

#define HISTORY_KEY_NONE  (0)
#define HISTORY_KEY_UP	  (1)
#define HISTORY_KEY_DOWN  (2)
#define HISTORY_KEY_BOTH  (3)

#define DIR_SORT_BY_NAME  (0)
#define DIR_SORT_BY_TIME  (1)
#define DIR_SORT_BY_SIZE  (2)

#define RTNVAL_NEXT_BUFFER	    (1)
#define RTNVAL_SHELL_ESCAPE	    (2)
#define RTNVAL_QUIT_BUFFER	    (3)
#define RTNVAL_QUIT_CURRENT_BUFFER  (4)
#define RTNVAL_QUIT_ALL_BUFFER	    (5)
#define RTNVAL_SAVE_ALL_AND_QUIT    (6)

#define NORMAL_TEXT_BUFFER   (0)
#define DIRECTORY_BUFFER     (1)

#define QE_IF_NECESSARY      (0)
#define QE_FORCE	     (1)

#define maximum(x,y)	  (((x)>(y))?(x):(y))
#define minimum(x,y)	  (((x)<(y))?(x):(y))

#ifndef USE_ANSICOLOR
#define setattr(x,y)	wattrset(x,y)
#endif

class keydefine;

class filebuffer {
protected:
    qeString	      filename;
    WINDOW	      *win;
    char	      cmdline[MAX_BUFFER_LEN];
    int 	      cmdline_idx;
    int 	      screen_x, screen_y;
    int 	      cursor_x, cursor_y;
    int 	      buffer_x, buffer_y;
    unsigned int      left_col_no;
    linebuffer	      *head, *tail, *current;
    linebuffer	      *topline;
    int 	      top_line_no;
    int 	      total_line;
    short	      mode;
    short	      loadingOK;
    short	      input_mode;
    short	      dirty_flag;
    short	      dirty_line;
    short	      quiet_flag;
    short	      read_only;
    short	      buffer_type;
    short	      message_dirty_flag;
    short	      keep_in_menu;

    char	      workbuffer[MAX_BUFFER_LEN];
    int 	      workbufidx;
    int 	      workbuflen;

    int 	      clientarea(void);
    int 	      cmdarea(void);

protected:
    static const int  markstacksize = 10;
    static mark       markstack[];
    static int	      marksp;
    static mark       current_mark;
    static filebuffer *current_buffer;
    filebuffer	      *next_buffer;
    filebuffer	      *previous_buffer;
    static int	      push_mark(void);
    static int	      pop_mark(void);
    static short      enable_ctrl_k;
    static short      enable_ctrl_x;
    static short      enable_ctrl_q;
    static short      history_key;
    static short      blank_compress;
    static short      search_case;
    static short      dir_sort;
    static char       current_dir[PATH_MAX];
    static int	      screen_columns;
    static int	      screen_lines;
    static keydefine  *kdf;
    static short      big5msg_flag;
    static short      quit_confirm;
    static menu       menubar;
    static cmdHistory history;
#ifdef USE_ANSICOLOR
    static int	      ansi_color;
    void	      setattr(WINDOW *w, int color);
#endif

    void	      init_window(void);
    void	      init_menu(void);
    void	      load_file(void);

protected:
    void	      general_init(void);
    void	      display_status(void);
    void	      load_workbuffer(void);
    void	      write_workbuffer(const int force);
    virtual void      refresh_clientarea(const int  bflflag);
    virtual void      refresh_bufferline(void);
    void	      refresh_cmdarea(void);
    void	      display_filename(void);
    int 	      dirty_buffer(const int force = 0);
    void	      display_messages(const char *fmt, ...);
    void	      display_messages(const int  msgno);
    void	      display_messages(void);
    int 	      getfirstkey(void);
    int 	      getnextkey(void);
    void	      ca_attrset(void);
    char	      outputformat[10];
    int 	      stringcompare(const char *s1, const char *s2);
    int 	      stringncompare(const char *s1,const char *s2,const int n);
    void	      fix_all_mark_position(const int tp);
    void	      fix_mark_position(mark& mk, const int tp);
    void	      unmark_all_mark(void);
    void	      moveto(const int y, const int x);
    void	      mvprint(const int y, const int x, const char *str);
    void	      mvprint(const int y, const int x, const int len,
							 const char *str);
public:
    filebuffer(void);
    filebuffer(const char *fname);

    virtual	      ~filebuffer(void);
    static void       init_curses(void);
    static void       end_curses(void);
    static int	      setenviron(const char  *string);
    static void       init_keydefine(const char *fname);
    static void       init_keydefine(void);
    static int	      is_big5(void);

    static void       out_of_memory(void);

    filebuffer*       active(void);

protected:

    void	      paint(void);
    int 	      cmd_kbinput(void);
    int 	      cmd_edit(const char *filename, const int ro = 0);
    int 	      cmd_dir(const char *path);
    int 	      cmd_execute_key(const int  keycode);
    int 	      cmd_execute(const char  *cmdstr);
    int 	      cmd_cmdquote(const char *quote);
    virtual int       do_function_key(const int  key);
    int 	      cmd_change(const char *from, const char *to,
				     const int dir = 0, const int mk = 0,
				     int confirm = 1, int all = 0);
    int 	      cmd_find(const char *str, const int dir = 0,
							   const int mark = 0);
    int 	      search_current_line(const char *str, const int  dir,
						    linebuffer* lptr, int &x);
    int 	      search_next_line(const char *str, const int  dir,
					    linebuffer*& lptr, int &y, int &x);
public:
    virtual int       cmd_up(void);
    virtual int       cmd_down(void);
    virtual int       cmd_left(void);
    virtual int       cmd_right(void);
    virtual int       cmd_pgup(void);
    virtual int       cmd_pgdn(void);
    virtual int       cmd_scrollup(void);
    virtual int       cmd_scrolldown(void);
    virtual int       cmd_scrollleft(void);
    virtual int       cmd_scrollright(void);
    virtual int       cmd_wordend(void);
    virtual int       cmd_wordbegin(void);
    virtual int       cmd_beginline(void);
    virtual int       cmd_endline(void);
    virtual int       cmd_first_nonblank(void);
    virtual int       cmd_top_edge(void);
    virtual int       cmd_bottom_edge(void);
    virtual int       cmd_tab(void);
    virtual int       cmd_top(void);
    virtual int       cmd_bottom(void);
    virtual int       cmd_centerline(void);
    virtual int       cmd_backword(void);
    virtual int       cmd_tabword(void);
    virtual int       cmd_quit(void);
    virtual int       cmd_file(void);
    virtual int       cmd_save(void);
    virtual int       cmd_new(void);
    virtual int       cmd_edit(void);
    virtual int       cmd_dir(void);
    virtual int       cmd_shell(void);
    virtual int       cmd_command_toggle(void);
    virtual int       cmd_insert_toggle(void);
    virtual int       cmd_insert_mode(void);
    virtual int       cmd_replace_mode(void);
    virtual int       cmd_cursor_data(void);
    virtual int       cmd_cursor_cmd(void);
    virtual int       cmd_insertline(void);
    virtual int       cmd_rubout(void);
    virtual int       cmd_delchar(void);
    virtual int       cmd_undo(void);
    virtual int       cmd_split(void);
    virtual int       cmd_join(void);
    virtual int       cmd_deleteline(void);
    virtual int       cmd_erase_begin(void);
    virtual int       cmd_erase_end(void);
    virtual int       cmd_confirm(void);
    virtual int       cmd_execute(void);
    virtual int       cmd_beep(void);
    virtual int       cmd_pushmark(void);
    virtual int       cmd_popmark(void);
    virtual int       cmd_markline(void);
    virtual int       cmd_markblock(void);
    virtual int       cmd_markchar(void);
    virtual int       cmd_clear_marks(void);
    virtual int       cmd_unmark(void);
    virtual int       cmd_copymark(void);
    virtual int       cmd_movemark(void);
    virtual int       cmd_deletemark(void);
    virtual int       cmd_fillmark(void);
    virtual int       cmd_beginmark(void);
    virtual int       cmd_endmark(void);
    virtual int       cmd_overlayblock(void);
    virtual int       cmd_shiftleft(void);
    virtual int       cmd_shiftright(void);
    virtual int       cmd_copyfromcmd(void);
    virtual int       cmd_copytocmd(void);
    virtual int       cmd_uppercase(void);
    virtual int       cmd_lowercase(void);
//  virtual int       cmd_decodebase64(void);

    virtual int       cmd_gotoline(void);
    virtual int       cmd_search(void);
    virtual int       cmd_menu(void);
    virtual int       cmd_kill_line(void);
    virtual int       query_blnkcmprs(void);
    virtual int       query_searchcase(void);
    virtual int       query_ctrl_k(void);
    virtual int       query_ctrl_x(void);
    virtual int       query_ctrl_q(void);
    virtual int       query_big5(void);
    virtual int       cmd_about(void);
    virtual int       cmd_help(void);
    virtual int       cmd_lang_big5(void);
    virtual int       cmd_lang_english(void);
    virtual int       cmd_toggle_blankcompress(void);
    virtual int       cmd_history(void);
    virtual int       cmd_toggle_historykey(void);
    virtual int       cmd_toggle_dirsort(void);
    virtual int       cmd_toggle_searchcase(void);
    virtual int       cmd_quit_all(void);
    virtual int       cmd_save_all_quit(void);

public:
    char *	      menu_language(const char *str);
    char *	      menu_blankcompress(const char *str);
    char *	      menu_inputmode(const char *str);
    char *	      menu_historykey(const char *str);
    char *	      menu_dirsort(const char *str);
    char *	      menu_searchcase(const char *str);

// friend:
    friend int	      mark::renumber(void);
};

typedef int (filebuffer::*filebuffer_pmfi)(void);

#endif

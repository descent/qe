/*
 *  history.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#ifndef __QE_HISTORY_H_
#define __QE_HISTORY_H_

#include "config.h"

class linebuffer;

class cmdHistory {
protected:
    linebuffer	 *head;
    linebuffer	 *tail;
    int 	 max_record;
    int 	 record_no;
    WINDOW	 *win;
    int 	 height;
    int 	 width;
protected:
    void	 init_cmdHistory(void);
    int 	 display_history(linebuffer *ptr, const linebuffer *sel);
public:
    cmdHistory(void);
    cmdHistory(const int  record);
   ~cmdHistory(void);

    void	 push(const char *str);
    char *	 pop(void);
    char *	 top(void);

    int 	 active(char *cmdline, WINDOW *pwin, const int y, const int x,
						   const int h, const int w);
};

#endif

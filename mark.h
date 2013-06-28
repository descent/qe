/*
 *  mark.h
 *
 *  Copyright (C) 1998  Jiann-Ching Liu
 */

#ifndef __QE_MARK_H_
#define __QE_MARK_H_

#include "linebuffer.h"

class filebuffer;

#define MARK_STACK_SIZE   (5)

#define UNKNOW_MARK       (0)
#define CHAR_MARK         (1)
#define LINE_MARK         (2)
#define BLOCK_MARK        (3)

#define FIX_MARK_INSERT_LINE   (0)
#define FIX_MARK_DELETE_LINE   (1)
#define FIX_MARK_JOIN_LINE     (2)

class mark {
protected:
    int         type;
public:
    filebuffer  *fb;
    linebuffer  *first;
    linebuffer  *last;
    int         fn;
    int         ln;
    int         fc;
    int         lc;

    mark(void);
    mark(filebuffer* fbf);
    mark(filebuffer* fbf, const int  tp);
    mark(filebuffer* fbf, linebuffer *f, linebuffer *l);

    int         renumber(void);

    void        settype(int tp = UNKNOW_MARK);
    int         gettype(void);
    operator    int(void);
    mark        operator = (mark& mk);
};

#endif

/*
 *  mark.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdlib.h>
#include "mark.h"
#include "filebuffer.h"

mark::mark(void) {
    fb	   = NULL;
    first  = NULL;
    last   = NULL;
    type   = UNKNOW_MARK;
}

mark::mark(filebuffer* fbf) {
    fb	   = fbf;
    first  = NULL;
    last   = NULL;
    type   = UNKNOW_MARK;
}

mark::mark(filebuffer* fbf,  const int	tp) {
    fb	   = fbf;
    first  = NULL;
    last   = NULL;
    type   = tp;
}

mark::mark(filebuffer* fbf, linebuffer *f, linebuffer *l) {
    fb	   = fbf;
    first  = f;
    last   = l;
    type   = UNKNOW_MARK;
}

mark mark::operator = (mark& mk) {
    fb	   = mk.fb;
    first  = mk.first;
    last   = mk.last;
    type   = mk.type;
    fc	   = mk.fc;
    lc	   = mk.lc;
    fn	   = mk.fn;
    ln	   = mk.ln;

    return *this;
}


void mark::settype(int tp) {
    type = tp;
}

int mark::gettype(void) {
    return type;
}

mark::operator int(void) {
    return type;
}

int mark::renumber(void) {
    linebuffer	*ptr;
    int 	lineno = 0;
    int 	match  = 0;

    if (type == UNKNOW_MARK) return -1;

    for (ptr = fb->head->next; match < 2 && ptr != fb->tail;
					       ptr = ptr->next, lineno++) {
	if (ptr == first) match++, fn = lineno;
	if (ptr == last)  match++, ln = lineno;
    }

    return fn;
}

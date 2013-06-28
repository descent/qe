/*
 *  linebuffer.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#include <stdlib.h>
#include <string.h>
#include "linebuffer.h"
#include "misc.h"

linebuffer::linebuffer(void) : qeString() {
    next     = NULL;
    previous = NULL;
}

linebuffer::linebuffer(const char *s) : qeString(s) {
    next     = NULL;
    previous = NULL;
}

linebuffer::linebuffer(const linebuffer& lb) : qeString(lb.str) {
    next     = lb.next;
    previous = lb.previous;
}

linebuffer linebuffer::operator = (const linebuffer& lb) {
    init(lb.str);
    next     = lb.next;
    previous = lb.previous;

    return *this;
}

linebuffer linebuffer::operator = (const char *s) {
    qeString::operator = (s);

    return *this;
}

linebuffer linebuffer::operator = (const qeString& qs) {
    qeString::operator = (qs);

    return *this;
}

linebuffer::~linebuffer(void) {
    next     = NULL;
    previous = NULL;
}

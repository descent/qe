/*
 *  linebuffer.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#ifndef __QE_LINEBUFFER_H_
#define __QE_LINEBUFFER_H_

#include "qestring.h"

class linebuffer : public qeString {
protected:
//  char       *str;
//  int        len;
//  int        buflen;
//  void       init(const char *s);

public:
    linebuffer *next;
    linebuffer *previous;

    linebuffer(void);
    linebuffer(const char *s);
    linebuffer(const qeString&	qs);
    linebuffer(const linebuffer& lb);

   ~linebuffer(void);

//  unsigned int  getLength(void);
//  char *	  getString(void);
//  char *	  getString(const int idx);


//  void assign(const char *s);
    linebuffer operator = (const linebuffer& lb);
    linebuffer operator = (const qeString& qs);
    linebuffer operator = (const char *s);
//  char& operator [] (const int idx);
};

#endif

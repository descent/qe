/*
 *  qestring.cc
 *
 *  Copyright (C) 1998,1999  Jiann-Ching Liu
 */


#include <stdlib.h>
#include <string.h>
#include "qestring.h"

void  (*qeString::handler)(void) = (void (*)(void)) NULL;

qeString::qeString(void) {
    str = NULL;
    len = buflen = 0;
}

qeString::qeString(const char *s) {
    str = NULL;
    len = buflen = 0;

    init(s);
}

qeString::qeString(const qeString& qs) {
    str = NULL;
    len = buflen = 0;

    init(qs.str);
}

void qeString::init(const char *s) {


    int slen = 0;

    if (s != NULL) slen = strlen(s);

    if (buflen <= slen) {
	char *ptr = new char[slen + 1];

	if (ptr != NULL) {
	    if (str != NULL) delete str;
	    buflen = (len = slen) + 1;
	    str = ptr;
	} else {
	    // memory allocation error
	    if (handler != NULL) (*handler)();
	    return;
	}
    }

    if (s == NULL) {
	str[0] = '\0';
    } else {
	memcpy(str, s, len+1);
    }
}

void qeString::setHandler(void (*f)(void)) {
    handler = f;
}

qeString qeString::operator = (const qeString& qs) {
    init(qs.str);

    return *this;
}

qeString qeString::operator = (const char *s) {
    init(s);
    return *this;
}

qeString::~qeString(void) {
    if (str != NULL) delete str;
    str = NULL;
    buflen = len = 0;
}

//  void qeString::assign(const char *s) {
//	init(s);
//  }

unsigned int qeString::getLength(void) {
    return len;
}

void qeString::setLength(void) {
    len = ::strlen(str);
    buflen = len + 1;
}

char * qeString::getString(void) {
    return str;
}

char * qeString::getString(const int idx) {
    return &str[idx];
}

char& qeString::operator [] (const int idx) {
    return str[idx];
}

qeString::operator char * (void) {
    return str;
}

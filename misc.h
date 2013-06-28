/*
 *  misc.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#ifndef __QE_MISC_H_
#define __QE_MISC_H_

#include "config.h"

char *basename(char *pathname);
char *qeStrdup(const char *str);
void qeFree(char *ptr);

// char * decode_base64(const char *str);

#endif

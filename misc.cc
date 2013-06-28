/*
 *  misc.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "misc.h"


char *basename(char *pathname) {
    char  *str;

    if ((str = strrchr(pathname, '/')) != NULL) {
	str = str + 1;
    } else {
	str = pathname;
    }

    return str;
}

char *qeStrdup(const char *str) {
    char  *ptr;

    if (str == NULL) {
	ptr = new char;
	ptr[0] = '\0';
    } else {
	int len = strlen(str) + 1;
	if ((ptr = new char[len]) != NULL) {
	    memcpy(ptr, str, len);
	} else {
	    // memory allocation error
	}
    }

    return ptr;
}

void qeFree(char *str) {
    if (str != NULL) delete str;
    str = NULL;
}


// char * decode_base64(const char *str) {
//     static char buffer[MAX_BUFFER_LEN+1];
//     int	      x, len, j, k;
//     unsigned long  ch;
//
//     len = strlen(str);
//
//     ch = 0L;
//     for (int i = j = k = 0; i < len; i++) {
//	   if (str[i] >= 'A' && str[i] <= 'Z') {
//	       x = str[i] - 'A';
//	   } else if (str[i] >= 'a' && str[i] <= 'z') {
//	       x = str[i] - 'a' + 26;
//	   } else if (str[i] >= '0' && str[i] <= '9') {
//	       x = str[i] - '0' + 52;
//	   } else if (str[i] == '-') {
//	       x = 62;
//	   } else if (str[i] == '+') {
//	       x = 63;
//	   } else {
//	       x = 0;
//	   }
//
//	   ch = (ch << 6) + x;
//
//	   if (k++ == 3) {
//	       buffer[j++] = char((ch >> 16) & 0xFF);
//	       buffer[j++] = char((ch >> 8)  & 0xFF);
//	       buffer[j++] = char(ch & 0xFF);
//	       ch = 0L;
//	       k = 0;
//	   }
//     }
//     buffer[j] = '\0';
//
//     return buffer;
// }

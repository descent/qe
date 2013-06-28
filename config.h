/*
 *  config.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#ifndef __QE_CONFIG_H_
#define __QE_CONFIG_H_

#if defined(__FreeBSD__) || defined(FREEBSD)
#    include <ncurses.h>
#else
#    include <curses.h>
#    define AUTO_TERM_SIZE_DETECT    1
#endif


#ifndef MAX_BUFFER_LEN
#define MAX_BUFFER_LEN     (512)
#endif


// #define USE_ANSICOLOR

#ifndef DEFAULT_PROFILE
#define DEFAULT_PROFILE "/usr/local/etc/qe.pro"
#endif

#ifndef DEFAULT_USER_PROFILE
#define DEFAULT_USER_PROFILE ".qerc"
#endif

#endif

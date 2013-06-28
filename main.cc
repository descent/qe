/*
 *  main.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include "config.h"
#include "version.h"
#include "dirbuffer.h"
#include "filebuffer.h"

void usage(void);

int main(int argc, char *argv[])
{
    struct passwd   *pw;
    filebuffer	    *f;
    int 	    c, errflag = 0;
    char	    *profile = NULL;


    while ((c = getopt(argc, argv, "hp:t:")) != EOF) {
	char  *envbuff;

	switch (c) {
	case 't':
	    // putenv("TERM=xterm-color");
	    // putenv("TERM=color_xterm");
	    envbuff = new char[strlen(optarg) + 6];
	    sprintf(envbuff, "TERM=%s", optarg);
	    putenv(envbuff);
	    delete envbuff;
	    break;
	case 'p':
	    profile = optarg;
	    break;
	case 'h':
	default:
	    errflag++;
	    break;
	}
    }

    if (errflag) usage();


    if (profile == NULL) {
	if ((pw = getpwuid(getuid())) != NULL) {
	    char   filename[500];
	    sprintf(filename, "%s/%s", pw->pw_dir, DEFAULT_USER_PROFILE);
	    filebuffer::init_keydefine(filename);
	} else {
	    filebuffer::init_keydefine();
	}
    } else {
	filebuffer::init_keydefine(profile);
    }

    filebuffer::init_curses();

    if (optind < argc) {
	struct stat   stbuf;

	if (stat(argv[1], &stbuf) == 0 && S_ISDIR(stbuf.st_mode)) {
	    f = new dirbuffer(argv[optind]);
	} else {
	    f = new filebuffer(argv[optind]);
	}
    } else {
	f = new dirbuffer();
    }

    while ((f = f->active()) != NULL);
    filebuffer::end_curses();

    printf("%c[0m\n\n", 27);

//  printf("Thanks for using QE v" QE_VERSION "\n\n"
//	   "Get newest QE source from:\n"
//	   "http://www.cc.ncu.edu.tw/~center5/product/qe/\n\n"
//	   "Email: center5@cc.ncu.edu.tw (Jiann-Ching Liu)\n\n");

    return 0;
}

void usage(void) {
    fprintf(stderr, "usage: qe [-t term] [-p profile] [-h] [filename|directory]\n");
    exit(1);
}

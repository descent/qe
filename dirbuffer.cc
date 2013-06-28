/*
 *  dirbuffer.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <pwd.h>
#include "config.h"
#include "colors.h"
#include "dirbuffer.h"
#include "keymap.h"
#include "misc.h"
#include "messages.h"

static time_t  current_time;

static char *monthstring[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char *timestring(const time_t  tm) {
    static char  buffer[20];
    struct tm	 *tp;

// struct tm
// {
//	   int	   tm_sec;	   /* seconds */
//	   int	   tm_min;	   /* minutes */
//	   int	   tm_hour;	   /* hours */
//	   int	   tm_mday;	   /* day of the month */
//	   int	   tm_mon;	   /* month */
//	   int	   tm_year;	   /* year */
//	   int	   tm_wday;	   /* day of the week */
//	   int	   tm_yday;	   /* day in the year */
//	   int	   tm_isdst;	   /* daylight saving time */
// };

    tp = localtime(&tm);
    if (tm + 86400L * 365 > current_time) {
	sprintf(buffer, "%s %2d %02d:%02d", monthstring[tp->tm_mon],
		   tp->tm_mday, tp->tm_hour, tp->tm_min);
    } else {
	sprintf(buffer, "%s %2d  %04d", monthstring[tp->tm_mon],
		   tp->tm_mday, 1900 + tp->tm_year);
    }
    return buffer;
}

filelist::filelist(void) : filename() {
    next     = NULL;
}

filelist::filelist(const char *fname, const struct stat *stbuf)
							 : filename(fname){
    mode     = stbuf->st_mode;
    uid      = stbuf->st_uid;
    size     = stbuf->st_size;
    mtime    = stbuf->st_mtime;
    next     = NULL;
}

filelist::~filelist(void) {
    filename.~qeString();
    if (next != NULL) {
	 next->~filelist();
	 delete next;
    }
    next = NULL;
}


static char *rwxmode(const mode_t  stmod) {
    static char  buffer[12];

    buffer[0] = S_ISDIR(stmod)	? 'd' : S_ISREG(stmod)	? '-' :
		S_ISCHR(stmod)	? 'c' : S_ISBLK(stmod)	? 'b' :
		S_ISFIFO(stmod) ? 'f' : S_ISSOCK(stmod) ? 's' :
		S_ISLNK(stmod)	? 'l' : '?';

    int m = int(stmod & 0777);

    for (int i = 1; i <= 9; i++, m <<= 1)
	buffer[i] = ((m & 0400) == 0) ? '-' : ("rwx"[(i-1) % 3]);

    buffer[10] = '\0';
    return buffer;
}


dirbuffer::dirbuffer(void) : filebuffer(), pathname() {
    init_dirbuffer();

    refreshdir(current_dir);
}

dirbuffer::dirbuffer(const char *path) : filebuffer(), pathname() {
    init_dirbuffer();

    if (path != NULL && path[0] != '\0') {
	refreshdir(path);
    } else {
	refreshdir(current_dir);
    }
}

void dirbuffer::init_dirbuffer(void) {
    buffer_type    = DIRECTORY_BUFFER;
    read_only	   = 1;
    pathok	   = 0;
    flist.next	   = NULL;
    loadingOK	   = 1;

    filename = "<< dir >>";
}

dirbuffer::~dirbuffer(void) {
    pathname.~qeString();
    flist.~filelist();
}

int dirbuffer::cmd_save(void) {
    display_messages(MSG_CANT_SAVE_INTERNAL);
    return 0;
}

int dirbuffer::do_function_key(const int key) {
    filelist   *fptr = &flist;

    switch (key) {
    case KEY_ENTER:
    case KEY_RETURN:
    case KEY_LINEFEED:
	if (pathok) {
	    int  lnktype = 0;

	    for (int i = 0; i <= buffer_y; i++) {
		fptr = fptr->next;
	    }

	    if (S_ISLNK(fptr->mode)) {
		struct stat  stbuf;
		stat(fptr->filename.getString(), &stbuf);

		if (S_ISREG(stbuf.st_mode)) {
		    lnktype = 1;
		} else if (S_ISDIR(stbuf.st_mode)) {
		    lnktype = 2;
		}
	    }

	    if (S_ISREG(fptr->mode) || lnktype == 1) {
		return cmd_edit(fptr->filename.getString());
	    } else if (S_ISDIR(fptr->mode) || lnktype == 2) {
		if (access(fptr->filename.getString(), X_OK) == 0 &&
		    access(fptr->filename.getString(), R_OK) == 0) {
		    return cmd_dir(fptr->filename.getString());
		} else {
		    beep();
		    display_messages(MSG_PERMISSION_DENY);
		}
	    }
	}
	break;
    default:
	return getnextkey();
    }
    return 0;
}

int dirbuffer::refreshdir(const char *path) {
    DIR 	     *dirp;
    struct dirent    *direntp;
    linebuffer	     *tmpptr, *ptr;
    char	     linebuf[MAX_BUFFER_LEN+PATH_MAX];
    struct stat      stbuf;
    filelist	     *fptr = &flist;


    time(&current_time);

    unmark_all_mark();
    general_init();

    if (path[0] == '/') {
	pathname = path;
    } else {
	sprintf(linebuf, "%s/%s", current_dir, path);
	pathname = linebuf;
    }

    if (lstat(pathname.getString(), &stbuf) == 0 && S_ISDIR(stbuf.st_mode)) {
	chdir(pathname.getString());
	getcwd(linebuf, PATH_MAX);
	chdir(current_dir);
	pathname = linebuf;
    }

    sprintf(linebuf, "[ %s ]", pathname.getString());
    filename = linebuf;

    if ((dirp = opendir(pathname.getString())) != NULL) {

	for (ptr = head->next; ptr != tail; ptr = current) {
	    current = ptr->next;
	    ptr->~linebuffer();
	    delete ptr;
	}

	ptr = current = head;

	while ((direntp = readdir(dirp)) != NULL ) {
	    sprintf(linebuf, "%s/%s", pathname.getString(), direntp->d_name);
	    lstat(linebuf, &stbuf);

	    int  found = 0;
	    for (fptr = &flist; fptr->next != NULL; fptr = fptr->next) {
		if (S_ISDIR(fptr->next->mode)) {
		    if (! S_ISDIR(stbuf.st_mode)) continue;

		    if (strcmp(basename(fptr->next->filename.getString()), ".") == 0)
			continue;

		    if (strcmp(direntp->d_name, ".") == 0) break;

		    if (strcmp(basename(fptr->next->filename.getString()), "..") == 0)
			continue;

		    if (strcmp(direntp->d_name, "..") == 0) break;

		} else if (S_ISDIR(stbuf.st_mode)) {
		    break;
		}

		switch (dir_sort) {
		case DIR_SORT_BY_SIZE:
		    if (fptr->next->size >= stbuf.st_size)
			found = 1;
		    break;
		case DIR_SORT_BY_TIME:
		    if (fptr->next->mtime >= stbuf.st_mtime)
			found = 1;
		    break;
		case DIR_SORT_BY_NAME:
		default:
		    if (strcmp(fptr->next->filename.getString(), linebuf) >= 0)
			found = 1;
		    break;
		}
		if (found) break;
	    }

	    filelist *q = fptr->next;
	    fptr->next	= new filelist(linebuf, &stbuf);
	    fptr->next->next = q;
	}
	closedir(dirp);

	total_line = 0;

	struct passwd *pw;
	char	      *str;

	for (fptr = flist.next; fptr != NULL; fptr = fptr->next) {
	    str = basename(fptr->filename.getString());

	    if ((pw = getpwuid(fptr->uid)) != NULL) {
		sprintf(linebuf, "%s %-10s", rwxmode(fptr->mode), pw->pw_name);
	    } else {
		sprintf(linebuf, "%s %10d", rwxmode(fptr->mode),
							 (int) fptr->uid);
	    }

	    int  len = strlen(linebuf);

	    if (S_ISDIR(fptr->mode)) {
		sprintf(&linebuf[len], " %-9s %s %s", "<DIR>",
					     timestring(fptr->mtime), str);
	    } else if (S_ISLNK(fptr->mode)) {
		char  lnkbuf[128];

		readlink(fptr->filename.getString(), lnkbuf, 127);
		lnkbuf[minimum(127, fptr->size)] = '\0';
		sprintf(&linebuf[len], " %9lu %s %s -> %s",
		     (unsigned long) fptr->size, timestring(fptr->mtime), str,
		     lnkbuf);
	    } else {
		sprintf(&linebuf[len], " %9lu %s %s",
		     (unsigned long) fptr->size, timestring(fptr->mtime), str);
	    }

	    tmpptr	     = new linebuffer(linebuf);
	    ptr->next	     = tmpptr;
	    tmpptr->previous = ptr;
	    ptr 	     = tmpptr;
	    total_line++;
	}

	ptr->next	  = tail;
	tail->previous	  = ptr;
	current 	  = head->next;
	current->previous = head;

	pathok = 1;

	return 0;
    } else {
	sprintf(linebuf, "%s: path not found", pathname.getString());
	// current->assign(linebuf);
	*current = linebuf;
	pathok = 0;
	return 1;
    }
}

void dirbuffer::refresh_clientarea(const int  bflflag) {
    filebuffer::refresh_clientarea(0);
    load_workbuffer();
    refresh_bufferline(1);
}

int dirbuffer::cmd_down(void) {
    refresh_bufferline(0);
    filebuffer::cmd_down();
    refresh_bufferline(1);

    return 0;
}

int dirbuffer::cmd_up(void) {
    refresh_bufferline(0);
    filebuffer::cmd_up();
    refresh_bufferline(1);

    return 0;
}

void dirbuffer::refresh_bufferline(const int  type) {
    filebuffer::refresh_bufferline();
    if (type) {
	setattr(win, ATTRIBUTE_current_selected_text);

	mvprint(cursor_y, 0, ((unsigned int)workbuflen > left_col_no) ?
					   &workbuffer[left_col_no] : "");

	setattr(win, ATTRIBUTE_normal_text);
    }
}

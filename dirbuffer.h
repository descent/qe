/*
 *  dirbuffer.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#ifndef __QE_DIRBUFFER_H_
#define __QE_DIRBUFFER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include "config.h"
#include "filebuffer.h"
#include "mark.h"

class filelist {
public:
    qeString  filename;
    mode_t    mode;
    uid_t     uid;
    off_t     size;
    time_t    mtime;
    filelist  *next;

    filelist(void);
    filelist(const char *fname, const struct stat *stbuf);
   ~filelist(void);
};

class dirbuffer : public filebuffer {
protected:
    int       pathok;
//  char      *pathname;
    qeString  pathname;
    filelist  flist;
public:
    dirbuffer(void);
    dirbuffer(const char *path);
   ~dirbuffer(void);

protected:
    void     init_dirbuffer(void);

private:
    int      refreshdir(const char *path);

protected:
    virtual int      do_function_key(const int	key);
    virtual void     refresh_clientarea(const int  bflflag);
    virtual void     refresh_bufferline(const int  type);

public:
    virtual int      cmd_save(void);
    virtual int      cmd_up(void);
    virtual int      cmd_down(void);
};

#endif

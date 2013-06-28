/*
 *  qestring.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#ifndef __QE_QESTRING_H_
#define __QE_QESTRING_H_

class qeString {
protected:
    char	 *str;
    int 	 len;
    int 	 buflen;
    void	 init(const char *s);
    static void  (*handler)(void);
public:

    qeString(void);
    qeString(const char *s);
    qeString(const qeString&  qs);

   ~qeString(void);

    static void   setHandler(void (*)(void));

    unsigned int  getLength(void);
    void	  setLength(void);
    char *	  getString(void);
    char *	  getString(const int idx);

    qeString operator = (const qeString& lb);
    qeString operator = (const char *s);
    char& operator [] (const int idx);
    operator char * (void);
};

#endif

/*
 *  menu.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#ifndef __QE_MENU_H_
#define __QE_MENU_H_

#include "config.h"

class filebuffer;

class menu_list {
protected:
    char	*title;
    char*	(filebuffer::*getTitle)(const char *);
    int 	(filebuffer::*action)(void);
    menu_list	*next;
    int 	keyidx;
public:
    menu_list(void);
    menu_list(const char *name, int (filebuffer::*function)(void));
    menu_list(const char *name, int (filebuffer::*function)(void),
				      char *(filebuffer::*tfcn)(const char *));
   ~menu_list(void);

    friend class menu_bar;
    friend class menu;

protected:
    void init_menu_list(void);
    void init_menu_list(const char *name, int (filebuffer::*function)(void));
};


class menu_bar {
protected:
    int 	id;
    int 	width;
    int 	height;
    int 	sel;
    char	*title;
    menu_list	*ml;
    menu_bar	*next;
    menu_bar	*previous;
public:
    menu_bar(void);
    menu_bar(const int num, const char *name);
   ~menu_bar(void);

    int 	add_item(const char *name, int (filebuffer::*action)(void),
		 char *(filebuffer::*tfcn)(const char *) = NULL);

    friend class menu;
protected:
    void	init_menu_bar(void);
};

class menu {
protected:
    menu_bar  *list;
    WINDOW    *mw;
    char      bar[MAX_BUFFER_LEN+1];
    int       assigned;
    int       screen_y, screen_x;
    char      fmt[10];
    int       (filebuffer::*function)(void);
public:
    menu(void);
   ~menu(void);

    int active(WINDOW *win, const int y, const int x, filebuffer *fb);

    int add_menu(const char *name);
    int add_item(const int mid, const char *name, int (filebuffer::*f)(void),
		 char *(filebuffer::*tfcn)(const char *) = NULL);

    int add_item(const char *mt, const char *name, int (filebuffer::*f)(void),
		 char *(filebuffer::*tfcn)(const char *) = NULL);

    int del_item(const int mid, const char *name);
    int del_item(const char *mt, const char *name);
protected:
    void     show_title_bar(void);
    void     show_list(WINDOW*& win, menu_bar *mb, int& sel, filebuffer *fb, const int up = 0);
};

#endif

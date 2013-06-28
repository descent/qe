# Makefile

CC=	g++

# CFLAGS=-Wall -O2
CFLAGS=-Wall -O2 -DHAS_VSNPRINTF -I/usr/include/ncurses

## Linux
# CFLAGS=-Wall -O2 -DHAS_VSNPRINTF -I/usr/include/ncurses

## Solaris / IRIX
# CFLAGS=-Wall -O2

## FreeBSD
# CFLAGS=-Wall -O2 -DFREEBSD -DHAS_VSNPRINTF

## HP-UX
# DEFAULT_PROFILE=$(INSTDIR)/lib
# CFLAGS=-Wall -O2 -DDEFAULT_PROFILE=\"$(DEFAULT_PROFILE)\" -I/usr/include/ncurses

LFLAGS= -lncurses


SRC=	main.cc filebuffer.cc fbcmd.cc fbcmdln.cc fbgetch.cc fbcmdmove.cc \
	fbcmddel.cc fbcmdmark.cc setcolor.cc linebuffer.cc keycmd.cc \
	fbquery.cc fbcmdextra.cc fbcmdconv.cc keydefine.cc keyqueue.cc \
	dirbuffer.cc mark.cc fbcmdfind.cc fbmenu.cc qestring.cc misc.cc \
	messages.cc menu.cc history.cc about.cc

OBJ=	$(SRC:.cc=.o)
INSTDIR=/usr/local

.cc.o:
	$(CC) $(CFLAGS) -c -o $*.o $<

all:	qe

qe:	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

clean:
	rm -f qe $(OBJ) core

install:
	[ -d $(INSTDIR)/bin ] || mkdir $(INSTDIR)/bin
	[ -d $(INSTDIR)/etc ] || mkdir $(INSTDIR)/etc
	cp -f qe $(INSTDIR)/bin
	cp -f qe.pro qe.hlp $(INSTDIR)/etc
	strip $(INSTDIR)/bin/qe

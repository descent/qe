/*
 *  keycmd.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */


#ifndef __QE_KEYCMD_H_
#define __QE_KEYCMD_H_

#include "keymap.h"
#include "filebuffer.h"

#define QE_CMD_NULL	  0
#define QE_CMD_STRING	  1
#define QE_CMD_CMDQUOTE   2
#define QE_CMD_MENU	  3
#define QE_CMD_LEFT	  4
#define QE_CMD_RIGHT	  5
#define QE_CMD_UP	  6
#define QE_CMD_DOWN	  7
#define QE_CMD_CC	  8
#define QE_CMD_CA	  9
#define QE_CMD_BL	 10
#define QE_CMD_EL	 11
#define QE_CMD_CM	 12
#define QE_CMD_MB	 13
#define QE_CMD_ML	 14
#define QE_CMD_MC	 15
#define QE_CMD_UM	 16
#define QE_CMD_PS	 17
#define QE_CMD_PO	 18
#define QE_CMD_FN	 19
#define QE_CMD_CT	 20
#define QE_CMD_PGUP	 21
#define QE_CMD_PGDN	 22
#define QE_CMD_EX	 23
#define QE_CMD_CO	 24
#define QE_CMD_MM	 25
#define QE_CMD_E	 26
#define QE_CMD_BM	 27
#define QE_CMD_DL	 28
#define QE_CMD_EE	 29
#define QE_CMD_DM	 30
#define QE_CMD_WE	 31
#define QE_CMD_WB	 32
#define QE_CMD_IL	 33
#define QE_CMD_QUIT	 34
#define QE_CMD_SAVE	 35
#define QE_CMD_RO	 36
#define QE_CMD_DC	 37
#define QE_CMD_UNDO	 38
#define QE_CMD_IT	 39
#define QE_CMD_TE	 40
#define QE_CMD_BE	 41
#define QE_CMD_TAB	 42
#define QE_CMD_TOP	 43
#define QE_CMD_BOTTOM	 44
#define QE_CMD_SP	 45
#define QE_CMD_JO	 46
#define QE_CMD_IM	 47
#define QE_CMD_RM	 48
#define QE_CMD_EB	 49
#define QE_CMD_SCUP	 50
#define QE_CMD_SCDOWN	 51
#define QE_CMD_SCLEFT	 52
#define QE_CMD_SCRIGHT	 53
#define QE_CMD_BEEP	 54
#define QE_CMD_FM	 55
#define QE_CMD_EM	 56
#define QE_CMD_OB	 57
#define QE_CMD_EDIT	 58
#define QE_CMD_CG	 59
#define QE_CMD_SL	 60
#define QE_CMD_SR	 61
#define QE_CMD_CL	 62
#define QE_CMD_BW	 63
#define QE_CMD_TW	 64
#define QE_CMD_QBCP	 65
#define QE_CMD_QSCS	 66
#define QE_CMD_QCX	 67
#define QE_CMD_QCK	 68
#define QE_CMD_CF	 69
#define QE_CMD_KL	 70
#define QE_CMD_LC	 71
#define QE_CMD_UC	 72
#define QE_CMD_QCQ	 73
#define QE_CMD_SEARCH	 74
#define QE_CMD_DIR	 75
#define QE_CMD_CK	 76
#define QE_CMD_QB5	 77
#define QE_CMD_HISTORY	 78
#define QE_CMD_FILEALL	 79
#define QE_CMD_QUITALL	 80
#define QE_CMD_BASE64	 81


class keycmd {
public:
    int     cmd;
    char    *str;
    int     (filebuffer::*action)(void);
    keycmd  *next;
};

class keycmdlist {
public:
    keycmd  *list;

    keycmdlist(const char *cmdlist);
   ~keycmdlist(void);
};

extern keycmdlist *keydef[];

int init_keycmd(void);


struct command_mapping {
    char   *command;
    int    (filebuffer::*action)(void);
    int    cmd;
};

extern struct command_mapping cmdmap[];

#endif

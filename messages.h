/*
 *  messages.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#ifndef __QE_MESSAGES_H_
#define __QE_MESSAGES_H_


#define MSG_BLOCK_MARK_REQ	   0
#define MSG_CANT_SAVE_INTERNAL	   1
#define MSG_CHANGE_IT		   2
#define MSG_CONFIRM_CHANGE	   3
#define MSG_CONFLICT_EXIST_MARK    4
#define MSG_REALLY_QUIT 	   5
#define MSG_ERROR_OPEN_FILE	   6
#define MSG_READ_ONLY_FILE	   7
#define MSG_FILE_SAVED		   8
#define MSG_MESSING_FILE_NAME	   9
#define MSG_NAME_IN_USE 	  10
#define MSG_NEW_FILE		  11
#define MSG_NO_CHANGE		  12
#define MSG_NO_MARKED_AREA	  13
#define MSG_NO_MARK_SAVED	  14
#define MSG_NO_MATCH_FOUND	  15
#define MSG_NO_MATCH_CTRL_G	  16
#define MSG_OUT_OF_MEMORY	  17
#define MSG_PERMISSION_DENY	  18
#define MSG_QUIT		  19
#define MSG_WRITE_FORCE 	  20
#define MSG_READ_ERROR		  21
#define MSG_SEARCH_WRAPPED	  22
#define MSG_S_T_CONFLICT	  23
#define MSG_TOO_MANY_MARK_SAVE	  24
#define MSG_TYPE_A_CHARACTER	  25
#define MSG_UNKNOW_COMMAND	  26
#define MSG_MENU_USAGE		  27
#define MSG_CMD_HISTORY 	  28



extern const char *chinese_message[];
extern const char *english_message[];

#endif

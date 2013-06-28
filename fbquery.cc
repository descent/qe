/*
 *  fbquery.cc
 *
 *  Copyright (C) 1998, 1999  Jiann-Ching Liu
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filebuffer.h"
#include "keyqueue.h"

int filebuffer::setenviron(const char  *string) {
    char  cmd[MAX_BUFFER_LEN+1];
    char  arg1[MAX_BUFFER_LEN+1];
    char  arg2[MAX_BUFFER_LEN+1];

    sscanf(string, "%s %s %s", cmd, arg1, arg2);
    if (strcmp(cmd, "s") == 0 || strcmp(cmd, "set") == 0) {
	if (strcmp(arg1, "blankcompress") == 0) {
	    if (strcmp(arg2, "on") == 0 || strcmp(arg2, "1") == 0) {
		blank_compress = 1;
		return 1;
	    } else if (strcmp(arg2, "off") == 0 || strcmp(arg2, "0") == 0) {
		blank_compress = 0;
		return 1;
	    }
	} else if (strcmp(arg1, "searchcase") == 0) {
	    if (strcmp(arg2, "exact") == 0 || strcmp(arg2, "1") == 0) {
		search_case = 1;
		return 1;
	    } else if (strcmp(arg2, "any") == 0 || strcmp(arg2, "0") == 0) {
		search_case = 0;
		return 1;
	    }
	} else if (strcmp(arg1, "ctrl-k") == 0) {
	    if (strcmp(arg2, "on") == 0 || strcmp(arg2, "1") == 0) {
		enable_ctrl_k = 1;
		return 1;
	    } else if (strcmp(arg2, "off") == 0 || strcmp(arg2, "0") == 0) {
		enable_ctrl_k = 0;
		return 1;
	    }
	} else if (strcmp(arg1, "ctrl-x") == 0) {
	    if (strcmp(arg2, "on") == 0 || strcmp(arg2, "1") == 0) {
		enable_ctrl_x = 1;
		return 1;
	    } else if (strcmp(arg2, "off") == 0 || strcmp(arg2, "0") == 0) {
		enable_ctrl_x = 0;
		return 1;
	    }
	} else if (strcmp(arg1, "ctrl-q") == 0) {
	    if (strcmp(arg2, "on") == 0 || strcmp(arg2, "1") == 0) {
		enable_ctrl_q = 1;
		return 1;
	    } else if (strcmp(arg2, "off") == 0 || strcmp(arg2, "0") == 0) {
		enable_ctrl_q = 0;
		return 1;
	    }
	} else if (strcmp(arg1, "big5") == 0) {
	    if (strcmp(arg2, "on") == 0 || strcmp(arg2, "1") == 0) {
		big5msg_flag = 1;
		// display_status();
		return 1;
	    } else if (strcmp(arg2, "off") == 0 || strcmp(arg2, "0") == 0) {
		big5msg_flag = 0;
		// display_status();
		return 1;
	    } else if (strcmp(arg2, "auto") == 0) {
		char *lang = getenv("LANG");

		big5msg_flag = 0;
		if (lang != NULL && strncasecmp(lang, "zh_TW", 5) == 0) {
		    big5msg_flag = 1;
		}
		return 1;
	    }
	} else if (strcmp(arg1, "history-key") == 0) {
	    if (strcmp(arg2, "none") == 0) {
		history_key = HISTORY_KEY_NONE;
		return 1;
	    } else if (strcmp(arg2, "both") == 0) {
		history_key = HISTORY_KEY_BOTH;
		return 1;
	    } else if (strcmp(arg2, "up") == 0) {
		history_key = HISTORY_KEY_UP;
		return 1;
	    } else if (strcmp(arg2, "down") == 0) {
		history_key = HISTORY_KEY_DOWN;
		return 1;
	    }
	} else if (strcmp(arg1, "dir-sort-by") == 0) {
	    if (strcmp(arg2, "name") == 0) {
		dir_sort = DIR_SORT_BY_NAME;
		return 1;
	    } else if (strcmp(arg2, "time") == 0) {
		dir_sort = DIR_SORT_BY_TIME;
		return 1;
	    } else if (strcmp(arg2, "size") == 0) {
		dir_sort = DIR_SORT_BY_SIZE;
		return 1;
	    }
	}
    }
    return 0;
}

int filebuffer::query_blnkcmprs(void) {
    char buffer[50];

    cmd_cursor_cmd();
    sprintf(buffer, "set blankcompress %s", (blank_compress==1) ? "on" : "off");
    queue.enqueue(buffer);

    return 0;
}

int filebuffer::query_searchcase(void) {
    char buffer[50];

    cmd_cursor_cmd();
    sprintf(buffer, "set searchcase %s", (search_case==1) ? "exact" : "any");
    queue.enqueue(buffer);

    return 0;
}

int filebuffer::query_ctrl_k(void) {
    char buffer[50];

    cmd_cursor_cmd();
    sprintf(buffer, "set ctrl-k %s", (enable_ctrl_k==1) ? "on" : "off");
    queue.enqueue(buffer);

    return 0;
}

int filebuffer::query_ctrl_x(void) {
    char buffer[50];

    cmd_cursor_cmd();
    sprintf(buffer, "set ctrl-x %s", (enable_ctrl_x==1) ? "on" : "off");
    queue.enqueue(buffer);

    return 0;
}

int filebuffer::query_ctrl_q(void) {
    char buffer[50];

    cmd_cursor_cmd();
    sprintf(buffer, "set ctrl-q %s", (enable_ctrl_q==1) ? "on" : "off");
    queue.enqueue(buffer);

    return 0;
}

int filebuffer::query_big5(void) {
    char buffer[50];

    cmd_cursor_cmd();
    sprintf(buffer, "set big5 %s", (big5msg_flag==1) ? "on" : "off");
    queue.enqueue(buffer);

    return 0;
}

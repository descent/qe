/*
 *  fbcmdln.cc
 *
 *  Copyright (C) 1998, 1999  Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "keydefine.h"
#include "filebuffer.h"
#include "dirbuffer.h"
#include "colors.h"
#include "keyqueue.h"
#include "misc.h"
#include "messages.h"

void filebuffer::refresh_cmdarea(void) {
    int  cmdline_left_co = 0;
    char buffer[201];
    sprintf(buffer, "%200s", " ");


    if (cmdline_idx >= screen_x -1)
	cmdline_left_co = cmdline_idx - screen_x + 1;

    int  len = strlen(&cmdline[cmdline_left_co]);

    setattr(win, ATTRIBUTE_command_line);
    mvwaddnstr(win, screen_y-3, 0, &cmdline[cmdline_left_co], screen_x);
    mvwaddnstr(win, screen_y-3, len, buffer, screen_x - len);
}

int filebuffer::cmdarea(void) {
    int  rtlval = 0;

    refresh_cmdarea();
    while (mode == FB_COMMAND_MODE) {
	display_status();
//	display_messages(MSG_CMD_MODE);
	if ((rtlval = cmd_kbinput()) != 0) return rtlval;
    }
    return rtlval;
}

int filebuffer::cmd_dir(void) {
    write_workbuffer(QE_IF_NECESSARY);
    return cmd_dir("");
}

int filebuffer::cmd_dir(const char *path) {
    int 	 found = 0;

    if (current_buffer->buffer_type == DIRECTORY_BUFFER) {
	found = 1;
    } else {
	for (filebuffer *fb = current_buffer->next_buffer;
				  fb != current_buffer; fb = fb->next_buffer) {
	    if (fb->buffer_type == DIRECTORY_BUFFER) {
		current_buffer = fb;
		found = 1;
		break;
	    }
	}
    }

    filebuffer *ptr = new dirbuffer(path);

    if (found) return RTNVAL_QUIT_CURRENT_BUFFER;

    current_buffer = ptr;
    return RTNVAL_NEXT_BUFFER;
}

int filebuffer::cmd_execute(const char	*cmdstr) {
    char  orgcmd[MAX_BUFFER_LEN+1];
    char  cmd[MAX_BUFFER_LEN+1], args[MAX_BUFFER_LEN+1];
    int   retval = 0;
    int   cmderr = 0;

    if (strlen(cmdstr) == 0) return 0;

    strncpy(orgcmd, cmdstr, MAX_BUFFER_LEN);
    cmd[0] = args[0] = '\0';
    sscanf(cmdstr, "%s %s", cmd, args);
//  cmdline[(cmdline_idx = 0)] = '\0';
//  refresh_cmdarea();

    if (strcasecmp(cmd, "quit") == 0 || strcasecmp(cmd, "q") == 0) {
	return cmd_quit();
    } else if (strcasecmp(cmd, "save") == 0) {
	return cmd_save();
    } else if (strcasecmp(cmd, "file") == 0) {
	return cmd_file();
    } else if (strcasecmp(cmd, "shell") == 0) {
	return cmd_shell();
    } else if (strcasecmp(cmd, "dir")  == 0) {
	return cmd_dir(args);
    } else if (strcasecmp(cmd, "edit") == 0 || strcasecmp(cmd, "e") == 0) {
	return cmd_edit(args);
    } else if (strcasecmp(cmd, "read") == 0 || strcasecmp(cmd, "r") == 0) {
	return cmd_edit(args, 1);
    } else if (strcasecmp(cmd, "name") == 0 || strcasecmp(cmd, "n") == 0) {
	int  found = 0;

	filebuffer *f = current_buffer;
	while ((f = f->next_buffer) != current_buffer) {
	    if (f->filename.getLength() != 0 &&
			 strcmp(f->filename.getString(), args) == 0) {
		found = 1;
		break;
	    }
	}

	if (! found) {
	    filename = args;
	    dirty_flag = 1;
	    setattr(win, ATTRIBUTE_modified_file_line);
	    mvwprintw(win, screen_y-2, 0, "%-30s ",  filename.getString());
	} else {
	    display_messages(MSG_NAME_IN_USE);
	}
    } else if (strcasecmp(cmd, "line") == 0) {
	int linenumber = atoi(args) - 1;
	moveto(linenumber, buffer_x);
	cmd_cursor_data();
	display_status();
    } else if (strcasecmp(cmd, "goto") == 0) {
	int y, x;
	sscanf(orgcmd, "%s %d %d", cmd, &y, &x);
	moveto(--y, --x);
	cmd_cursor_data();
	display_status();
    } else if (orgcmd[0] == '[') {
	if (keydef[QE_KEY_TEMP] != NULL) delete keydef[QE_KEY_TEMP];
	keydef[QE_KEY_TEMP] = new keycmdlist(orgcmd);
	if (keydef[QE_KEY_TEMP]->list == NULL) {
	    cmd_cursor_cmd();
	    queue.enqueue(orgcmd);
	    display_messages(MSG_UNKNOW_COMMAND);
	} else {
	    return cmd_execute_key(QE_KEY_TEMP);
	}
//	    for (keycmd *ptr = kl.list; ptr != NULL; ptr = ptr->next) {
//		if (ptr->action != NULL) {
//		    filebuffer_pmfi  member = ptr->action;
//		    retval = (this->*member)();
//		} else if (ptr->str != NULL) {
//		    int len = strlen(ptr->str);
//		    for (int i = 0; i < len; i++) {
//			queue.enqueue(int((unsigned char) ptr->str[i]));
//			cmd_kbinput();
//		    }
//		} else {
//		    display_messages("cmd code: %d", ptr->cmd);
//		}
//	    }
    } else if (strncmp(orgcmd, "d ", 2)==0 || strncmp(orgcmd, "def ", 2)==0) {
	if (keydefine::define_key(orgcmd)) {
	    display_messages("key define: %s", orgcmd);
	}
    } else if (strcmp(cmd, "s") == 0 || strcmp(cmd, "set") == 0) {
	if (setenviron(orgcmd) == 0) cmderr = 1;
    } else if (orgcmd[0] == 'l' || orgcmd[0] == 'L' || orgcmd[0] == '/') {
	int  i, j = 0, len = strlen(orgcmd);
	int  k = orgcmd[0] == '/' ? 1 : 2;
	char match = orgcmd[0] == '/' ? '/' : orgcmd[1];
	args[0] = '\0';

	for (i = k; i < len; i++, j++) {
	    if (orgcmd[i] == match)
		break;
	    args[j] = orgcmd[i];
	}
	args[j] = '\0';

	int  direction = 0, markonly = 0;

	for (; i < len; i++) {	// read flag
	    switch (orgcmd[i]) {
	    case '+':
		direction = 0;
		break;
	    case '-':
		direction = 1;
		break;
	    case 'm':
		markonly = 1;
		break;
	    }
	}

	strcpy(cmdline, orgcmd);
	cmdline_idx = len;
	refresh_cmdarea();
	if (cmd_find(args, direction, markonly)) cmd_cursor_data();
    } else if (orgcmd[0] == 'c' || orgcmd[0] == 'C') {
	int  i, j, len = strlen(orgcmd);
	char arg2[MAX_BUFFER_LEN];

	args[0] = arg2[0] = '\0';

	for (i = 2, j = 0; i < len; i++, j++) {
	    if (orgcmd[i] == orgcmd[1])
		break;
	    args[j] = orgcmd[i];
	}
	args[j] = '\0';

	for (i++, j = 0; i < len; i++, j++) {
	    if (orgcmd[i] == orgcmd[1])
		break;
	    arg2[j] = orgcmd[i];
	}
	arg2[j] = '\0';

	int  direction = 0, markonly = 0, all = 0;

	for (i++; i < len; i++) {  // read flag
	    switch (orgcmd[i]) {
	    case '*':
		all	  = 1;
		break;
	    case '+':
		direction = 0;
		break;
	    case '-':
		direction = 1;
		break;
	    case 'm':
		markonly = 1;
		break;
	    }
	}

	strcpy(cmdline, orgcmd);
	cmdline_idx = len;
	refresh_cmdarea();

	cmd_change(args, arg2, direction, markonly, ! all, 1);
	// .if (cmd_find(args, direction, markonly)) cmd_cursor_data();
    } else if (strcasecmp(cmd, "msg:") == 0) {
	int i, len = strlen(orgcmd);
	for (i = 4; i < len; i++)
	    if (orgcmd[i] != ' ' && orgcmd[i] != '\t')
		break;
	display_messages(&orgcmd[i]);
    } else if (strcasecmp(cmd, "key") == 0) {
	int  k1, k2;

	if (keydefine::get_keycode(args, k1, k2) == 0) {
	    cmderr = 1;
	} else {
	    return cmd_execute_key(k1);
	}
    } else {
	cmderr = 1;
    }

    if (cmderr) {
	queue.enqueue(orgcmd);
	display_messages(MSG_UNKNOW_COMMAND);
    }

    return retval;
}

int filebuffer::cmd_execute(void) {
    char  orgcmd[MAX_BUFFER_LEN+1];

    write_workbuffer(QE_IF_NECESSARY);

    strncpy(orgcmd, cmdline, MAX_BUFFER_LEN);
    cmdline[(cmdline_idx = 0)] = '\0';
    refresh_cmdarea();
    return cmd_execute(orgcmd);
}

int filebuffer::cmd_confirm(void) {
    return 0;
}

int filebuffer::cmd_copyfromcmd(void) {

    write_workbuffer(QE_IF_NECESSARY);

    cmd_cursor_data();
    cmd_insertline();
    strcpy(workbuffer, cmdline);
    workbufidx = workbuflen = buffer_x = cmdline_idx;
    cmdline[(cmdline_idx = 0)] = '\0';
    dirty_line = 1;
    refresh_cmdarea();
    write_workbuffer(QE_FORCE);
    load_workbuffer();
    refresh_bufferline();
    cmd_up();

    return 0;
}

int filebuffer::cmd_copytocmd(void) {

    write_workbuffer(QE_IF_NECESSARY);

    strncpy(cmdline, workbuffer, workbuflen);
    cmdline[(cmdline_idx = workbuflen)] = '\0';
    refresh_cmdarea();
    return 0;
}

int filebuffer::cmd_shell(void) {
    return RTNVAL_SHELL_ESCAPE;
}

int filebuffer::cmd_execute_key(const int key) {
    int 	rtlval = 0;
    keycmdlist	*keycommand;
    keycmd	*ptr = NULL;

    if ((keycommand = kdf->getkeydef(key)) != NULL &&
	(ptr = keycommand->list) != NULL) {
	while (ptr != NULL) {
	    if (ptr->action != NULL) {
		// (int (filebuffer::*)(void))	  member = ptr->action;
		filebuffer_pmfi member = ptr->action;

		if (ptr->cmd == QE_CMD_FM) {
		    if (ptr->next != NULL && ptr->next->cmd == QE_CMD_STRING) {
			if (strlen(ptr->next->str) > 0) {
			    ptr = ptr->next;
			    queue.enqueue(int((unsigned char)ptr->str[0]));
			}
		    }
		}

		rtlval = (this->*member)();
	    } else if (ptr->str != NULL) {
		int len = strlen(ptr->str);

		switch(ptr->cmd) {
		case QE_CMD_CMDQUOTE:
		    cmd_cmdquote(ptr->str);
		    break;
		case QE_CMD_STRING:
		    for (int i = 0; i < len; i++) {
			queue.enqueue(int((unsigned char)ptr->str[i]));
			cmd_kbinput();
		    }
		    // queue.enqueue(ptr->str);
		    break;
		}
	    } else {
		display_messages("cmd code: %d", ptr->cmd);
	    }
	    ptr = ptr->next;
	    display_status();
	}
    } else {
	// display_messages("key code: %d (%o)", key, key);
	display_messages(
  "key code: [ %d ] is not defined, use \"d %d=...\" to define its function.",
	    key, key);
    }
    return rtlval;
}

/*
 *  fbcmdextra.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "filebuffer.h"
#include "messages.h"


int filebuffer::cmd_kill_line(void) {
    if (mode == FB_DATAAREA_MODE) {
	if (read_only) {
	    display_messages(MSG_READ_ONLY_FILE);
	} else {
	    if (buffer_x < workbuflen) {
		return cmd_erase_end();
	    } else {
		workbuflen = buffer_x;
		return cmd_join();
	    }
	}
    } else {
	return cmd_erase_end();
    }
    return 0;
}

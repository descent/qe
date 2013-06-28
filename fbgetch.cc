/*
 *  fbgetch.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "keydefine.h"
#include "filebuffer.h"
#include "colors.h"
#include "keyqueue.h"

#define KEY_BUFFER_QUE_LEN   (4096)

static int	   key;
keyqueue    queue(KEY_BUFFER_QUE_LEN);

int filebuffer::getfirstkey(void) {
    if (queue.isempty()) {
	key = wgetch(win);
	if (message_dirty_flag) display_messages();
	if (key == 127) key = KEY_DC;
    } else {
	key = queue.dequeue();
    }
    return key;
}

int filebuffer::getnextkey(void) {
//  int 	rtlval = 0;
//  keycmdlist	*keycommand;
//  keycmd	*ptr = NULL;

    if (key == KEY_ESC) {
	display_messages("Key: ^[");
	if ((key = wgetch(win)) == '[') {
	      display_messages("Key: ^[[");
	      switch (key = wgetch(win))  {
	      case 'H':
		  key = KEY_HOME;
		  break;
	      case '1':
		  if ((key = wgetch(win)) == '~') {
		      key = KEY_HOME;
		      break;
		  }
		  if (wgetch(win) == '~') {
		      switch(key) {
		      case '1': key = KEY_F(1);  break;
		      case '2': key = KEY_F(2);  break;
		      case '3': key = KEY_F(3);  break;
		      case '4': key = KEY_F(4);  break;
		      case '5': key = KEY_F(5);  break;
		      case '7': key = KEY_F(6);  break;
		      case '8': key = KEY_F(7);  break;
		      case '9': key = KEY_F(8);  break;
		      default : key = KEY_ESC;	 break;
		      }
		  } else {
		      key = KEY_ESC;
		  }
		  break;
	      case '2':
		  if ((key = wgetch(win)) == '~') {
		      key = KEY_IC;
		  } else {
		      if (wgetch(win) == '~') {
			  switch(key) {
			  case '0': key = KEY_F(9);   break;
			  case '1': key = KEY_F(10);  break;
			  case '3': key = KEY_F(11);  break;
			  case '4': key = KEY_F(12);  break;
			  default : key = KEY_ESC;    break;
			  }
		      } else {
			  key = KEY_ESC;
		      }
		  }
		  break;
	      case '3':
	      case '4':
		  key = KEY_ESC;
		  break;
	      case '5':
		  if (wgetch(win) == '~') {
		      key = KEY_PPAGE;
		  } else {
		      key = KEY_ESC;
		  }
		  break;
	      case '6':
		  if (wgetch(win) == '~') {
		      key = KEY_NPAGE;
		  } else {
		      key = KEY_ESC;
		  }
		  break;
	      default:
		  key = KEY_ESC;
		  break;
	      }
	} else if (key == '@') {
	    key = KEY_IC;
	} else if (key == 'O') {
	      switch (key = wgetch(win)) {
	      case 'P':
	      case 'Q':
	      case 'R':
	      case 'S':
		  key = KEY_F(key - 'P' + 1);
		  break;
	      case 't':
	      case 'u':
	      case 'v':
	      case 'l':
		  key = KEY_F(key - 't' + 5);
		  break;
	      case 'w':
		  key = KEY_END;
		  break;
	      case 'x':
	      case 'y':
	      case 'z':
		  key = KEY_F(key - 'w' + 9);
		  break;
	      }
	}
	display_messages();
    } else if (enable_ctrl_k && key == KEY_CTRL('k')) {
	display_messages("Key: ^K");
	if ((key = wgetch(win)) >= '0' && key <= '9') {
	    key = QE_KKEY_START + key - '0';
	} else if (key >= 'A' && key <= 'Z') {
	    key = QE_KKEY_START + key - 'A' + 10;
	} else if (key >= 'a' && key <= 'z') {
	    key = QE_KKEY_START + key - 'a' + 10;
	} else if (key >= KEY_CTRL('a') && key <= KEY_CTRL('z')) {
	    key = QE_KKEY_START + key - KEY_CTRL('a') + 10;
	} else {
	    key = KEY_MAX;
	}
    } else if (enable_ctrl_x && key == KEY_CTRL('x')) {
	display_messages("Key: ^X");
	if ((key = wgetch(win)) >= '0' && key <= '9') {
	    key = QE_XKEY_START + key - '0';
	} else if (key >= 'A' && key <= 'Z') {
	    key = QE_XKEY_START + key - 'A' + 10;
	} else if (key >= 'a' && key <= 'z') {
	    key = QE_XKEY_START + key - 'a' + 10;
	} else if (key >= KEY_CTRL('a') && key <= KEY_CTRL('z')) {
	    key = QE_XKEY_START + key - KEY_CTRL('a') + 10;
	} else {
	    key = KEY_MAX;
	}
    } else if (enable_ctrl_q && key == KEY_CTRL('q')) {
	display_messages("Key: ^Q");
	if ((key = wgetch(win)) >= '0' && key <= '9') {
	    key = QE_QKEY_START + key - '0';
	} else if (key >= 'A' && key <= 'Z') {
	    key = QE_QKEY_START + key - 'A' + 10;
	} else if (key >= 'a' && key <= 'z') {
	    key = QE_QKEY_START + key - 'a' + 10;
	} else if (key >= KEY_CTRL('a') && key <= KEY_CTRL('z')) {
	    key = QE_QKEY_START + key - KEY_CTRL('a') + 10;
	} else {
	    key = KEY_MAX;
	}
    }

    return cmd_execute_key(key);
}

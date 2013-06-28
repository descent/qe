/*
 *  colors.h
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#ifndef __QE_COLORS_H_
#define __QE_COLORS_H_

#include "config.h"

#define C_ATTR(x,y)		     ((x ? A_BOLD : 0) | COLOR_PAIR((y)))


extern chtype attributes[];
int color_setup(void);

#ifdef USE_ANSICOLOR
#   define ATTRIBUTE_normal_text	      0
#   define ATTRIBUTE_mark_text		      1
#   define ATTRIBUTE_command_line	      2
#   define ATTRIBUTE_status_line	      3
#   define ATTRIBUTE_message_line	      4
#   define ATTRIBUTE_modified_file_line       5
#   define ATTRIBUTE_copyright_line	      6
#   define ATTRIBUTE_current_selected_text    7
#   define ATTRIBUTE_find_text		      8
#   define ATTRIBUTE_menu_bar		      9
#   define ATTRIBUTE_menu_bar_select	     10
#   define ATTRIBUTE_menu_border	     11
#   define ATTRIBUTE_menu_item		     12
#   define ATTRIBUTE_menu_item_select	     13
#   define ATTRIBUTE_menu_item_hotkey	     14
#   define ATTRIBUTE_about_box_border	     15
#   define ATTRIBUTE_about_box_text	     16
#   define ATTRIBUTE_cmdhistory_border	     17
#   define ATTRIBUTE_cmdhistory_text	     18
#   define ATTRIBUTE_cmdhistory_select	     19
#   define ATTRIBUTE_goto_text		     20
#else
#   define ATTRIBUTE_normal_text	     attributes[0]
#   define ATTRIBUTE_mark_text		     attributes[1]
#   define ATTRIBUTE_command_line	     attributes[2]
#   define ATTRIBUTE_status_line	     attributes[3]
#   define ATTRIBUTE_message_line	     attributes[4]
#   define ATTRIBUTE_modified_file_line      attributes[5]
#   define ATTRIBUTE_copyright_line	     attributes[6]
#   define ATTRIBUTE_current_selected_text   attributes[7]
#   define ATTRIBUTE_find_text		     attributes[8]
#   define ATTRIBUTE_menu_bar		     attributes[9]
#   define ATTRIBUTE_menu_bar_select	     attributes[10]
#   define ATTRIBUTE_menu_border	     attributes[11]
#   define ATTRIBUTE_menu_item		     attributes[12]
#   define ATTRIBUTE_menu_item_select	     attributes[13]
#   define ATTRIBUTE_menu_item_hotkey	     attributes[14]
#   define ATTRIBUTE_about_box_border	     attributes[15]
#   define ATTRIBUTE_about_box_text	     attributes[16]
#   define ATTRIBUTE_cmdhistory_border	     attributes[17]
#   define ATTRIBUTE_cmdhistory_text	     attributes[18]
#   define ATTRIBUTE_cmdhistory_select	     attributes[19]
#   define ATTRIBUTE_goto_text		     attributes[20]
#endif

#endif

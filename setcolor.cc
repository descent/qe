#include <stdio.h>
#include "config.h"
#include "colors.h"

#define ATTRIBUTE_COUNT        21

chtype attributes[] =
{
    A_NORMAL,		       /* normal  text	     */
    A_REVERSE,		       /* mark	  text	     */
    A_REVERSE,		       /* command line	     */
    A_BOLD,		       /* status  line	     */
    A_DIM,		       /* message line	     */
    A_UNDERLINE,	       /* file (modified)    */
    A_NORMAL,		       /* copyright	     */
    A_UNDERLINE,	       /* current select     */
    A_REVERSE,		       /* fine text	     */
    A_REVERSE,		       /* menu bar	     */
    A_NORMAL,		       /* menu bar select    */
    A_BOLD,		       /* menu border	     */
    A_NORMAL,		       /* menu item	     */
    A_REVERSE,		       /* menu item select   */
    A_REVERSE,		       /* menu item hotkey   */
    A_REVERSE,		       /* about box border   */
    A_NORMAL,		       /* about box text     */
    A_NORMAL,		       /* cmd history_border */
    A_NORMAL,		       /* cmd history_text   */
    A_REVERSE,		       /* cmd history_select */
    A_NORMAL		       /* goto_text	     */
};




int color_table[][3] =
{
    { COLOR_WHITE,   COLOR_BLUE,    FALSE },	 /* normal  text       */
//  { COLOR_WHITE,   COLOR_RED,     TRUE  },
    { COLOR_BLACK,   COLOR_YELLOW,  TRUE  },	 /* mark    text       */
    { COLOR_YELLOW,  COLOR_GREEN,   TRUE  },	 /* command line       */
    { COLOR_WHITE,   COLOR_BLACK,   TRUE  },	 /* status  line       */
    { COLOR_RED,     COLOR_BLACK,   TRUE  },	 /* message line       */
    { COLOR_RED,     COLOR_BLACK,   TRUE  },	 /* file (modified)    */
    { COLOR_YELLOW,  COLOR_BLACK,   TRUE  },	 /* copyright	       */
    { COLOR_BLACK,   COLOR_WHITE,   TRUE  },	 /* current select     */
    { COLOR_BLACK,   COLOR_WHITE,   TRUE  },	 /* fine text	       */
    { COLOR_WHITE,   COLOR_RED,     TRUE  },	 /* menu bar	       */
    { COLOR_BLACK,   COLOR_WHITE,   TRUE  },	 /* menu bar select    */
    { COLOR_CYAN,    COLOR_GREEN,   TRUE  },	 /* menu border        */
    { COLOR_BLACK,   COLOR_GREEN,   TRUE  },	 /* menu item	       */
    { COLOR_WHITE,   COLOR_RED,     TRUE  },	 /* menu item select   */
    { COLOR_WHITE,   COLOR_GREEN,   TRUE  },	 /* menu item hotkey   */
    { COLOR_YELLOW,  COLOR_CYAN,    TRUE  },	 /* about box border   */
    { COLOR_BLUE,    COLOR_WHITE,   TRUE  },	 /* about box text     */
    { COLOR_YELLOW,  COLOR_CYAN,    TRUE  },	 /* cmd history_border */
    { COLOR_BLACK,   COLOR_CYAN,    TRUE  },	 /* cmd history_text   */
    { COLOR_WHITE,   COLOR_RED,     TRUE  },	 /* cmd history_select */
    { COLOR_BLACK,   COLOR_WHITE,   TRUE  }	 /* goto_text	       */
};


// COLOR_BLACK	   0
// COLOR_RED	   1
// COLOR_GREEN	   2
// COLOR_YELLOW    3
// COLOR_BLUE	   4
// COLOR_MAGENTA   5
// COLOR_CYAN	   6
// COLOR_WHITE	   7

int color_setup(void)
{
    int i;

    if (has_colors ()) {	/* Terminal supports color? */
	start_color ();

	/* Initialize color pairs */
	for (i = 0; i < ATTRIBUTE_COUNT; i++)
	    init_pair (i + 1, color_table[i][0], color_table[i][1]);

	/* Setup color attributes */
	for (i = 0; i < ATTRIBUTE_COUNT; i++)
	    attributes[i] = C_ATTR (color_table[i][2], i + 1);

	return 1;
    }
    return 0;
}

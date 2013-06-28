#ifndef __QE_KEYMAP_H_
#define __QE_KEYMAP_H_

#include "config.h"

#ifndef KEY_ESC
#define KEY_ESC 	(27)
#endif

#ifndef KEY_RETURN
#define KEY_RETURN	(13)
#endif

#ifndef KEY_LINEFEED
#define KEY_LINEFEED	(10)
#endif

#ifndef KEY_TAB
#define KEY_TAB 	(9)
#endif

#ifndef KEY_PGUP
#define KEY_PGUP	(339)
#endif

#ifndef KEY_PGDN
#define KEY_PGDN	(338)
#endif

#ifndef KEY_PADEND
#define KEY_PADEND	(348)
#endif

// #ifndef KEY_BACKSPACE
// #define KEY_BACKSPACE   (8)
// #endif

#define KEY_META(x)	(int(x))
#define KEY_CTRL(x)	(int((x)-'a')+1)
#define ESCAPE(x)	(x)

#define QE_KEY_TEMP	(KEY_MAX + 1)
#define QE_UKEY_START	(KEY_MAX + 2)
#define QE_XKEY_START	(QE_UKEY_START + 26)
#define QE_KKEY_START	(QE_XKEY_START + 26 + 10)
#define QE_QKEY_START	(QE_KKEY_START + 26 + 10)
#define QE_KEY_MAX	(QE_QKEY_START + 26 + 10 + 1)

#endif

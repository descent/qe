#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "keymap.h"
#include "keydefine.h"
#include "filebuffer.h"

struct {
    char *key;
    int  first;
    int  second;
} kstr[] = {
    { "up"	    , KEY_UP	     , -1	    },
    { "down"	    , KEY_DOWN	     , -1	    },
    { "left"	    , KEY_LEFT	     , -1	    },
    { "right"	    , KEY_RIGHT      , -1	    },
    { "pgup"	    , KEY_PPAGE      , -1	    },
    { "pgdn"	    , KEY_NPAGE      , -1	    },
    { "home"	    , KEY_HOME	     , int('H')     },
    { "end"	    , KEY_END	     , KEY_PADEND   },
    { "enter"	    , KEY_RETURN     , KEY_LINEFEED },
    { "backspace"   , KEY_BACKSPACE  , '\b'	    },
    { "c-backspace" , 25	     , -1	    },
    { "tab"	    , KEY_TAB	     , -1	    },
    { "ins"	    , KEY_IC	     , -1	    },
    { "del"	    , KEY_DC	     , -1	    },
    { "esc"	    , KEY_ESC	     , -1	    },
    { "f1"	    , KEY_F(1)	     , -1	    },
    { "f2"	    , KEY_F(2)	     , -1	    },
    { "f3"	    , KEY_F(3)	     , -1	    },
    { "f4"	    , KEY_F(4)	     , -1	    },
    { "f5"	    , KEY_F(5)	     , -1	    },
    { "f6"	    , KEY_F(6)	     , -1	    },
    { "f7"	    , KEY_F(7)	     , -1	    },
    { "f8"	    , KEY_F(8)	     , -1	    },
    { "f9"	    , KEY_F(9)	     , -1	    },
    { "f10"	    , KEY_F(10)      , -1	    },
    { "f11"	    , KEY_F(11)      , -1	    },
    { "f12"	    , KEY_F(12)      , -1	    },
    { "m-minus"     , KEY_META('-')  , -1	    },
    { "m-equals"    , KEY_META('=')  , -1	    },
    { "m-comma"     , KEY_META(',')  , -1	    },
    { "m-period"    , KEY_META('.')  , -1	    },
    { ""	    , -1	     , -1	    }
	   };

keydefine::keydefine(void) {
    FILE *fd = NULL;

    for (int i = 0; i < QE_KEY_MAX; i++)
	keydef[i] = NULL;

    init_keycmd();

    if ((fd = fopen(DEFAULT_PROFILE, "r")) != NULL) {
	fclose(fd);
	init_profile(DEFAULT_PROFILE, 1);
    }
}

keydefine::keydefine(const char *filename) {
    FILE *fd = NULL;

    keydefine();

    if ((fd = fopen(filename, "r")) != NULL) {
	fclose(fd);
	init_profile(filename, 1);
    }
}

int keydefine::get_keycode(const char *key, int &k1, int &k2) {
    int  j, k;

    j = strlen(key);

    if (key[1] == '-' && j == 3) {
	k = int(key[2]);

	switch (key[0]) {
	case 'c':   // Control key
	case 'C':
	    k1 = KEY_CTRL(k);
	    return 1;
	case 'm':   // Meta key
	case 'M':
	    j = 1;
	    if (k >= 'a' && k <= 'z') j++, k2 = KEY_META(k - 'a' + 'A');
	    k1 = KEY_META(k);
	    return j;
	case 'x':   // Ctrl-X
	case 'X':
	    if (k >= 'a' && k <= 'z') {
		k1 = QE_XKEY_START + k - 'a' + 10;
	    } else if (k >= '0' && k <= '9') {
		k1 = QE_XKEY_START + k - '0';
	    }
	    return 1;
	case 'k':   // Ctrl-K
	case 'K':
	    if (k >= 'a' && k <= 'z') {
		k1 = QE_KKEY_START + k - 'a' + 10;
	    } else if (k >= '0' && k <= '9') {
		k1 = QE_KKEY_START + k - '0';
	    }
	    return 1;
	case 'q':   // Ctrl-Q
	case 'Q':
	    if (k >= 'a' && k <= 'z') {
		k1 = QE_QKEY_START + k - 'a' + 10;
	    } else if (k >= '0' && k <= '9') {
		k1 = QE_QKEY_START + k - '0';
	    }
	    return 1;
	case 'u':   // User key
	case 'U':
	    if (k >= 'a' && k <= 'z') {
		k1 = QE_UKEY_START + k - 'a' + 10;
	    } else if (k >= '0' && k <= '9') {
		k1 = QE_UKEY_START + k - '0';
	    }
	    return 1;
	    break;
	default :
	    return 0;
	    break;
	}
    } else if (key[0] >= '0' && key[0] <= '9') {
	for (int i = 0; i < j; i++) if (key[i] < '0' || key[j] > '9') return 0;

	k1 = atoi(key);
	return 1;
    }

    for (int i = 0; kstr[i].first != -1; i++) {
	if (strcmp(key, kstr[i].key) == 0) {
	    k1 = kstr[i].first;
	    if (kstr[i].second != -1) {
		k2 = kstr[i].second;
		return 2;
	    }
	    return 1;
	}
    }
    return 0;
}

int keydefine::define_key(const char *defstr) {
    char    key[MAX_BUFFER_LEN+1];
    int     i, j, k, len = strlen(defstr);

    for (i = 0; i < len; i++) if (defstr[i] != ' ' && defstr[i] != '\t') break;

    if (strncmp(&defstr[i], "def ", 4) == 0) {
	i += 4;
    } else if (strncmp(&defstr[i], "d ", 2) == 0) {
	i += 2;
    } else {
	// fprintf(stderr, "error: %s\n", &defstr[i]);
	return 0;
    }

    for (; i < len; i++) if (defstr[i] != ' ' && defstr[i] != '\t') break;

    for (j = k =0; i < len; i++) {
	if (defstr[i] == '=') {
	    key[j] = '\0';
	    k = 1;
	    i++;
	    break;
	} else if (defstr[i] == ' ' || defstr[i] == '\t') {
	    key[j] = '\0';
	} else {
	    key[j++] = defstr[i];
	}
    }

    if (k == 0) return 0;
    for (; i < len; i++) if (defstr[i] != ' ' && defstr[i] != '\t') break;

    const char	  *cmdstr = &defstr[i];

    int    rtnval = 0;

    if (defstr[i] == '\0') rtnval = 2;

    int k1, k2;

    if ((j = get_keycode(key, k1, k2)) == 0) return 0;

    if (j == 2) {
	if (keydef[k2] != NULL) delete keydef[k2];
	keydef[k2] = new keycmdlist(cmdstr);
    }

    if (keydef[k1] != NULL) delete keydef[k1];
    keydef[k1] = new keycmdlist(cmdstr);
    if (keydef[k1]->list == NULL) return rtnval;

    return 1;
}

int keydefine::init_profile(const char *filename, const int  initflag) {
    FILE   *fd;
    char   cmdbuffer[8000];
    int    idx;

    if ((fd = fopen(filename, "r")) == NULL) return 0;

    while (fgets(cmdbuffer, MAX_BUFFER_LEN, fd) != NULL) {
	if (cmdbuffer[0] == '*') continue;

	for (idx = strlen(cmdbuffer)-1; idx >= 0; idx--) {
	    if (cmdbuffer[idx] == '\r' || cmdbuffer[idx] == '\n' ||
		cmdbuffer[idx] == ' '  || cmdbuffer[idx] == '\t') {
		cmdbuffer[idx] = '\0';
	    } else {
		break;
	    }
	}

	if (cmdbuffer[0] == '\0') continue;

	while (cmdbuffer[idx] == '+') {
	    cmdbuffer[idx] = '\0';

	    if (fgets(&cmdbuffer[idx], MAX_BUFFER_LEN, fd) != NULL) {
		for (idx = strlen(cmdbuffer)-1; idx >= 0; idx--) {
		    if (cmdbuffer[idx] == '\r' || cmdbuffer[idx] == '\n' ||
			cmdbuffer[idx] == ' '  || cmdbuffer[idx] == '\t') {
			cmdbuffer[idx] = '\0';
		    } else {
			break;
		    }
		}
	    } else {
		break;
	    }
	}

	if (define_key(cmdbuffer) == 0) {
	    if (filebuffer::setenviron(cmdbuffer) == 0) {
		if (initflag) {
		    fprintf(stderr, "unknow: %s\n", cmdbuffer);
		    exit(1);
		}
	    }
	}
    }
    fclose(fd);

    return 1;
}

keycmdlist *keydefine::getkeydef(int key) {
    return (key < QE_KEY_MAX) ? keydef[key] : (keycmdlist *) NULL;
}

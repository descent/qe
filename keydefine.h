#ifndef __QE_KEYDEFINE_
#define __QE_KEYDEFINE_

#include <stdio.h>
#include "config.h"
#include "keycmd.h"

class keydefine {
public:
    keycmdlist	 *klist;

protected:
    int 	 init_profile(const char *profile, const int initflag);

public:
    keydefine(void);
    keydefine(const char *profile);

    keycmdlist*  getkeydef(int);
    static int	 define_key(const char *cmd);
    static int	 get_keycode(const char *key, int &k1, int &k2);
};

#endif

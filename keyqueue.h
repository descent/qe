/*
 *  keyqueue.h
 *
 *  Copyright (C) 1998, 1999  Jiann-Ching Liu
 */


#ifndef __QE_KEY_QUEUE_H_
#define __QE_KEY_QUEUE_H_

class keyqueue {
protected:
    int    *keybuffer;
    int    key_front;
    int    key_rear;
    int    keybuffer_len;
public:
    keyqueue(int len = 4096);
   ~keyqueue(void);

    int    enqueue(const int  key);
    int    enqueue(const char *str);
    int    dequeue(void);
    int    isempty(void);
    void   emptyit(void);
};

extern keyqueue queue;

#endif

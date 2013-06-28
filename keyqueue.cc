/*
 *  keyqueue.cc
 *
 *  Copyright (C) 1998, 1999  Jiann-Ching Liu
 */


#include "keyqueue.h"

keyqueue::keyqueue(int len) {
    keybuffer = new int[keybuffer_len = len];
    key_front = key_rear = 0;
}

keyqueue::~keyqueue(void) {
    delete keybuffer;
}

int keyqueue::enqueue(const int  key) {
    if ((key_rear + 1) % keybuffer_len == key_front) return 0;

    keybuffer[key_rear] = key;
    key_rear = (key_rear + 1) % keybuffer_len;
    return 1;
}

int keyqueue::enqueue(const char  *str) {
    int  len = strlen(str);
    int  rtlval = 1;

    for (int i = 0; i < len; i++)
	if ((rtlval = enqueue(int((unsigned char) str[i]))) == 0) break;
    return rtlval;
}

int keyqueue::dequeue(void) {
    if (key_front == key_rear) return -1;

    int key = keybuffer[key_front];
    key_front = (key_front + 1) % keybuffer_len;
    return key;
}

int keyqueue::isempty(void) {
    return (key_front == key_rear);
}

void keyqueue::emptyit(void) {
    key_front = key_rear;
}

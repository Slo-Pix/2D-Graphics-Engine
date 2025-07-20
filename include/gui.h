/* gui.h */
#pragma once

#define $1 (int8 *)
#define $2 (int16)
#define $v (void *)
#define false 0
#define true 1

#define packed __attribute__((packed))

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned char boolean;

extern void *heap1;
extern int16 heapsz;

void print(int8*);
void putchar(int8);
void *alloc(int16);
void freeall(void);
void save(void);
void load(void);
int8 getchar(void);

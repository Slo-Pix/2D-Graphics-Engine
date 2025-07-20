/* main.c */
asm(
    ".code16gcc\n"
    "call main\n"
    "call exit\n"
);
#include <gui.h>
#include <assembly.h>
#include <shapes.h>

void *heaptr, *heapsaved;
extern boolean videoinit;

void main() {
    point *pnt1, *pnt2;
    rectangle *rct;
    int8 clr;
    int16 thickness;

    videoinit = false;
    freeall();

    videomode(x640x480x16);

    clr = 2;
    thickness = 10;

    pnt1 = mkpoint(10, 10, clr);
    pnt2 = mkpoint(500, 300, clr);
    if (!pnt1 || !pnt2) {
        print($1 "Memory error\r\n");
        return;
    }

    rct = mkrectangle(pnt1, pnt2, clr, 0, thickness, false);
    if (!rct) {
        print($1 "Memory error\r\n");
        return;
    }

    drawrectangle(rct);

    getchar();
    freeall();

    return;
}

int8 getchar(void) {
    int8 al, ah, ax;
    int8 ret;

    ax = xgetchar();
    al = (ax & 0xf);
    ah = ((ax & 0xf0) >> 4);
    
    ret = (al) ?
            al :
        ah;
    
    return ret;
}

void putchar(int8 c) {
    xputchar(c);

    return;
}

void print(int8 *str) {
    int8 *p;

    for (p=str; *p; p++)
        putchar(*p);

    return;
}

void *alloc(int16 size) {
    void *p;
    int16 newsize;

    if (!size)
        return $v 0;

    newsize = ((heaptr-heap1)+size);
    if (newsize > heapsz)
        return $v 0;

    p = heaptr;
    heaptr += size;

    return p;
}

void freeall() {
    heaptr = heap1;

    return;
}

void save() {
    heapsaved = heaptr;

    return;
}

void load() {
    heaptr = heapsaved;

    return;
}

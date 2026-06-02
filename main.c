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

void plot(int16 x, int16 y, int8 color) {
    point *p;

    save();
    p = mkpoint(x, y, color);
    if (p)
        drawpoint(p);
    load();

    return;
}

void trail(int16 x0, int16 y0, int16 x1, int16 y1, int8 color) {
    int16 t;

    if (x0 == x1) {
        if (y0 > y1) { t = y0; y0 = y1; y1 = t; }
        for (; y0 <= y1; y0++)
            plot(x0, y0, color);
    }
    else if (y0 == y1) {
        if (x0 > x1) { t = x0; x0 = x1; x1 = t; }
        for (; x0 <= x1; x0++)
            plot(x0, y0, color);
    }

    return;
}

void box(int16 x1, int16 y1, int16 x2, int16 y2, int8 color) {
    point *p1, *p2;
    rectangle *rct;
    int16 t;

    if (x1 > x2) { t = x1; x1 = x2; x2 = t; }
    if (y1 > y2) { t = y1; y1 = y2; y2 = t; }
    if ((x1 == x2) || (y1 == y2))
        return;

    save();
    p1 = mkpoint(x1, y1, color);
    p2 = mkpoint(x2, y2, color);
    if (p1 && p2) {
        rct = mkrectangle(p1, p2, color, 0, 2, false);
        if (rct)
            drawrectangle(rct);
    }
    load();

    return;
}

void main() {
    int16 cx, cy;
    int16 oldx, oldy;
    int16 anchorx, anchory;
    int16 step;
    int16 maxx, maxy;
    int8 clr;
    int8 key;
    boolean anchored;

    videoinit = false;
    freeall();

    videomode(x640x480x16);

    maxx = getmaxx();
    maxy = getmaxy();

    cx = 320;
    cy = 240;
    anchorx = 0;
    anchory = 0;
    step = 8;
    clr = 2;
    anchored = false;

    plot(cx, cy, clr);

    for (;;) {
        key = getchar();

        if (key == 'q')
            break;

        if (key == 'c') {
            clr++;
            if (clr > 0xf)
                clr = 1;
            continue;
        }
        else if (key == ' ') {
            anchorx = cx;
            anchory = cy;
            anchored = true;
            continue;
        }
        else if (key == 'r') {
            if (anchored)
                box(anchorx, anchory, cx, cy, clr);
            anchored = false;
            continue;
        }

        oldx = cx;
        oldy = cy;

        if (key == 'w') {
            if (cy >= step)
                cy -= step;
        }
        else if (key == 's') {
            if ((cy + step) <= maxy)
                cy += step;
        }
        else if (key == 'a') {
            if (cx >= step)
                cx -= step;
        }
        else if (key == 'd') {
            if ((cx + step) <= maxx)
                cx += step;
        }
        else
            continue;

        trail(oldx, oldy, cx, cy, clr);
    }

    freeall();

    return;
}

int8 getchar(void) {
    return xgetchar();
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

/* shapes.c */
#include <gui.h>
#include <shapes.h>
#include <assembly.h>

boolean videoinit;

point *mkpoint(int16 x, int16 y, int8 color) {
    point *p;
    int16 size;

    size = sizeof(struct s_point);
    p = (point *)alloc(size);
    if (!p)
        return (point *)0;
    
    p->x = x;
    p->y = y;
    p->color = color;

    return p;
}

line *mkline(point *pnt1, point *pnt2, int8 color, int16 thickness) {
    line *ln;
    int16 size;
    
    if (!pnt1 || !pnt2 || !thickness)
        return (line *)0;
    
    size = sizeof(struct s_line);
    ln = (line *)alloc(size);
    if (!ln)
        return (line *)0;
    
    ln->p1 = pnt1;
    ln->p2 = pnt2;
    ln->color = color;
    ln->thickness = thickness;

    return ln;
}

rectangle *mkrectangle(point *pnt1, point *pnt2, int8 fgcolor, int8 bgcolor,
    int16 thickness, boolean solid)
{
    rectangle *rct;
    int16 size;

    if (!pnt1 || !pnt2)
        return (rectangle *)0;

    size = sizeof(struct s_rectangle);
    rct = (rectangle *)alloc(size);
    if (!rct)
        return (rectangle *)0;
    
    rct->p1 = pnt1;
    rct->p2 = pnt2;
    rct->fgcolor = fgcolor;
    rct->bgcolor = bgcolor;
    rct->thickness = thickness;
    rct->flags = FlagNone;
    if (solid)
        rct->flags |= FlagSolid;
    
    return rct;
}

void videomode(int8 mode) {
    if (mode > 0x9f)
        return;

    xvideomode(mode);
    videoinit = true;

    return;
}

int16 getmaxx() {
    return (MaxX-1);
}

int16 getmaxy() {
    return (MaxY-1);
}

boolean drawpoint(point *pnt) {
    int16 maxx, maxy;

    if (!videoinit)
        return false;

    if (!pnt)
        return false;

    maxx = getmaxx();
    maxy = getmaxy();

    if ((pnt->x > maxx) || (pnt->y > maxy))
        return false;

    return xdrawpoint(pnt->x, pnt->y, pnt->color);
}

boolean drawline(line *ln) {
    point *pnt;
    int16 startx, starty;
    int16 maxx, maxy;
    int16 maxx_, maxy_;
    int16 x, y;

    if (!videoinit)
        return false;
    if (!ln)
        return false;
    
    maxx_ = getmaxx();
    maxy_ = getmaxy();

    if (ln->p1->x > ln->p2->x)
        return false;
    if (ln->p1->y > ln->p2->y)
        return false;

    if (ln->p1->x == ln->p2->x) {
        starty = ln->p1->y;
        maxy = (ln->p2->y+ln->thickness);
        if (maxy > maxy_)
            maxy = maxy_;

        startx = ln->p1->x;
        maxx = (ln->p1->x + ln->thickness);
        if (maxx > maxx_)
            maxx = maxx_;

    }
    else if (ln->p1->y == ln->p2->y) {
        startx = ln->p1->x;
        maxx = (ln->p2->x+ln->thickness);
        if (maxx > maxx_)
            maxx = maxx_;

        starty = ln->p1->y;
        maxy = (ln->p1->y + ln->thickness);
        if (maxy > maxy_)
            maxy = maxy_;
    }
    else
        return false;

    save();
    for (x=startx; x<maxx; x++)
        for (y=starty; y<maxy; y++) {
            pnt = mkpoint(x, y, ln->color);
            if (pnt)
                drawpoint(pnt);
            load();
        }
    
    return true;
}

boolean drawrectangle(rectangle *rct) {
    point *topright, *bottomleft;
    point *topleft, *bottomright;
    line *top, *bottom, *left, *right;
    int16 maxx, maxy;

    if (!videoinit)
        return false;
    if (!rct)
        return false;

    maxx = getmaxx();
    maxy = getmaxy();
    
    if ((rct->p1->x > maxx) || (rct->p1->y > maxy))
        return false;
    if ((rct->p1->x >= rct->p2->x) || (rct->p1->y >= rct->p2->y))
        return false;

    topleft = rct->p1;
    bottomright = rct->p2;
    topright = mkpoint(bottomright->x, topleft->y, rct->fgcolor);
    bottomleft = mkpoint(topleft->x, bottomright->y, rct->fgcolor);
    if (!topright || !bottomleft)
        return false;
    
    top = mkline(topleft, topright, rct->fgcolor, rct->thickness);
    bottom = mkline(bottomleft, bottomright, rct->fgcolor, rct->thickness);
    left = mkline(topleft, bottomleft, rct->fgcolor, rct->thickness);
    right = mkline(topright, bottomright, rct->fgcolor, rct->thickness);

    if (!top || !bottom || !left || !right)
        return false;
    
    drawline(top);
    drawline(bottom);
    drawline(left);
    drawline(right);

    // (xx) solid bgcolor
    return true;
}

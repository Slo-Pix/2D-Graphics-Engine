; xgfx.asm
bits 16
%include "xgfx.h"
global heap1
global heapsz
global exit
global xputchar
global xvideomode
global xgetchar
global xdrawpointT
global xdrawpoint

heap1   dd _heap
_heap:
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
heapsz  dw ($-_heap)

xdrawpoint:
        push bp
        mov bp,sp

        arg cx,0
        arg dx,1
        arg ax,2

        xor bx,bx
        mov ah,0x0c
        int 0x10

        mov sp,bp
        pop bp
        ret

xdrawpointT:
        push bp
        mov bp,sp

        arg ax,0
        arg dx,1

        and dx,0xf
        and ax,0xf
        mov dh,al

        xor ax,ax
        mov ah,0x02
        xor bx,bx
        int 0x10

        mov ah,0x0a
        mov al,'+'
        xor bx,bx
        mov cx,0x01
        int 0x10

        mov ax,0x01
        mov sp,bp
        pop bp
        ret

xgetchar:
        push bp
        mov bp,sp

        xor ax,ax
        int 0x16

        mov sp,bp
        pop bp
        ret

xvideomode:
        push bp
        mov bp,sp

        arg ax,0
        xor ah,ah
        int 0x10

        mov sp,bp
        pop bp
        ret

exit:
        push bp
        mov bp,sp

        xor ax,ax
        int 0x21

    .halt:
        cli
        hlt
        jmp .halt
    
        mov sp,bp
        pop bp
        ret

xputchar:
        push bp
        mov bp,sp

        arg ax,0
        mov ah,0x0e
        xor bx,bx
        int 0x10

        mov sp,bp
        pop bp
        ret

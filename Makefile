flags := -fno-stack-protector -O0 -Wall -std=gnu2x -m16
flags += -ffreestanding -I include -nostdlib -fno-pic -fno-pie
ldflags := -m elf_i386 --nmagic --script=gui.ld
objects := main.o shapes.o
xobjects := xgfx.o
NASMENV := -i include/asm/
export NASMENV

.PHONY: clean

all: gui.com

gui.com: $(objects) $(xobjects)
	ld $(ldflags) $^ -o $@

main.o: main.c
	cc $(flags) -c $^

shapes.o: shapes.c
	cc $(flags) -c $^

xgfx.o: xgfx.asm
	nasm -f elf $^ -o $@

clean:
	rm -f *.o *.com

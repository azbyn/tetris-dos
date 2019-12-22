CFLAGS := -std=gnu11 -Os -nostdlib -m16 -march=i386 -Wall -Wextra \
  -Wno-unused-function \
  -ffreestanding -fomit-frame-pointer -fwrapv -fno-strict-aliasing \
  -fno-leading-underscore -fno-pic -fno-stack-protector
LDFLAGS := -Wl,--nmagic,--static,--script=com.ld
OUT := t.com
IN := main.c
CC := gcc
main:
	@${CC} ${CFLAGS} -o ${OUT} ${LDFLAGS} ${IN}
clean:
	@rm main.o -vf

#!/usr/bin/make

CC=clang
LD=ld
PKGF = `pkg-config  --cflags libnotify`
CFLAGS=-Werror -Wall -ggdb -O0 ${PKGF}

notify-hack.so: notify-hack.o
	${LD} -shared -o notify-hack.so notihack.o -ldl

notify-hack.o:
	${CC} ${CFLAGS} -fPIC -DPIC -c notihack.c


clean:
	rm -rf notify-hack.so

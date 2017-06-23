#
# Makefile for pong
#

all: pong

clean:
	rm -f pong

pong: pong.c
	gcc -Wall -o pong pong.c paddle.c set_ticker.c -lcurses



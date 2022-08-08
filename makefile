CC = gcc
FLAG = -lm

all: main.c modulo.c keyGen.c
	$(CC) main.c modulo.c -o affinehill
	$(CC) keyGen.c modulo.c -o keyGen


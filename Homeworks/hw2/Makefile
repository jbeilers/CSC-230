CC = gcc
CFLAGS = -Wall -std=c99
TARGET = hw2

spellcheck: spellcheck.o util.o correction.o

spellcheck.o: spellcheck.c util.h correction.h
util.o: util.c util.h
correction.o: correction.c correction.h

clean:
	rm -f spellcheck.o util.o correction.o
	rm -f spellcheck
	rm -f output.txt input.txt



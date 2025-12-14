CC = gcc
CFLAGS = -g -Wall -std=c99 -D_GNU_SOURCE
LDFLAGS = -lm

all: encrypt decrypt

encrypt: encrypt.o aes.o io.o aesUtil.o field.o

decrypt: aes.o io.o aesUtil.o field.o

encrypt.o: encrypt.c aes.h io.h field.h
decrypt.o: decrypt.c aes.h io.h field.h
aes.o: aes.c aes.h aesUtil.h field.h
io.o: io.c io.h field.h
aesUtil.o: aesUtil.c aesUtil.h field.h
field.o: field.c field.h

clean:
	rm -f encrypt decrypt *.o output.* stderr.txt
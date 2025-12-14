CC = gcc
CFLAGS = -Wall -std=c99 -g
LDFLAGS = -lm --coverage
all: snark

snark: snark.o archive.o IO.o compress.o error.o buffer.o
	$(CC) $(CFLAGS) -o snark snark.o archive.o IO.o compress.o error.o buffer.o $(LDFLAGS)
snark.o: snark.c archive.h error.h
archive.o: archive.c archive.h IO.h compress.h error.h
IO.o: IO.c IO.h buffer.h
compress.o: compress.c compress.h buffer.h
error.o: error.c error.h
buffer.o: buffer.c buffer.h

clean:
	rm -f snark *.o *.gdca *.gcno *.gcov output.* stderr.txt
CC = gcc
CFLAGS = -g -Wall -std=c99 -D_GNU_SOURCE
LDFLAGS = -lm
TARGET = travel

$(TARGET): travel.o parklist.o citylist.o input.o
	$(CC) $(CFLAGS) -o $(TARGET) travel.o parklist.o citylist.o input.o $(LDFLAGS)

travel.o: travel.c parklist.h citylist.h
	$(CC) $(CFLAGS) -c travel.c -o travel.o
parklist.o: parklist.c input.h
	$(CC) $(CFLAGS) -c parklist.c -o parklist.o
citylist.o: citylist.c input.h
	$(CC) $(CFLAGS) -c citylist.c -o citylist.o
input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c -o input.o

clean:
	rm -f $(TARGET) *.o output.txt stderr.txt
CC = gcc
CFLAGS = -Wall -std=c99

main: main.o names.o scores.o report.o

main.o: main.c names.h scores.h report.h
names.o: names.c names.h
scores.o: scores.c scores.h names.h
report.o: report.c report.h
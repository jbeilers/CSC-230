/**
 * @file input.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains a method for reading and returning a single line of text from an input file
 */

#ifndef HW3_INPUT_H
#define HW3_INPUT_H
#include <stdio.h>

#endif //HW3_INPUT_H

/** line size that should throw error if length of string ever reached this size */
#define MAX_SIZE 45

/**
 * Takes an unread line form a file pointer and stores it as
 * a string inside a block of dynamically allocated memory
 * @param fp File pointer to read from
 * @return Line of text representing line of file read
 */
char *readLine(FILE *fp);
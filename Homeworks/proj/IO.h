/**
* @file IO.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods for IO processes
 */

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

/**
 * Reads a line from the given file pointer
 * @param fp file pointer to read from
 * @return string of characters
 */
char *readLine(FILE *fp);

/**
 * Reads from a file in binary mode and stores it in a buffer
 * @param filename filename of file to be read from
 * @return buffer containing data from file
 */
Buffer *readFile(char const *filename);

/**
 * Writes data from a buffer to a file
 * @param filename filename of file to write to
 * @param buf buffer to extract data from
 * @return true: success | false: failure
 */
bool writeFile(char const *filename, Buffer *buf);

#endif

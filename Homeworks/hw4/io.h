/**
 *@file io.h
 *@author Jordan B. Eilers (jbeilers)
 *This File contains I/O functions for binary and text files
 */

#ifndef HW4_IO_H
#define HW4_IO_H

#include "field.h"

/** argc value if padding flag is not added */
#define REG_ARG_COUNT 4
/** argc value if padding flag is added */
#define PADDING_ARG_COUNT 5

/**
 * This function reads byte values from a file and stores it in a dynamic array
 * @param filename Filename of file to be opened
 * @param size Amount of bytes read from file
 * @return Dynamic array of bytes
 */
byte *readBinaryFile(char const *filename, int *size);

/**
 * This function writes bytes from data array to a specified file
 * @param filename Filename of file to be opened
 * @param data Byte array to write from
 * @param size Amount of bytes to write
 */
void writeBinaryFile(char const *filename, byte *data, int size);

#endif //HW4_IO_H


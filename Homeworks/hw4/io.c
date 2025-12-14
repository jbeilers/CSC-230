/**
*@file io.c
 *@author Jordan B. Eilers (jbeilers)
 *This File contains I/O functions for binary and text files
 */

#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aesUtil.h"

byte *readBinaryFile(char const *filename, int *size)
{
    // Allocate initial data array
    byte *data = (byte *) malloc(BLOCK_SIZE * sizeof(byte));

    // Open file in either txt or binary format
    FILE *fp;
    if (filename[strlen(filename) - 3] == 't')
        fp = fopen(filename, "r");
    else
        fp = fopen(filename, "rb");

    // Check to see if file is valid
    if (!fp) {
        fprintf(stderr,"Can't open file: %s\n", filename);
        exit(1);
    }

    // Now read the file
    int buffer;
    do {
        // Check if resize is necessary
        if (*size % BLOCK_SIZE == 0 && *size != 0)
            data = (byte *) realloc(data, (*size + BLOCK_SIZE) * sizeof(byte));
        buffer = (int) fread(data + *size, sizeof(byte), BLOCK_SIZE, fp);
        *size += buffer;
    } while (buffer != 0);

    // Close file and return data
    fclose(fp);
    return data;
}

void writeBinaryFile(char const *filename, byte *data, int size)
{
    // Open file for writing
    FILE *fp = fopen(filename, "wb");

    // Check to see if file is valid
    if (!fp) {
        fprintf(stderr,"Can't open file: %s\n", filename);
        exit(1);
    }

    // Put chars into file
    fwrite(data, sizeof(byte), size, fp);

    // Close file
    fclose(fp);
}
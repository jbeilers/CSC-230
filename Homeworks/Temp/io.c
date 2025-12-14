//
// Created by jeile on 10/29/2025.
//

#include "io.h"

#include <stdio.h>
#include <stdlib.h>

#include "aesUtil.h"

byte *readBinaryFile(char const *filename, int *size)
{
    byte *data = (byte *) malloc(BLOCK_SIZE * sizeof(byte));

    // Open file
    FILE *fp = fopen(filename, "rb");
    // Check to see if file is valid
    if (!fp) {
        fprintf(stderr,"Can't open file: %s", filename);
        exit(1);
    }
    // Go through file
    while (!feof(fp)) {
        // Check if a resize is necessary
        if (*size % BLOCK_SIZE == 0 && *size != 0) {
            data = (byte *) realloc(data, (*size + BLOCK_SIZE));
            // Check if reallocation failed
            if (data == NULL) {
                free(data);
                exit(23);
            }
        }
        // Check if the current character is a return char
        char buffer = fgetc(fp);
        if (buffer == RETURN_CHAR)
            break; // Break out of while loop
        //Add character to data
        data[(*size)++] = buffer;
    }
    fclose(fp);
    return data;
}

void writeBinaryFile(char const *filename, byte *data, int size)
{
    // Open file for writing
    FILE *fp = fopen(filename, "wb");
    // Put chars into file
    fwrite(data, size, sizeof(byte), fp);
    // Close file
    fclose(fp);
}
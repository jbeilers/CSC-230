//
// Created by jeile on 10/29/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "field.h"
#include "io.h"
#include "aes.h"

#define PADDING true
#define NO_PADDING false

int main(int argc, char *argv[])
{
    // Check for number of input arguments
    if (argc < 4 || argc > 5) {
        fprintf(stderr,"usage: encrypt <key-file> <input-file> <output-file>");
        exit(1);
    }

    // Initialize key size
    int keySize = 0;
    // Scan key and store it
    byte *key = readBinaryFile(argv[argc-3], &keySize);
    // Check key size
    if (keySize != BLOCK_SIZE) {
        fprintf(stderr, "Bad key file: %s", argv[argc-2]);
        exit(1);
    }

    // Initialize Input size
    int inputSize = 0;
    // Scan input file and store it
    byte *input = readBinaryFile(argv[argc-2], &inputSize);

    // Check for padding
    int diff = inputSize % BLOCK_SIZE;
    if (argc == 5) { // Its Padding Time!
        for (int i = diff; i < BLOCK_SIZE; i++)
            input[inputSize++] = (byte) diff;
    } else {
        if (diff != 0) { // If not a multiple of 16
            fprintf(stderr, "Bad plaintext file length: %s", argv[argc - 1]);
            exit(1);
        }
    }
    // Encrypt, encrypt, encrypt!
    encryptBlock(input, key);

    // Write to output file
    writeBinaryFile(argv[argc], input, inputSize);

    free(input);
    free(key);
    // Exit successfully
    exit(EXIT_SUCCESS);
}
/**
 *@file decrypt.c
 *@author Jordan B. Eilers (jbeilers)
 *This file contains the main method that will decrypt a file using a specified key
 */

#include <stdio.h>
#include <stdlib.h>

#include "field.h"
#include "io.h"
#include "aes.h"

/**
 * Reads a key and cipher file,
 * decrypts all the data, and writes to an output file
 * @param argc Argument count
 * @param argv Command arguments
 * @return Exit status
 */
int main(int argc, char *argv[])
{
    // Check for number of input arguments
    if (argc != 4 && argc != 5) {
        fprintf(stderr,"usage: encrypt <key-file> <input-file> <output-file>\n");
        exit(1);
    }

    // Store args
    char *kFile = argv[argc-3], *iFile = argv[argc-2], *oFile = argv[argc-1];

    // Initialize key size
    int keySize = 0;
    // Scan key and store it
    byte *key = readBinaryFile(kFile, &keySize);
    // Check key size
    if (keySize != BLOCK_SIZE && argc != 5) {
        fprintf(stderr, "Bad key file: %s\n", kFile);
        exit(1);
    }

    // Initialize Input size
    int inputSize = 0;
    // Scan input file and store it
    byte *input = readBinaryFile(iFile, &inputSize);

    // Check for padding
    int diff = inputSize % BLOCK_SIZE;
    if (argc != 5 && diff != 0) { // If not a multiple of 16
        fprintf(stderr, "Bad plaintext file length: %s\n", iFile);
        exit(1);
    }
    // Decrypt, Decrypt, Decrypt!
    for (int i = 0; i < inputSize/BLOCK_SIZE; i++)
        decryptBlock(input + (i*BLOCK_SIZE), key);

    // If padding, get rid of bytes
    if (argc == 5) {
        // Check for number of bytes to not read
        int check = (int) input[inputSize - 1];
        writeBinaryFile(oFile, input, inputSize - check);
    }
    else
        writeBinaryFile(oFile, input, inputSize);

    free(input);
    free(key);
    // Exit successfully
    exit(EXIT_SUCCESS);
}
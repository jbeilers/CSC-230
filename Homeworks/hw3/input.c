/**
 * @file input.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains a method for reading and returning a single line of text from an input file
 */

#include "input.h"

#include <stdlib.h>


char *readLine(FILE *fp)
{
    // Initialize string and set index count to 0
    char *in = calloc(MAX_SIZE, sizeof(char));
    int index = 0;

    // Get 1st character
    char temp = fgetc(fp);

    // If the character is EOF, return NULL
    if (temp == EOF) {
        return NULL;
    }

    // While there's text to read
    while (temp != '\n' && index < MAX_SIZE - 1) {
        // Add character to line and offset null terminator by +1 index
        in[index] = temp;
        in[++index] = '\0';

        // Read another character
        temp = fgetc(fp);
    }
    // Check to see if the next character was EOF
    char next = fgetc(fp);

    // If not, go back one character
    if (next != EOF)
        ungetc(next, fp);

    // Set the last character to a null terminator to ensure string of proper length
    in[index] = '\0';

    return in;
}

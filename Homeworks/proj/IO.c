/**
* @file IO.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods for IO processes
 */

#include "IO.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/** arbitrary buffer size */
#define BUFFER_SIZE 50

char *readLine(FILE *fp)
{
    int bufSize = 0, bufCapacity = BUFFER_SIZE;
    char *line = (char *) malloc(bufCapacity * sizeof(char));
    while (true) {
        char c = (char) fgetc(fp);
        // Check if a buffer resize is necessary
        if (bufSize == bufCapacity)
            line = (char *) realloc(line, bufCapacity *= 2 * sizeof(char));
        // Add character to buffer line
        line[bufSize++] = c;
        // check if the character is a newline char
        if (c == '\n' || c == EOF)
            break;
    }
    if (line[0] == '\n') {
        strcpy(line, "#\n"); // Treat it as a comment so the program can skip it
        bufSize++;
    }
    line = realloc(line, bufSize * sizeof(char));
    return line;
}

Buffer *readFile(char const *filename)
{
    // Open file pointer
    FILE *fp = fopen(filename, "rb");
    if (!fp) // Invalid file
        return NULL;
    // Create a buffer variable
    Buffer *buf = makeBuffer();

    // Get size of file
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    // Read whole file
    char line[fileSize];
    fread(line, sizeof(byte), fileSize, fp);

    // Store it in a buffer
    appendBytes(buf, line, fileSize);

    fclose(fp);
    return buf;
}

bool writeFile(char const *filename, Buffer *buf)
{
    // Open file pointer
    FILE *fp = fopen(filename, "ab");
    if (!fp) // Invalid file
        return NULL;
    fseek(fp, 0, SEEK_END);
    fwrite(buf->data, sizeof(byte), buf->len, fp);

    fclose(fp);
    return true;
}
/**
* @file buffer.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods pertaining to buffers
 */

#include "buffer.h"

#include <math.h>
#include <stdlib.h>

/** Initial capacity of the buffer. */
#define INITIAL_CAP 5

Buffer *makeBuffer()
{
    Buffer *buf = malloc(sizeof(Buffer));
    buf->data = malloc(INITIAL_CAP * sizeof(byte));
    buf->len = 0;
    buf->cap = INITIAL_CAP;
    buf->pos = 0;
    return buf;
}

void freeBuffer(Buffer *buf)
{
    // Free buffer data and the buffer itself
    free(buf->data);
    free(buf);
}

void appendByte(Buffer *buf, byte val)
{
    // Check if a resize is necessary
    resize(buf, 1);
    // Add byte to end of buffer
    buf->data[buf->len++] = val;
}

void appendBytes(Buffer *buf, void *seq, const int n)
{
    // Check if a resize if necessary
    resize(buf, n);
    byte *castSeq = (byte *) seq;
    int clen = buf->len;
    // Add sequence to end of buffer
    for (int i = clen; i < clen + n; i++) {
        buf->data[i] = castSeq[i - clen];
    }
    buf->len += n;
}

bool extractByte(Buffer *buf, byte *val)
{
    // If data is empty or the position is at the length, return false
    if (buf->len == 0 || buf->len == buf->pos)
        return false;
    // Otherwise extract away!
    *val = buf->data[buf->pos++];
    return true;
}

bool extractBytes(Buffer *buf, void *seq, const int n)
{
    // If data is empty or the sequence of bytes flows past the
    // amount stored in data, return false
    if (buf->len == 0 || buf->pos + n > buf->len)
        return false;
    // Otherwise extract away!
    for (int i = buf->pos; i < buf->pos + n; i++) {
        // Create pointer to current element of seq
        byte *temp = (byte *) seq + (i - buf->pos);
        // Set the value to the buffer data
        *temp = buf->data[i];
    }
    buf->pos += n;
    return true;
}


void resize(Buffer *buf, int additional)
{
    // How much bigger to multiply the capacity by
    int factor = ceil(( (double) (buf->len) + additional ) / ( buf->cap ));
    if (factor > 1) {
        // Increase capacity field
        buf->cap *= factor;
        // Reallocate buffer data
        buf->data = (byte *) realloc(buf->data, buf->cap * sizeof(byte));
    }
}
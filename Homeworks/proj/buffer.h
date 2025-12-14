/**
* @file buffer.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods pertaining to buffers
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <stdbool.h>

/** Type used to represent a byte. */
typedef unsigned char byte;

typedef struct {
  /** Dynamically allocated sequence of bytes stored in this buffer. */
  byte *data;

  /** Number of bytes stored in this buffer. */
  int len;

  /** Capacity of the data array. */
  int cap;
  
  /** Current position in the buffer, to simplify processing the
      contents of the buffer from front to back. */
  int pos;
} Buffer;

/**
 * Creates a new buffer structure
 * @return pointer to newly made buffer
 */
Buffer *makeBuffer();

/**
 * Frees the data from a buffer
 * @param buf buffer to be freed
 */
void freeBuffer(Buffer *buf);

/**
 * Appends one byte to the end of a buffer
 * @param buf buffer to append to
 * @param val byte to append
 */
void appendByte(Buffer *buf, byte val);

/**
 * appends n bytes to the end of a buffer
 * @param buf buffer to append to
 * @param seq sequence of bytes to append
 * @param n number of bytes to append
 */
void appendBytes(Buffer *buf, void *seq, int n);

/**
 * Extracts a byte from a buffer and stores it
 * @param buf buffer to extract a byte from
 * @param val byte to be extracted
 * @return true: success | false: failure
 */
bool extractByte(Buffer* buf, byte* val);

/**
 * Extracts multiple bytes from a buffer and stores them
 * @param buf buffer to be extracted from
 * @param seq sequence of bytes to store extracted bytes in
 * @param n number of bytes to extract
 * @return true: success | false: failure
 */
bool extractBytes(Buffer *buf, void *seq, int n);


/**
 * Resizes a buffer if the additional bytes will exceed the buffer size
 * @param buf buffer to resize
 * @param additional number of bytes being added
 */
void resize(Buffer *buf, int additional);

#endif

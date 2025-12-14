/**
 */

#ifndef FIELD_H
#define FIELD_H

/** Type used for our field, an unsigned byte. */
typedef unsigned char byte;

/** Number of bits in a byte. */
#define BBITS 8

#define REDUCER 0x11B << 7

byte fieldAdd(byte a, byte b);

byte fieldSub(byte a, byte b);

byte fieldMul(byte a, byte b);

#endif


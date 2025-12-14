/**
 *@file field.h
 *@author Jordan B. Eilers (jbeilers)
 *This file contains Galois field arithmetic functions for byte data
 */

#ifndef FIELD_H
#define FIELD_H

/** Type used for our field, an unsigned byte. */
typedef unsigned char byte;

/** Number of bits in a byte. */
#define BBITS 8
/** Reducer hex value for byte multiplication */
#define REDUCER 0x11B << 7

/**
 * Adds bytes a & b using Galois field addition
 * @param a byte a
 * @param b byte b
 * @return Galois field a + b
 */
byte fieldAdd(byte a, byte b);

/**
 * Subtracts byte b from a using Galois field subtraction
 * @param a byte a
 * @param b byte b
 * @return Galois field a - b
 */
byte fieldSub(byte a, byte b);

/**
 * Multiplies bytes a & b using Galois field multiplication
 * @param a byte a
 * @param b byte b
 * @return Galois field a*b
 */
byte fieldMul(byte a, byte b);

#endif


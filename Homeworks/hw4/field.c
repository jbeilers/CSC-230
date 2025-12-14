/**
*@file field.c
 *@author Jordan B. Eilers (jbeilers)
 *This file contains Galois field arithmetic functions for byte data
 */

#include "field.h"

byte fieldAdd(byte a, byte b)
{
    return a ^ b;
}

byte fieldSub(byte a, byte b)
{
    return a ^ b;
}

byte fieldMul(byte a, byte b) // a x b
{
    // Set initial byte to 0
    unsigned short currentBits = 0x0000;
    // Set initial bit mask to 0x0001 to get rightmost bit
    unsigned short maskCurrent = 0x0001;

    for (int i = 0; i < BBITS; i++) {
        // If the current bit is 1
        if ((b & maskCurrent) == maskCurrent) {
            if (currentBits == 0) // If currentBits is empty
                currentBits = (a << i); // Set currentBits to shifted a byte
            else
                currentBits ^= (a << i); // Do xor operation
        }

        // shift bit mask to the left
        maskCurrent <<= 1;
    }

    // Now do Stage 2
    maskCurrent = 0x8000;

    for (int i = 0; i < BBITS; i++) {
        // If the current bit is 1
        if ((currentBits & maskCurrent) == maskCurrent)
            currentBits ^= (REDUCER >> i); // Do xor operation

        // Shift bit mask to the right
        maskCurrent >>= 1;
    }

    return (byte) currentBits;
}
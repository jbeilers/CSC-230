/**
  */

#ifndef AES_H
#define AES_H

#include <stdbool.h>

#include "aesUtil.h"

#define LEFT true
#define RIGHT false

/**
 * Generates subkeys from a given key and stores it in an array
 * @param subkey Array of generated subkeys
 * @param key Key to generate subkeys from
 */
void generateSubkeys(byte subkey[ROUNDS + 1][BLOCK_SIZE], byte const key[BLOCK_SIZE]);

/**
 * Adds a given subkey to the data
 * @param data Array of bytes
 * @param key Subkey to add to data
 */
void addSubkey(byte data[BLOCK_SIZE], byte const key[BLOCK_SIZE]);

/**
 * Converts a line of 16 bytes to a 4x4 square of bytes
 * @param square 2D square of bytes
 * @param data Line of bytes to be converted
 */
void blockToSquare(byte square[BLOCK_ROWS][BLOCK_COLS], byte const data[BLOCK_SIZE]);

/**
 * Converts a 4x4 square of bytes to a line of 16 bytes
 * @param data Line of bytes
 * @param square 2D square of bytes to be converted
 */
void squareToBlock(byte data[BLOCK_SIZE], byte const square[BLOCK_ROWS][BLOCK_COLS]);

/**
 * Shifts each row to the left the same number as their row index
 * @param square 4x4 square of bytes
 */
void shiftRows(byte square[BLOCK_ROWS][BLOCK_COLS]);

/**
 * Shifts each row to the right the same number as their row index
 * @param square 4x4 square of bytes
 */
void unShiftRows(byte square[BLOCK_ROWS][BLOCK_COLS]);

/**
 * Multiplies each column by a given matrix
 * @param square 4x4 square of bytes
 */
void mixColumns(byte square[BLOCK_ROWS][BLOCK_COLS]);

/**
 * Multiplies each column by the inverse of a given matrix
 * @param square 4x4 square of bytes
 */
void unMixColumns(byte square[BLOCK_ROWS][BLOCK_COLS]);

/**
 * Encrypts data through a series of operations
 * @param data Date to be encrypted
 * @param key Key to be used in encryption
 */
void encryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE]);

/**
 * Decrypts data through the reverse series of operations as
 * the encryptBlock function follows
 * @param data Data to be decrypted
 * @param key Key to be used in decryption
 */
void decryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE]);

/**
 * Gets 4 4-byte data blocks from data
 * @param word1 4-byte array
 * @param word2 4-byte array
 * @param word3 4-byte array
 * @param word4 4-byte array
 * @param data line of 16 bytes to split
 */
void getWords(byte word1[WORD_SIZE], byte word2[WORD_SIZE], byte word3[WORD_SIZE], byte word4[WORD_SIZE], byte const data[BLOCK_SIZE]);

/**
 * Puts each word back into a line of 16 bytes
 * @param word1 4-byte array
 * @param word2 4-byte array
 * @param word3 4-byte array
 * @param word4 4-byte array
 * @param data line of 16 bytes to be created
 */
void wordsToData(const byte word1[WORD_SIZE], const byte word2[WORD_SIZE], const byte word3[WORD_SIZE], const byte word4[WORD_SIZE], byte data[BLOCK_SIZE]);

/**
 * Copies each byte from the source to the dest variables
 * @param size amount of bytes to copy
 * @param source source to copy from
 * @param dest destination to copy to
 */
void copyData(int size, const byte source[size], byte dest[size]);

/**
 * Performs a Galois field operation with two bytes
 * @param size Amount of bytes
 * @param source source data
 * @param dest destination data
 * @param operation Galois Field operation function
 */
void bitOperation(int size, const byte source[size], byte dest[size], byte (*operation)(byte a, byte b));

/**
 * Shifts a word either to the left or right
 * @param word 4-byte array
 * @param left true: left, false: right
 */
void shiftBits(byte word[WORD_SIZE], bool left);

/**
 * Performs matrix multiplication
 * @param square 4x4 block of bytes
 * @param vec Column from square
 * @param temp Temporary square data
 * @param column What column to be used in multiplying
 */
void vectorMultiply(byte const square[BLOCK_ROWS][BLOCK_COLS], byte const vec[BLOCK_ROWS], byte temp[BLOCK_ROWS][BLOCK_COLS], int column);

#endif



/**
  */

#ifndef AES_H
#define AES_H

#include <stdbool.h>

#include "aesUtil.h"

#define LEFT true
#define RIGHT false

void generateSubkeys(byte subkey[ROUNDS + 1][BLOCK_SIZE], byte const key[BLOCK_SIZE]);

void addSubkey(byte data[BLOCK_SIZE], byte const key[BLOCK_SIZE]);

void blockToSquare(byte square[BLOCK_ROWS][BLOCK_COLS], byte const data[BLOCK_SIZE]);

void squareToBlock(byte data[BLOCK_SIZE], byte const square[BLOCK_ROWS][BLOCK_COLS]);

void shiftRows(byte square[BLOCK_ROWS][BLOCK_COLS]);

void unShiftRows(byte square[BLOCK_ROWS][BLOCK_COLS]);

void mixColumns(byte square[BLOCK_ROWS][BLOCK_COLS]);

void unMixColumns(byte square[BLOCK_ROWS][BLOCK_COLS]);

void encryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE]);

void decryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE]);


void getWords(byte word1[WORD_SIZE], byte word2[WORD_SIZE], byte word3[WORD_SIZE], byte word4[WORD_SIZE], byte const data[BLOCK_SIZE]);

void wordsToData(const byte word1[WORD_SIZE], const byte word2[WORD_SIZE], const byte word3[WORD_SIZE], const byte word4[WORD_SIZE], byte data[BLOCK_SIZE]);

void copyData(int size, const byte source[size], byte dest[size]);

void bitOperation(int size, const byte source[size], byte dest[size], byte (*operation)(byte a, byte b));

void shiftBits(byte word[WORD_SIZE], bool left);

void vectorMultiply(byte const square[BLOCK_ROWS][BLOCK_COLS], byte const vec[BLOCK_ROWS], byte temp[BLOCK_ROWS][BLOCK_COLS], int column);

#endif



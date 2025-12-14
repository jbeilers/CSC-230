/**
  */

#include "aes.h"

#include <stdlib.h>

void generateSubkeys(byte subkey[ROUNDS + 1][BLOCK_SIZE], byte const key[BLOCK_SIZE])
{
    // Add key to first subkeys
    copyData(BLOCK_SIZE, key, subkey[0]);

    // Now do rounds

    // Define words of key
    byte word1[WORD_SIZE], word2[WORD_SIZE], word3[WORD_SIZE], word4[WORD_SIZE], temp[WORD_SIZE], data[BLOCK_SIZE];
    // Get words from key
    getWords(word1, word2, word3, word4, key);

    // Go through each round
    for (int i = 1; i <= ROUNDS; i++) {
        copyData(WORD_SIZE, word4, temp); // Copy word4 to temp
        // Step 1
        gFunction(temp, word4, i);
        bitOperation(WORD_SIZE, temp, word1, fieldAdd);
        // Step 2
        bitOperation(WORD_SIZE, word1, word2, fieldAdd);
        // Step 3
        bitOperation(WORD_SIZE, word2, word3, fieldAdd);
        // Step 4
        bitOperation(WORD_SIZE, word3, word4, fieldAdd);
        // Return words to block
        wordsToData(word1, word2, word3, word4, data);
        // Store data in subkey
        copyData(BLOCK_SIZE, data, subkey[i]);
    }

}

void addSubkey(byte data[BLOCK_SIZE], byte const key[BLOCK_SIZE])
{
    bitOperation(BLOCK_SIZE, key, data, fieldAdd);
}

void blockToSquare(byte square[BLOCK_ROWS][BLOCK_COLS], byte const data[BLOCK_SIZE])
{
    // Get words from data
    byte word1[WORD_SIZE], word2[WORD_SIZE], word3[WORD_SIZE], word4[WORD_SIZE];
    getWords(word1, word2, word3, word4, data);

    for (int c = 0; c < BLOCK_COLS; c++) {
        copyData(WORD_SIZE, (byte[]){word1[c], word2[c], word3[c], word4[c]}, square[c]);
    }
}

void squareToBlock(byte data[BLOCK_SIZE], byte const square[BLOCK_ROWS][BLOCK_COLS])
{
    byte word1[WORD_SIZE], word2[WORD_SIZE], word3[WORD_SIZE], word4[WORD_SIZE];
    for (int r = 0; r < BLOCK_COLS; r++) {
        int col = 0;
        word1[r] = square[r][col++];
        word2[r] = square[r][col++];
        word3[r] = square[r][col++];
        word4[r] = square[r][col++];
    }
    wordsToData(word1, word2, word3, word4, data);
}

void shiftRows(byte square[BLOCK_ROWS][BLOCK_COLS])
{
    for (int r = 0; r < BLOCK_ROWS; r++) {
        for (int i = 0; i < r; i++)
            shiftBits(square[r], LEFT);
    }
}

void unShiftRows(byte square[BLOCK_ROWS][BLOCK_COLS])
{
    for (int r = 0; r < BLOCK_ROWS; r++) {
        for (int i = 0; i < r; i++)
            shiftBits(square[r], RIGHT);
    }
}

void mixColumns(byte square[BLOCK_ROWS][BLOCK_COLS])
{
    byte vec[BLOCK_COLS];
    for (int c = 0; c < BLOCK_COLS; c++) {
        for (int r = 0; r < BLOCK_COLS; r++)
            vec[r] = square[r][c];
        vectorMultiply(forwardMixMatrix, vec, square, c);
    }
}

void unMixColumns(byte square[BLOCK_ROWS][BLOCK_COLS])
{
    byte vec[BLOCK_COLS];
    for (int c = 0; c < BLOCK_COLS; c++) {
        for (int r = 0; r < BLOCK_COLS; r++)
            vec[r] = square[r][c];
        vectorMultiply(inverseMixMatrix, vec, square, c);
    }
}

void encryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE])
{
    // Generate and store subkeys
    byte subkeys[ROUNDS + 1][BLOCK_SIZE];
    generateSubkeys(subkeys, key);

    // Add the first subkey
    addSubkey(data, subkeys[0]);

    for (int i = 1; i <= ROUNDS; i++) {
        // Initialize square version of data
        byte square[BLOCK_ROWS][BLOCK_COLS];
        // Replace each key byte with its substitution
        for (int j = 0; j < BLOCK_SIZE; j++)
            data[j] = substBox(data[j]);
        // Convert data to block
        blockToSquare(square, data);
        // Shift rows
        shiftRows(square);
        // Mix columns
        if (i != ROUNDS)
            mixColumns(square);
        // Convert square back to data
        squareToBlock(data, square);
        // Add subkey to data
        addSubkey(data, subkeys[i]);
    }
}

void decryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE])
{
    // Generate and store subkeys
    byte subkeys[ROUNDS + 1][BLOCK_SIZE];
    generateSubkeys(subkeys, key);

    for (int i = ROUNDS; i >= 1; i--) {
        // Add subkey to data
        addSubkey(data, subkeys[i]);
        // Initialize square version of data
        byte square[BLOCK_ROWS][BLOCK_COLS];
        // Convert data to block
        blockToSquare(square, data);
        // Mix columns
        if (i != ROUNDS)
            unMixColumns(square);
        // Shift rows
        unShiftRows(square);
        // Convert square back to data
        squareToBlock(data, square);
        // Replace each key byte with its substitution
        for (int j = 0; j < BLOCK_SIZE; j++)
            data[j] = invSubstBox(data[j]);
    }
    // Add subkey to data
    addSubkey(data, subkeys[0]);
}


void getWords(byte word1[WORD_SIZE], byte word2[WORD_SIZE], byte word3[WORD_SIZE], byte word4[WORD_SIZE], byte const data[BLOCK_SIZE])
{
    for (int i = 0; i < WORD_SIZE; i++) {
        // Copy initial words from data into words
        word1[i] = data[i];
        word2[i] = data[i + WORD_SIZE];
        word3[i] = data[i + 2*WORD_SIZE];
        word4[i] = data[i + 3*WORD_SIZE];
    }
}

void wordsToData(const byte word1[WORD_SIZE], const byte word2[WORD_SIZE], const byte word3[WORD_SIZE], const byte word4[WORD_SIZE], byte data[BLOCK_SIZE])
{
    for (int i = 0; i < WORD_SIZE; i++) {
        data[i] = word1[i];
        data[i + WORD_SIZE] = word2[i];
        data[i + 2*WORD_SIZE] = word3[i];
        data[i + 3*WORD_SIZE] = word4[i];
    }
}

void copyData(const int size, const byte source[size], byte dest[size])
{
    for (int i = 0; i < size; i++)
        dest[i] = source[i];
}

void bitOperation(const int size, const byte source[size], byte dest[size], byte (*operation)(byte a, byte b))
{
    for (int i = 0; i < size; i++)
        dest[i] = operation(dest[i], source[i]);
}

void shiftBits(byte word[WORD_SIZE], bool left)
{
    if (left) {
        const byte temp = word[0];
        for (int i = 0; i < WORD_SIZE - 1; i++)
            word[i] = word[i + 1];
        word[WORD_SIZE - 1] = temp;
    } else {
        const byte temp = word[WORD_SIZE - 1];
        for (int i = WORD_SIZE - 1; i > 0; i--)
            word[i] = word[i - 1];
        word[0] = temp;
    }
}

void vectorMultiply(byte const reference[BLOCK_ROWS][BLOCK_COLS], byte const vec[BLOCK_ROWS], byte square[BLOCK_ROWS][BLOCK_COLS], int column)
{

    for (int r = 0; r < BLOCK_ROWS; r++) {
        byte block = 0;
        for (int c = 0; c < BLOCK_COLS; c++)
            block = fieldAdd( fieldMul(reference[r][c], vec[c]), block );
        square[r][column] = block;
    }
}
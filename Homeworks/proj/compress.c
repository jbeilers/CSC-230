/**
* @file compress.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods for compressing data
 */

#include "compress.h"
#include <stdlib.h>
#include <string.h>

/** Limit on the number of different byte values in an uncompressed block. */
#define BYTE_LIMIT 224

/** Minimum number of occurrences of a pair in order to create a rule
    to replace it. */
#define REPLACEMENT_THRESHOLD 3

#define STORE 1
#define SEARCH 2

#define CHAR 1
#define SHORT 2

/** max binary value for a hex value (0xF) */
#define MAX_HEX_VALUE 16

void serializeBlock(Block *block, Buffer *buf)
{
    if (block->len == 0)
        return;
    // Store data as appropriate
    unsigned short len = block->len;
    // Add final size
    appendByte(buf, len & RIGHT_MASK);
    appendByte(buf, (len & LEFT_MASK) >> 8);
    // Add block data
    appendBytes(buf, block->data, block->len);
    // Add num of rules
    appendByte(buf, block->rcount);
    // Add rules
    for (int i = 0; i < block->rcount; i++) {
        appendByte(buf, block->rlist[i].code);
        appendByte(buf, block->rlist[i].first);
        appendByte(buf, block->rlist[i].second);
    }
}

bool deserializeBlock(Block *block, Buffer *buf)
{
    // Get first two bytes for size
    byte temp1, temp2;
    if (!extractByte(buf, &temp1) || !extractByte(buf, &temp2))
        return false;
    block->len = (temp2 << 8) | temp1;
    // Get the last sequence of characters
    if (!extractBytes(buf, block->data, block->len))
        return false;
    // Get number of rules
    if (!extractByte(buf, &(block->rcount)))
        return false;
    for (int i = 0; i < block->rcount; i++) {
        byte code, first, second;
        if (!extractByte(buf, &code)
            || !extractByte(buf, &first)
            || !extractByte(buf, &second))
            return false;
        block->rlist[i] = (Replacement) { code, first, second };
    }
    return true;
}

void compressBlock(Block *block)
{
    // Store sequences and count
    unsigned short *seq[MAX_HEX_VALUE];
    int *count[MAX_HEX_VALUE];
    int *bytesUsed[MAX_HEX_VALUE];
    // Allocate memory
    for (int i = 0; i < MAX_HEX_VALUE; i++) {
        seq[i] = calloc(MAX_HEX_VALUE*MAX_HEX_VALUE*MAX_HEX_VALUE, sizeof(short));
        count[i] = calloc(MAX_HEX_VALUE*MAX_HEX_VALUE*MAX_HEX_VALUE, sizeof(int));
        bytesUsed[i] = calloc(MAX_HEX_VALUE, sizeof(int));
    }
    // Initial Replacement structure
    Replacement replacement = {0x00, 0x00, 0x00};
    while (true) {
        // Find a count of each sequence
        for (int i = 0; i < block->len-1; i++) {
            // Store each byte, check if the byte is unique
            byte b1 = block->data[i], b2 = block->data[i+1];
            if (bytesUsed[(b1 & 0xF0) >> 4][b1 & 0x0F] == 0)
                bytesUsed[(b1 & 0xF0) >> 4][b1 & 0x0F]++;
            if (bytesUsed[(b2 & 0xF0) >> 4][b2 & 0x0F] == 0)
                bytesUsed[(b2 & 0xF0) >> 4][b2 & 0x0F]++;

            // Temporarily store two character elements
            unsigned short pair = (unsigned short) ((b1 << 8) | (b2));
            // Check if the sequence is already stored
            bool found = false;
            int col = 0, index = 0;
            for (int j = 0; j < MAX_HEX_VALUE*MAX_HEX_VALUE*MAX_HEX_VALUE; j++) {
                index = pair >> 12; // Get the first hex value to find which row to index into
                if ((unsigned short) seq[index][j] == pair) { // Found it, return
                    found = true;
                    (count[index][j])++;
                    break;
                } else if (count[index][j] == 0) { // No more sequences to look over, return
                    break;
                }
                col++;
            }
            // If not found, insert into array
            if (!found) {
                seq[index][col] = pair;
                count[index][col] = 1;
            }
        }
        // Find the highest occurrence of a pair
        int max = -1;
        short getReplacedSon = 0x00;
        for (int i = 0; i < MAX_HEX_VALUE; i++) {
            for (int j = 0; j < MAX_HEX_VALUE*MAX_HEX_VALUE*MAX_HEX_VALUE; j++) {
                // check if the count is zero, if so skip it
                if (count[i][j] == 0)
                    break;
                if (count[i][j] > max) {
                    max = count[i][j];
                    getReplacedSon = seq[i][j];
                } else if (count[i][j] == max) {
                    if (seq[i][j] < getReplacedSon)
                        getReplacedSon = seq[i][j];
                }
            }
        }
        // If max occurrence is less than allowed, stop compressing
        if (max < REPLACEMENT_THRESHOLD)
            break;
        // Find the next available byte to replace
        bool ret = false;
        for (int i = 0; i < MAX_HEX_VALUE; i++) {
            for (int j = 0; j < MAX_HEX_VALUE; j++) {
                // Go through each byte used, then compare to the rlist
                if (bytesUsed[i][j] == 0) {
                    // Replace the replacement code
                    replacement.code = (byte) (i << 4 | j);
                    bytesUsed[i][j] = 1;
                    ret = true;
                    break;
                }
            }
            if (ret)
                break;
        }

        // Add sequence to replace into Replacement object
        replacement.first = (getReplacedSon & LEFT_MASK) >> 8;
        replacement.second = getReplacedSon & RIGHT_MASK;
        // Add replacement rule to block
        block->rlist[block->rcount++] = replacement;
        // Now replace, replace, replace!
        for (int i = 0; i < block->len - 1; i++) {
            byte b1 = block->data[i];
            byte b2 = block->data[i+1];
            // Check if this byte sequence needs to be replaced
            if (replacement.first == b1 && replacement.second == b2) {
                // Replace sequence
                block->data[i] = replacement.code;
                // Shift all data to the left
                for (int j = i+1; j < block->len - 1; j++)
                    block->data[j] = block->data[j+1];
                block->len--;
            }
        }
        // Reset sequence arrays
        for (int i = 0; i < MAX_HEX_VALUE; i++) {
            memset(seq[i], 0, sizeof(short) * MAX_HEX_VALUE*MAX_HEX_VALUE*MAX_HEX_VALUE);
            memset(count[i], 0, sizeof(int) * MAX_HEX_VALUE*MAX_HEX_VALUE*MAX_HEX_VALUE);
        }
        // Do it all again!
    }
    // free memory!
    for (int i = 0; i < MAX_HEX_VALUE; i++) {
        free(seq[i]);
        free(count[i]);
        free(bytesUsed[i]);
    }
}

bool uncompressBlock(Block *block)
{
    for (int i = block->rcount - 1; i >= 0; i--) {
        // Look at the last rule of the block to replace said bytes
        Replacement replacement = block->rlist[i];
        // Go through the sequence and replace bytes with specified sequence
        for (int j = 0; j < block->len - 1; j++) {
            if (block->data[j] == replacement.code) {
                // Check if the len will be above the allowed byte limit
                if (block->len + 1 > BLOCK_SIZE_LIMIT)
                    return false;
                // Shift all data to the right
                for (int k = block->len; k > j; k--)
                    block->data[k] = block->data[k-1];
                // Substitute first and second bytes in
                block->data[j] = replacement.first;
                block->data[j+1] = replacement.second;
                block->len++;
            }
        }
    }
    return true;
}

Buffer *compressData(Buffer *src)
{
    Block block = {{'\0'}, 0, {}, 0};
    Buffer *buf = makeBuffer();
    byte bytes[BYTE_LIMIT];
    int bytesSize = 0;
    byte b;
    bool justSerialized = false;

    while (extractByte(src, &b)) {
        justSerialized = false;
        // Add byte to block data
        block.data[block.len++] = b;
        // Check if byte exists in bytes array
        bool check = false;
        for (int i = 0; i < bytesSize; i++) {
            if (bytes[i] == b)
                check = true;
        }
        if (!check)
            if (bytesSize != BYTE_LIMIT)
                bytes[bytesSize++] = b;

        // check if unique bytes value or max bytes value exceeded
        if (block.len > BLOCK_SIZE_LIMIT || bytesSize > BYTE_LIMIT) {
            // Compress block, then serialize it
            compressBlock(&block);
            serializeBlock(&block, buf);
            justSerialized = true;
            // Reset block and bytes
            block.len = 0;
            block.data[block.len++] = b;
            block.rcount = 0;
            bytesSize = 0;
        }
    }

    if (!justSerialized) {
        // One last compress block and serialize it
        compressBlock(&block);
        serializeBlock(&block, buf);
    }

    return buf;
}

Buffer *uncompressData(Buffer *src)
{
    Buffer *buf = makeBuffer();
    Block block = {{}, 0, {}, 0};
    while (src->pos < src->len - 1) {
        // Convert data to block
        if (!deserializeBlock(&block, src))
            return NULL;
        // Now uncompress block
        if (!uncompressBlock(&block))
            return NULL;
        appendBytes(buf, block.data, block.len);
    }
    return buf;
}

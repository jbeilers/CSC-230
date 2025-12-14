/**
 * @file compress.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods for compressing data
 */

#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdbool.h>
#include "buffer.h"

/** Number of different codes a byte can represent. */
#define BYTE_CODES 256

/** Maximum size of a block. */
#define BLOCK_SIZE_LIMIT 16384

/** Masks for specific bytes*/
#define LEFT_MASK 0xFF00
#define RIGHT_MASK 0x00FF

// Normally, I'd hide these two structs in the implementation file,
// rather than exposing them in the header.  I put them here so some
// of the internal functions could be more easily testable.

/** Rule for replacing a pair of bytes with a single-byte code. */
typedef struct {
  /** One-byte code we're replacing the two-byte sequence with. */
  byte code;
  
  /** Two-byte sequence we're replacing. */
  byte first, second;
} Replacement;

/** Representation for a block being compressed or decompressed. */
typedef struct {
  /** Sequence of bytes in this block. */
  byte data[ BLOCK_SIZE_LIMIT ];

  /** Length of the sequence of bytes. */
  unsigned short len;

  /** Sequence of replacement rules. */
  Replacement rlist[ BYTE_CODES - 1 ];

  /** Number of replaement rules for this block. */
  unsigned char rcount;
} Block;

/**
 * Writes block data to a buffer in a specific format
 * @param block Block to serialize
 * @param buf buffer to write to
 */
void serializeBlock(Block *block, Buffer *buf);

/**
 * Takes data from a buffer and stores it in a block structure
 * @param block Block to write data to
 * @param buf buffer containing serialization data
 * @return true: success | false: failure
 */
bool deserializeBlock(Block *block, Buffer *buf);

/**
 * Compresses the data of a block
 * @param block block to be compressed
 */
void compressBlock(Block *block);

/**
 * Uncompresses the data of a block
 * @param block block to have data uncompressed
 * @return true: success | false: failure
 */
bool uncompressBlock(Block *block);

/**
 * Compresses data from a buffer
 * @param src compresses the data from a buffer
 * @return a buffer containing the compressed data
 */
Buffer *compressData(Buffer *src);

/**
 * Compresses data from a buffer
 * @param src buffer with compressed data
 * @return a buffer containing the uncompressed data
 */
Buffer *uncompressData(Buffer *src);

#endif

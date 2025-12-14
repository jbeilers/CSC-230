/**
* @file archive.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods for archive structures
 */

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "IO.h"
#include "compress.h"

typedef struct {
  /** Name and path of the file. */
  char *name;

  /** Raw, uncompressed copy of the file's contents. */
  Buffer *raw;

  /** Comrpessed copy of the file's contents. */
  Buffer *comp;
} FileRec;

/** Incomplete type for the archive. */
typedef struct ArchiveStruct Archive;

/**
 * Creates a new archive
 * @return Pointer to new archive
 */
Archive *makeArchive();

/**
 * Frees the given archive
 * @param arc archive to be freed
 */
void freeArchive(Archive *arc);

/**
 * Creates a new archive from the data of the given file
 * @param fname filename of the archive to be loaded
 * @return Pointer to new archive structure
 */
Archive *loadArchive(char const *fname);

/**
 * Adds a new file to an existing archive
 * @param arc archive to add a file to
 * @param fname filename of the file to be added
 * @return true: success | false: failure
 */
bool addArchive(Archive *arc, char const *fname);

/**
 * Removes a file from the given archive
 * @param arc archive to remove a file from
 * @param fname filename of the file to be removed
 * @return true: success | false: failure
 */
bool removeArchive(Archive *arc, char const *fname);

/**
 * Extracts the data from a file in an archive
 * @param arc archive to extract from
 * @param fname filename of file to extract
 * @return true: success | false: failure
 */
bool extractArchive(Archive *arc, char const *fname);

/**
 * Saves the archive as a file with the given filename
 * @param arc archive to save
 * @param fname filename to save as
 * @return true: success | false: false
 */
bool saveArchive(Archive *arc, char const *fname);

/**
 * Traverses an archive and performs the given function on each filerec node
 * @param arc archive to traverse
 * @param visitor function to call on each filenode
 * @param context helper parameter
 */
void traverseArchive(Archive *arc, void visitor(FileRec *rec, void *context), void *context);

/**
 * Creates a new filerec with the data given from the file
 * @param rec filerec to be created
 * @param fname file to extract data from
 * @return true: success | false: failure
 */
bool makeFileRec(FileRec *rec, char const *fname);

/**
 * frees the given filerec
 * @param rec filerec to be freed
 */
void freeFileRec(FileRec *rec);

/**
 * Compares the current filerec's filename length to the length variable.
 * If it's longer, replace it
 * @param rec filerec
 * @param length current longest filename
 */
void biggestFilename(FileRec *rec, void *length);

/**
 * Prints information about the data of the filerec
 * @param rec filerec to print
 * @param length longest filename in the archive
 */
void printFileInfo(FileRec *rec, void *length);



#endif

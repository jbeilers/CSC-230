/**
* @file archive.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains methods for archive structures
 */

#include "archive.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "compress.h"
#include "error.h"
#include "IO.h"

// Masks for extracting specific bytes
#define MASK1 0xFF000000
#define MASK2 0x00FF0000
#define MASK3 0x0000FF00
#define MASK4 0x000000FF

// Arbitrary filename length
#define FILENAME_LENGTH 200

/** Representation for a node on a linked list of FileRec objects. */
struct FileNodeStruct {
  /** FileRec struct stored inside this node (rather than by pointer). */
  FileRec rec;
  
  /** Pointer to the next node on the linked list. */
  struct FileNodeStruct *next;
};

/** Shorter name for FileNodeStruct */
typedef struct FileNodeStruct FileNode;

/** Representation of an archive, a linked list of FileNodes. */
struct ArchiveStruct {
  /** Head pointer of the head of a list of FileNodes. */
  FileNode *head;
};


Archive *makeArchive()
{
    // Allocate memory
    Archive *a = (Archive *) malloc(sizeof(Archive));
    // Set head to NULL
    a->head = NULL;
    return a;
}

void freeArchive(Archive *arc)
{
    // Save temp pointer as the archive head
    FileNode *temp = arc->head;
    while (temp != NULL) {
        FileNode* next = temp->next;
        // Free the current node and its elements
        freeFileRec(&(temp->rec));
        // Move to the next node
        free(temp);
        temp = next;
    }
    free(arc);
}

Archive *loadArchive(char const *fname)
{
    // Open file and see if its valid
    FILE *fp = fopen(fname, "rb");
    if (!fp) {
        setErrorMessage("Could not find file (Change this)");
        return NULL;
    }

    // Make archive
    Archive *a = makeArchive();
    FileNode *temp = a->head;

    // Get file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    int overall = 0;

    // Now parse file to populate archive
    while (overall < fileSize) {
        // Scan for filename
        char filename[FILENAME_LENGTH];
        char c;
        int count = 0;
        while ((c = fgetc(fp)) != '\0')
            filename[count++] = c;
        filename[count] = '\0';
        // Create filerec and store name
        FileRec *rec = (FileRec *) malloc(sizeof(FileRec));
        if (!makeFileRec(rec, fname))
            return NULL;
        strncpy(rec->name, filename, count + 1);
        overall += count + 1;

        // Scan next 4 bytes for filesize and store it
        byte b1, b2, b3, b4;
        fscanf(fp, "%c %c %c %c", &b1, &b2, &b3, &b4);
        rec->comp->len = (b4<<24)|(b3<<16)|(b2<<8)|b1;
        overall += 4;

        // Scan the compressed data and store in buffer
        int size = rec->comp->len;
        byte buf[size];
        fread(buf, sizeof(byte), size, fp);
        // Append to compressed buffer
        rec->comp = makeBuffer();
        appendBytes(rec->comp, buf, size);
        overall += size;

        // Decompress the compressed data and store it
        rec->raw = uncompressData(rec->comp);

        // store filerec in a filenode
        FileNode *fn = (FileNode *) malloc(sizeof(FileNode));
        fn->rec = *rec;
        fn->next = NULL;

        // Set the current node to the newly created one, and move on
        if (a->head == NULL) {
            a->head = fn;
            temp = a->head;
            continue;
        }
        temp->next = fn;
        temp = temp->next;
    }
    return a;
}

bool addArchive(Archive *arc, char const *fname)
{
    // Create a new FileRec object
    FileRec *rec = (FileRec *) malloc(sizeof(FileRec));
    // Set Filerec fields
    if (!makeFileRec(rec, fname))
        return false;

    // Now create a new FileNode object
    FileNode *fn = (FileNode *) malloc(sizeof(FileNode));
    fn->rec = *rec;
    fn->next = NULL;

    // Now add to archive
    FileNode *temp = arc->head;
    if (temp == NULL) { // If no filenodes
        arc->head = fn;
        return true;
    }
    // Add it in the appropriate position lexicographically
    FileNode *prev = arc->head;
    while (temp != NULL) {
        if (strcmp(fname, temp->rec.name) <= 0) {
            // Check if the name already exists
            if (strcmp(temp->rec.name, fname) == 0)
                return false;
            // Otherwise insert the node
            fn->next = temp;
            if (prev != arc->head)
                prev->next = fn;
            else // Insert at beginning
                arc->head = fn;
            return true;
        }
        prev = temp;
        temp = temp->next; // Move on to the next node
    }
    // Insert last
    prev->next = fn;

    return true;
}

bool removeArchive(Archive *arc, char const *fname)
{
    // Look through each filenode to find the filename
    FileNode *temp = arc->head, *prev = temp;
    // Check if it's the first element
    if (strcmp(fname, arc->head->rec.name) == 0) {
        arc->head = arc->head->next;
        return true;
    }
    while (temp != NULL) {
        // Check if the filename matches
        if (strcmp(fname, temp->rec.name) == 0) {
            // Found it! Now obliterate it
            prev->next = temp->next;
            free(temp);
            return true;
        }
        // Move on
        prev = temp;
        temp = temp->next;
    }
    return false; // Didn't find it :(
}

bool extractArchive(Archive *arc, char const *fname)
{
    // Open file and check if it errored
    FILE *fp = fopen(fname, "wb");
    if (!fp) {
        setErrorMessage("Change this");
        return false;
    }
    // Go through each filenode of archive
    FileNode *temp = arc->head;
    while (temp != NULL) {
        // If the filenames match, write to file and exit loop
        if (strcmp(temp->rec.name, fname) == 0) {
            fwrite(temp->rec.raw->data, sizeof(byte), temp->rec.raw->len, fp);
            fclose(fp);
            return true;
        }
        temp = temp->next;
    }
    fclose(fp);
    return false;
}

bool saveArchive(Archive *arc, char const *fname)
{
    // Open file and check if it errored
    FILE *fp = fopen(fname, "wb");
    if (!fp) {
        setErrorMessage("Change this");
        return false;
    }
    // Go through each filenode of archive
    FileNode *temp = arc->head;
    Buffer *buf = makeBuffer();
    while (temp != NULL) {
        // Reset Buffer
        buf->len = 0;
        // Write filename to buffer
        appendBytes(buf, temp->rec.name, strlen(temp->rec.name) + 1);
        // fwrite(temp->rec.name, sizeof(byte), strlen(temp->rec.name), fp);
        // Write size of file in LSB format
        int compLen = temp->rec.comp->len;
        // Convert to bytes
        byte lsb[4] = {compLen & MASK4, (compLen & MASK3) >> 8, (compLen & MASK2) >> 16, (compLen & MASK1) >> 24};
        appendBytes(buf, lsb, sizeof(byte) * 4);
        // fwrite(lsb, sizeof(byte), 4, fp);
        // Write compressed data to file)
        writeFile(fname, buf);
        writeFile(fname, temp->rec.comp);
        temp = temp->next;
    }
    free(buf);
    fclose(fp);

    return true;
}

void traverseArchive(Archive *arc, void visitor(FileRec *rec, void *context), void *context)
{
    // Store temporary filenode
    FileNode *temp = arc->head;
    // Traverse all filenodes in archive
    while (temp != NULL) {
        // Call function on current filenode's filerec
        visitor(&temp->rec, context);
        // Go to the next filenode
        temp = temp->next;
    }
}


bool makeFileRec(FileRec *rec, char const *fname)
{
    // copy filename to name field
    rec->name = (char *) malloc((strlen(fname) + 1) * sizeof(char));
    strcpy(rec->name, fname);
    // Add raw file to raw field
    rec->raw = readFile(fname);

    // Check if raw is NULL
    if (!rec->raw) {
        // change error message
        setErrorMessage("Could not read file (EDIT)");
        return false;
    }
    // Add compressed file to comp field
    rec->comp = compressData(rec->raw);
    // Check if comp is NULL
    if (!rec->comp) {
        // Change error message
        setErrorMessage("Could not compress data (EDIT)");
        return false;
    }

    return true;
}

void freeFileRec(FileRec *rec)
{
    // free(rec->name);
    free(rec->raw->data);
    free(rec->raw);
    free(rec->comp->data);
    free(rec->comp);
}

void biggestFilename(FileRec *rec, void *length)
{
    int *len = (int *) length;
    if (strlen(rec->name) > *len)
        *len = strlen(rec->name);
    length = len;
}

void printFileInfo(FileRec *rec, void *length)
{
    int *len = (int *) length;
    // Print rec data
    printf("%s", rec->name);
    // Pad with spaces
    for (int i = 0; i < (*len) - strlen(rec->name); i++)
        putchar(' ');
    // Get the size of the file, not the data
    printf(" %8d %8d\n", rec->raw->len, rec->comp->len);
}
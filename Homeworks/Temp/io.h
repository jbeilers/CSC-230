//
// Created by jeile on 10/29/2025.
//

#ifndef HW4_IO_H
#define HW4_IO_H

#include "field.h"

#define RETURN_CHAR 0x0d

byte *readBinaryFile(char const *filename, int *size);

void writeBinaryFile(char const *filename, byte *data, int size);

#endif //HW4_IO_H


/**
 *@file util.c
 *@author Jordan B. Eilers (jbeilers)
 * This file contains functions that have general utility for assisting program functions
 */

#include <string.h>

#include "util.h"

void removeSubstr(char str[], int pos, int len)
{
    // Initialize temporary string
    char temp[strlen(str) + 1];

    // Copy string that's after the index of pos+len
    strcpy(temp, str + pos + len);
    str[pos] = '\0'; // Add null terminator
    strcat(str, temp); // Concatenate temporary string back to str
}

void insertSubstr(char str[], int pos, char const substr[])
{
    // Initialize temporary string
    char temp[(strlen(str) + 1) + (strlen(substr) + 1)];

    // Copy string that's at the pos index
    strcpy(temp, str + pos);
    str[pos] = '\0'; // Add null terminator
    // Concatenate substr then temp to str to complete word
    strcat(str, substr);
    strcat(str, temp);
}


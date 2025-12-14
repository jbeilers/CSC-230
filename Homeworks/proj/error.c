/**
* @file error.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains information on storing global error messages, like errno
 */

#include "error.h"
#include <stdlib.h>
#include <string.h>

/** global error message */
static char *gMessage = NULL;

bool setErrorMessage(char const *message)
{
    // Check for NULL parameter
    if (message == NULL) {
        free(gMessage);
        gMessage = NULL;
        return true;
    }
    // Check for initialization
    if (gMessage == NULL) {
        gMessage = (char *) malloc(sizeof(char) * (strlen(message) + 1));
        strcpy(gMessage, message);
        return true;
    }
    // Check if gMessage needs to be resized
    if (strlen(message) > strlen(gMessage)) {
        gMessage = (char *) realloc(gMessage, sizeof(char) * (strlen(message) + 1));
        // If gMessage is now NULL, return false
        if (gMessage == NULL)
            return false;
    }
    // Copy string and return true
    strcpy(gMessage, message);
    return true;
}

char const *getErrorMessage()
{
    return gMessage;
}
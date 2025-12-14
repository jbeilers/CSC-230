/**
* @file error.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains information on storing global error messages, like errno
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

/**
 * Sets the global error message
 * @param message error message
 * @return true: set error message correctly | false: failure
 */
bool setErrorMessage(char const *message);

/**
 * Returns the global error message
 * @return global error message
 */
char const *getErrorMessage();

#endif

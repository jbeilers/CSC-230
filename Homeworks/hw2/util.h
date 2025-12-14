/**
 *@file util.h
 *@author Jordan B. Eilers (jbeilers)
 * This file contains functions that have general utility for assisting program functions
 */

/**
 * Removes a substring within str
 * @param str string of which substring will be removed from
 * @param pos starting index of the substring
 * @param len length of substring
 */
void removeSubstr(char str[], int pos, int len);

/**
 *
 * @param str string of which substring will be inserted in
 * @param pos starting index if insertion
 * @param substr substring to be inserted
 */
void insertSubstr(char str[], int pos, char const substr[]);




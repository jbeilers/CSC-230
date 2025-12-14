/**
 *@file correction.h
 *@author Jordan B. Eilers (jbeilers)
 * This file contains functions that assist with processing word corrections
 */

#include <stdbool.h>

/** Maximum length of a word in the correction list. */
#define WORD_LIMIT 20
/** Maximum line length */
#define LINE_LIMIT 80

/**
 * Takes a line of text, corrects any misspellings, and prints it
 * @param line Line of text to be corrected
 */
void scanWords(char line[LINE_LIMIT+1]);

/**
 * Scans for a single line of text from standard input
 * @return Whether the line was successful in scanning
 */
bool scanLine();

/**
 * Scans in a corrections list from standard input
 */
void readCorrections();

/**
 * Stores the misspelled word and its correction in the corrections lists
 * @param target misspelled word
 * @param replacement target's replacement
 * @return True: misspelled word was found in the corrections list | False: not found
 */
bool findCorrection(char const target[WORD_LIMIT + 1], char replacement[WORD_LIMIT + 1]);

/**
 * Checks if the misspelled word, and it's correction is valid to store in the corrections list
 * @param target misspelled word
 * @param replacement target's replacement
 * @return True: the correction is valid | False: Invalid correction
 */
bool isValidCorrection(char const target[WORD_LIMIT + 1], char replacement[WORD_LIMIT + 1]);

/**
 * Uses binary search to wind the word in the corrections list
 * @param word misspelled word to find in corrections list
 * @return Index in the corrections list where word was found
 */
int checkCorrections(char const word[WORD_LIMIT + 1]);


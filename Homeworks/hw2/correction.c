/**
 *@file correction.c
 *@author Jordan B. Eilers (jbeilers)
 * This file contains functions that assist with processing word corrections
 */

#include "correction.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "util.h"

/** Maximum number of words on the correction list. */
#define CORRECTION_LIMIT 50000

/** Exit status for a bad correction list. */
#define INVALID_CLIST_STATUS 100

/** Exit status for a line length of more than 80 characters */
#define INVALID_LINE_LENGTH 101

/** Exit status for invalid line when corrected */
#define INVALID_CORRECTED_DOCUMENT 102

/** Arbitrary line length for storing temporary lines */
#define ARBITRARY_LINE_LENGTH 200

/** Arbitrary word length for storing temporary words */
#define ARBITRARY_WORD_LENGTH 50

/** List of misspelled words */
static char misspell[CORRECTION_LIMIT][WORD_LIMIT];
/** List of misspelled words' replacement */
static char corrections[CORRECTION_LIMIT][WORD_LIMIT];

/** Keeps track of the intended number of corrections */
static int count;

/** Keeps track of the current number of corrections */
static int varCount = 0;

void scanWords(char line[LINE_LIMIT+1])
{
    // Initialize variables
    char word[ARBITRARY_WORD_LENGTH+1], temp[WORD_LIMIT+1], tempLine[ARBITRARY_LINE_LENGTH+1];
    int wIndex = 0;

    // Copy line to tempLine
    for (int i = 0; i <= strlen(line); i++)
        tempLine[i] = line[i];

    // Parse line for each word, separated by whitespace
    for (int i = 0; i <= strlen(tempLine); i++) {
        // Check to see if it has hit a whitespace or something that's not a char or apostrophe
        if (isalpha(tempLine[i]) || tempLine[i] == '\'') { // If not a space, add character to word and continue
            word[wIndex] = tempLine[i];
            wIndex++;
            continue;
        }

        // Now deal with the word
        word[wIndex] = '\0'; // Add null terminator to word
        if (wIndex > WORD_LIMIT) {
            wIndex = 0; // Reset index back to zero
            continue;
        }

        // Find word in misspellings
        int cIndex = checkCorrections(word);
        if (cIndex >= 0) { // If word found, replace it!
            removeSubstr(tempLine, i-strlen(word), strlen(word));
            for (int j = 0; j <= strlen(corrections[cIndex]); j++) { // Go through each letter and copy to temp
                temp[j] = corrections[cIndex][j];
                if (j < wIndex) { // For each character in the misspelled word, capitalize appropriate characters
                    if (isupper(word[j]))
                        temp[j] = toupper(word[j]);
                }
            }
            insertSubstr(tempLine, i-strlen(word), temp);
            i += strlen(corrections[cIndex]) - strlen(word); // Compensate i value
        }
        wIndex = 0; // Reset index back to zero
    }
    // Check to see if line will be too long
    if (strlen(tempLine) > LINE_LIMIT)
        exit(INVALID_CORRECTED_DOCUMENT);
    printf("%s\n", tempLine);
}

bool scanLine()
{
    // Initialize variables
    char line[LINE_LIMIT+1];
    int index = 0;

    // Scans entire line, stores it
    char ch = getchar();
    while (ch != '\n' && ch != EOF) {
        line[index] = ch;
        index++;
        ch = getchar();
    }

    // Perform error checks
    if (index > LINE_LIMIT) // If line larger than allowed, exit with error
        exit(INVALID_LINE_LENGTH);
    if (ch == EOF) // If reached end of file, no line to be read
        return false;
    line[index] = '\0'; // Adds null terminator to end of line
    if (strlen(line) > LINE_LIMIT) // If the line length was above the line limit, exit invalid code
        exit(INVALID_LINE_LENGTH);

    scanWords(line); // Now scan each word in the line
    return true; // Successfully scanned the line
}

void readCorrections()
{
    // Scan number of corrections and store it
    int check = scanf("%d\n", &count);
    if (check != 1) // If invalid scanning, exit with error
        exit(INVALID_CLIST_STATUS);

    // For every correction, scan it and check it
    for (int i = 0; i < count; i++) {
        // Initialize variables
        char target[WORD_LIMIT+1], replacement[WORD_LIMIT+1];
        // Scan correction word
        scanf("%s %s\n", target, replacement);
        if (!findCorrection(target, replacement)) { // checks if the correction is already there
            exit(INVALID_CLIST_STATUS);
        }
        // Update variable counter
        varCount++;
    }
}

bool findCorrection(char const target[WORD_LIMIT + 1], char replacement[WORD_LIMIT + 1])
{
    // Check if the correction is valid
    if (!isValidCorrection(target, replacement))
        return false;

    // Now check to see where the target fits
    if (varCount >= CORRECTION_LIMIT)
        return false;

    // Add correction to correction list
    strcpy(misspell[varCount], target);
    strcpy(corrections[varCount], replacement);
    return true;
}

bool isValidCorrection(char const target[WORD_LIMIT + 1], char replacement[WORD_LIMIT + 1])
{
    // Check for quirks of target & replacement
    // Check for length
    if (strlen(target) > WORD_LIMIT || strlen(replacement) > WORD_LIMIT)
        return false;
    // Check if the target & replacement are the same string
    if (strcmp(target, replacement) == 0)
        return false;
    // Check for lowercase letters & invalid characters
    for (int i = 0; i < strlen(target); i++) {
        if (isupper(target[i]))
            return false;
        if ( !(isalpha(target[i]) || target[i] == '\''))
            return false;
    }
    for (int i = 0; i < strlen(replacement); i++) {
        if ( !(isalnum(replacement[i]) || replacement[i] == '\'') )
            return false;
    }
    // Check if target exists already and/or replacement exists as a target
    if (checkCorrections(replacement) >= 0 || checkCorrections(target) >= 0)
        return false;
    // Checks if the target should go before the previous one
    if (strcmp(target, misspell[varCount-1]) < 0)
        return false;
    return true;
}

int checkCorrections(char const word[WORD_LIMIT + 1])
{
    // Convert word to lowercase for comparison
    char temp[WORD_LIMIT + 1];
    strcpy(temp, word);
    for (int i = 0; i < strlen(temp); i++) // Convert word to lower
        temp[i] = tolower(temp[i]);

    // Binary search algorithm
    int low = 0, high = varCount-1; // Gets half the index
    while (true) { // Find correction binary search style
        int index = low + (high - low) / 2;
        int comparison = strcmp(temp, misspell[index]);
        if (comparison == 0)
            return index; // Word found in misspell array
        if (comparison < 0) {
            // Word goes before indexed element, half index
            high = index - 1;
        } else { // Word goes after indexed element, add half of index to index
            low = index + 1;
        }
        if (low > high) // Could not find correction
            return -1;
    }
}

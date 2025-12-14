/**
 * @file spellcheck.c
 * @author Jordan B. Eilers (jbeilers)
 * This program, given a list of misspelled words and replacements, will correct the
 * misspellings of a piece of text
 */

#include <stdio.h>
#include <stdlib.h>

#include "correction.h"

/**
 * Main function that scans in corrections from standard input, reads each subsequent line,
 * corrects any misspellings, and prints it to standard output
 * @return EXIT_SUCCESS Program ran successfully
 */
int main()
{
    // Read the corrections list
    readCorrections();
    // Continuously scan the next line of text, will throw necessary errors
    while (scanLine()) {}
    // Program finished successfully
    return EXIT_SUCCESS;
}
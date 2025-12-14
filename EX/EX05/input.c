/**
  Implementation file for the input component, responsible for reading the
  shape thrown by a player.
*/

#include "input.h"

#include <stdio.h>
#include <ctype.h>

// This function is for use by other components.  It should get a prototype
// in the header.  That's where it's javadoc comment would normally go.
char getInitial()
{
  // You need to implement this.
    char ch;

    // Find the first letter
    do {
        ch = getchar();

        if (ch == EOF)
            return '\0';

    } while (!isalpha(ch));

    char play = ch; // Assign first letter

    // Read rest of word until it reaches whitespace, EOF, or \n
    while (ch != ' ' && ch != EOF && ch != '\n')
        ch = getchar();

    return play;
}

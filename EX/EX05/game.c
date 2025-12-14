/** 
  Implementation file for the game component, responsible for
  determining the winner of a rock-paper-scissors game.
*/

#include "summary.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>

/** 
 Called when the program gets a word starting with an invalid letter.
 It prints an error message and exits the program unsuccessfully.
 
 This funciton is static, so it's not visible to other components.
 It's for internal use only by this component, and it shouldn't not
 get a prototype in the header.
 */
static void invalid()
{
  printf( "Invalid character\n" );
  exit( EXIT_FAILURE );
}

// This function is for use by other components.  It should get a prototype
// in the header.  That's where it's javadoc comment would normally go.
void decideGame( char alet, char blet )
{
  // You'll need to implement this function.
    if (alet == blet) {
        TieTotal++;
        printf("Players tie\n");
        return;
    }

    switch ( alet) {
        case 'r':
            if (blet == 'p') {
                BWinTotal++;
                printf("Player B wins\n");
            }
            else if (blet == 's'){
                AWinTotal++;
                printf("Player A wins\n");
            }
            else
                invalid();
            break;
        case 'p':
            if (blet == 's') {
                BWinTotal++;
                printf("Player B wins\n");
            }
            else if (blet == 'r') {
                AWinTotal++;
                printf("Player A wins\n");
            }
            else
                invalid();
            break;
        case 's':
            if (blet == 'r') {
                BWinTotal++;
                printf("Player B wins\n");
            }
            else if (blet == 'p') {
                AWinTotal++;
                printf("Player A wins\n");
            }
            else
                invalid();
            break;
        default:
        invalid();
  }
}
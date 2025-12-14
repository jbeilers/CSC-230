/* 
   @file main.c
   @author CSC 230 (csc230)
   Program to print out a score report for a sequence of events.
*/

#include <stdio.h>
#include <stdlib.h>
#include "names.h"
#include "scores.h"
#include "report.h"

/**
   Report the average, high and low score for each event in a list of events.
   @return Exit status for the program.
 */
int main()
{
  // Read the number of events and the number of names.
  int eCount, nCount;
  if ( scanf( "%d%d", &eCount, &nCount ) != 2 )
    fail( "Can't read number of names and number of scores" );

  // Read all the names (into a variable-length array).
  char names[ nCount ][ NAME_LIMIT + 1 ];
  readNames( nCount, names );

  // Read the table of scores (into another variable-length array).
  double scores[ eCount ][ nCount ];
  readScores( eCount, nCount, scores );

  // Report score statistics for each event.
  report( eCount, nCount, names, scores );
  
  return EXIT_SUCCESS;
}

/**
  Implmementation file for the summary component, responsible for storing the
  count of wins and ties and printing the report at the end.
*/

#include "summary.h"

#include <stdio.h>

// Win, lose and tie counters.  These need extern declarations in the header.
int AWinTotal = 0;
int BWinTotal = 0;
int TieTotal = 0;

// Function for use by other components.  This needs a prototype in the
// header so other components know about this function.
void reportSummary()
{
  printf( "+--------+-------+\n" );
  printf( "| A Wins | %5d |\n", AWinTotal );
  printf( "+--------+-------+\n" );
  printf( "| B Wins | %5d |\n", BWinTotal );
  printf( "+--------+-------+\n" );
  printf( "| Ties   | %5d |\n", TieTotal );
  printf( "+--------+-------+\n" );
}

/**
   @file scores.c
   @author CSC 230 (csc230)
   Implementation file for reading a table of scores from standard input.
*/

#include "scores.h"
#include "names.h"
#include <stdio.h>

void readScores( int rows, int cols, double scores[ rows ][ cols ] )
{
  // Reach a rows x cols table of double values.
  for ( int r = 0; r < rows; r++ )
    for ( int c = 0; c < cols; c++ )
      if ( scanf( "%lf", &scores[ r ][ c ] ) != 1 )
        fail( "Can't read double in the table" );
}

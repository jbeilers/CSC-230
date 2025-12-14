/**
   @file names.c
   @author CSC 230 (csc230)
   Implementation file for reading a list of names from standard input.
*/

#include "names.h"
#include <stdlib.h>
#include <stdio.h>

void fail( char message[] )
{
  // Really, we should be printing this error message to stderr.
  printf( "%s\n", message );
  exit( EXIT_FAILURE );
}

void readNames( int n, char names[ n ][ NAME_LIMIT + 1 ] )
{
  // Try to read n names.
  for ( int i = 0; i < n; i++ )
    // Limit each name to NAME_LIMIT characters.
    if ( scanf( "%12s", names[ i ] ) != 1 )
      fail( "Can't read a name in the name list" );
}

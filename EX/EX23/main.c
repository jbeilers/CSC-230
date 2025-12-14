// Test program for the findChar assembly language function.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Prototype for our assembly language funciton.
// It should return the idnex of character ch in the string str.
// Or, it should return -1 if ch doesn't occur in the string.
int findChar( char *str, char ch );

int main( int argc, char *argv[] )
{
  int pos;

  // Try the funciton on a literal string.
  pos = findChar( "All This Time", 'm' );

  // Report the result afterward.
  printf( "Test 1: %d\n", pos );

  // Automated check to make sure it's right.
  assert( pos == 11 );

  // Try the funciton on a literal string that doesn't contain the
  // character.
  pos = findChar( "Still Alive", 's' );

  // Report the result afterward.
  printf( "Test 2: %d\n", pos );

  // Automated check to make sure it's right.
  assert( pos == -1 );

  // Try with a stack-allocated array (shouldn't make a difference)
  char stackString[] = "Artificial Heart";
  pos = findChar( stackString, 'i' );

  // Report the result afterward.
  printf( "Test 3: %d\n", pos );

  // Automated check to make sure it's right.
  assert( pos == 3 );

  // Try with a heap-allocated array (shouldn't make a difference)
  char *heapString = malloc( 100 );
  strcpy( heapString, "Chiron Beta Prime" );
  pos = findChar( heapString, 't' );

  // Report the result afterward.
  printf( "Test 4: %d\n", pos );

  // Automated check to make sure it's right.
  assert( pos == 9 );

  free( heapString );

  return EXIT_SUCCESS;
}

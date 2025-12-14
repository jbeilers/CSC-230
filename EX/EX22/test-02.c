// Test program for the swapGlobals assembly language function.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Global variables to swap.
int globalA = 99;
int globalB = -45;

// Prototype for our assembly language subroutine, so the
// compiler knows how to call it.  This one takes two pointers
// to ints and returns nothing.
void swapGlobals();

int main( int argc, char *argv[] )
{
  // Report the values before the swap.
  printf( "Before variable swap: %d %d\n", globalA, globalB );

  // Swap the contents of the two functions in assembly.
  swapGlobals();

  // Report the values after the swap.
  printf( "After variable swap: %d %d\n", globalA, globalB );

  // Automated check.
  assert( globalA == -45 );
  assert( globalB == 99 );

  return EXIT_SUCCESS;
}

// This program doesn't use the following variable, but the assembly
// language code may expect to link with it, so we need to provide a
// symbol with that name so this test code will build successfully.
int globalVar;


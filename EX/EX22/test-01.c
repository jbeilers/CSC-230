// Test program for the setGlobal assembly language function.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// This variable should be allocated right before the global.  We're
// using it to check to see if memory before the target global gets
// changed.
int earlyPad = -1;

// Here's the global variable that the assembly language subroutine
// is expected to modify.
int globalVar = -1;

// Same as earlyPad, used to check if memory is changed that shouldn't be.
int latePad = -1;

// Prototype for our assembly language subroutine, so the
// compiler knows how to call it.
void setGlobal();

int main( int argc, char *argv[] )
{
  // Report the global before calling the assembly language subroutine.
  printf( "before subroutine: %d\n", globalVar );
  
  // Call the assembly language subroutine.
  setGlobal();

  // Report the value afterward.
  printf( "after subroutine: %d\n", globalVar );

  // Automated check to make sure it's right.
  assert( globalVar == 47 );

  // Check the fencepost values.  If these get changed, you're probably
  // writing more than four bytes to memory.
  assert( earlyPad == -1 );
  assert( latePad == -1 );

  return EXIT_SUCCESS;
}

// This program doesn't use the following variables, but the assembly
// language code may expect to link with them, so we need to provide a
// symbol with that name so this test code will build successfully.
int globalA;
int globalB;


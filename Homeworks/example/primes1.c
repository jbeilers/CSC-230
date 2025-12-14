/**
   A buggy program to print out prime numbers.
   (This one has a divide-by-zero).
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 100

/*
 * Check odd numbers 3 or greater, to see if they're prime.
 */
bool isPrime( long val )
{
  // Check all smaller values as potential factors.
  for ( long f = 0; f * f <= val; f += 2 ) {
    // Is this a factor?
    if ( val % f == 0 ) {
      return false;
    }
  }
  
  return true;
}

/*
 * Starting point for this program.  Test a sequence of values for
 * prime-ness.
 */
int main( void )
{
  // Handle 2 as a special case (this is an error if LIMIT < 2)
  printf( "2\n" );
  
  // Test odd numbers, starting from 3.
  long val = 3;
  while ( val < LIMIT ) {
    // If it's prime, print it out.
    if ( isPrime( val ) ) {
      printf( "%ld\n", val );
    }
    val += 2;
  }
  
  return 0;
}

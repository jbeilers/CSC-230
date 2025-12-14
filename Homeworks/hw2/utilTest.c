/** @file unitTest.c
    Unit test program for the util.c component.
    @author dbsturgi
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "util.h"

/** Number of unit tests executed. */
static int unitTestCount = 0;

/** Number of unit tests passed. */
static int unitTestsPassed = 0;

/** A simple Function for recording the results of a unit test.  It
    reports failed tests and counts successful and unsuccessful tests.
    @param name Unique name for the unit test.
    @param passed True if this unit test passed.
*/
static void testCase( char const name[], bool passed )
{
  unitTestCount += 1;
  if ( passed )
    unitTestsPassed += 1;
  else
    printf( "Failed test: %s\n", name );
}

/** Starting point for program execution. Runs a series of unit tests.
    @return Exit status for the program, zero if the program is successful.
*/
int main()
{
  {
    // Try removing a short string.
    char str[] = "This is a test";
    removeSubstr( str, 5, 3 );

    testCase( "removeSubstr test 1", strcmp( str, "This a test" ) == 0 );
  }
  
  {
    // Remove a substring up to the end.
    char str[] = "This is a test";
    removeSubstr( str, 9, 5 );

    testCase( "removeSubstr test 2", strcmp( str, "This is a" ) == 0 );
  }
  
  {
    // Remove an empty substring.
    char str[] = "This is a test";
    removeSubstr( str, 0, 0 );

    testCase( "removeSubstr test 3", strcmp( str, "This is a test" ) == 0 );
  }
  
  {
    char str[] = "This is a test";
    removeSubstr( str, 0, 14 );

    testCase( "removeSubstr test 4", strcmp( str, "" ) == 0 );
  }
  
  {
    // Try inserting a short string.
    char str[ 20 ] = "This  a test";
    insertSubstr( str, 5, "is" );

    testCase( "insertSubstr test 1", strcmp( str, "This is a test" ) == 0 );
  }
  
  {
    // Try inserting a string at the end.
    char str[ 20 ] = "This is a";
    insertSubstr( str, 9, " test" );

    testCase( "insertSubstr test 2", strcmp( str, "This is a test" ) == 0 );
  }
  
  {
    // Try inserting an empty string.
    char str[ 20 ] = "This is a test";
    insertSubstr( str, 0, "" );

    testCase( "insertSubstr test 3", strcmp( str, "This is a test" ) == 0 );
  }
  
  {
    // Try inserting into an initially empty string.
    char str[ 20 ] = "";
    insertSubstr( str, 0, "This is a test" );

    testCase( "insertSubstr test 4", strcmp( str, "This is a test" ) == 0 );
  }

  // Report the overall unit test results.
  printf( "Passed %d of %d unit tests\n", unitTestsPassed, unitTestCount );
  
  return unitTestsPassed == unitTestCount ? EXIT_SUCCESS : EXIT_FAILURE;
}

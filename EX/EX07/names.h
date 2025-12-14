/**
   @file names.h
   @author CSC 230 (csc230)
   Header file for reading a list of names from standard input.
*/

/** Maximum length of a name */
#define NAME_LIMIT 12

/** 
   Print an error message then exit unsuccessfully.
   @param message Error message to print. 
*/
void fail( char message[] );


/**
   Read a list of n names from standard input and store them in
   the names array.
   @param nCount Number of names to read.
   @param naems Array of names to fill in.
*/
void readNames( int nCount, char names[ nCount ][ NAME_LIMIT + 1 ] );

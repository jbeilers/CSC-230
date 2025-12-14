#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int main( int argc, char *argv[] )
{
  // Check command-line arguments and try to open a file given on the
  // command line.
  FILE *fp;
  if ( argc != 2 || ( fp = fopen( argv[ 1 ], "r" ) ) == NULL ) {
    printf( "usage: listTest <number-file>\n" );
    exit( EXIT_FAILURE );
  }
  
  // First, read from the file, building a list from the front.
  {
    // Structure holding our list.
    List *list = makeList();

    // Read all the values, inserting at the front.
    int val;
    while ( fscanf( fp, "%d", &val ) == 1 )
      insertFront( list, val );
    
    // Print out the list (should be in reverse order given in the file);
    printList( list );

    freeList( list );
  }

  // Rewind the file, so we can read everything again.
  fseek( fp, SEEK_SET, 0 );

  // Now, read from the file, building a list from the back.
  {
    // Structure holding our list.
    List *list = makeList();

    // Read all the values, inserting at the tail.
    int val;
    while ( fscanf( fp, "%d", &val ) == 1 )
      insertBack( list, val );
    
    // Print out the list (should be in the same order as the file);
    printList( list );

    freeList( list );
  }

  // Rewind the file, so we can read everything again.
  fseek( fp, SEEK_SET, 0 );

  // Now, read from the file, building a list in sorted order.
  {
    // Structure holding our list.
    List *list = makeList();

    // Read all the values, inserting at the right place to keep the list sorted.
    int val;
    while ( fscanf( fp, "%d", &val ) == 1 )
      insertSorted( list, val );
    
    // Print out the list (should be in sorted order);
    printList( list );

    freeList( list );
  }

  fclose( fp );

  return EXIT_SUCCESS;
}

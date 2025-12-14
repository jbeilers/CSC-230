#include <stdio.h>
#include <stdlib.h>

// Put your macro definitions here.  That should be all
// you need to do to complete this exercise.

#define DECL_LIST(type, name) \
                  int name ## _cap = 5; \
                  int name ## _len = 0; \
                  type * name = (type *) malloc(name##_cap * sizeof(type)); \

#define CHECK_CAP(type, name)  \
    if (name ## _len == name ## _cap) {\
        name##_cap *= 2; \
        name = (type *) realloc(name, name##_cap * sizeof(type)); \
    \
}
#define SIZE(name) name##_len
#define FOR(var, limit) for (int var = 0; var < limit; var++)
#define SWAP(type, a, b) {\
    type temp = a;\
    a = b;\
    b = temp;\
    }


int main()
{
  // Make a resizable list.
  DECL_LIST( double, list );

  double val;
  while ( scanf( "%lf", &val ) == 1 ) {
    // Grow the list when needed.
    CHECK_CAP( double, list );

    // Add this item to the list
    list[ SIZE( list ) ] = val;
    SIZE( list ) += 1;
  }

  // Bubble-sort the list.
  FOR( i, SIZE( list ) )
    FOR( j, SIZE( list ) - i - 1 )
      if ( list[ j ] > list[ j + 1 ] )
        SWAP( double, list[ j ], list[ j + 1 ] );

  // Print out the resulting, sorted list, one value per line.
  FOR( i, SIZE( list ) )
    printf( "%.2f\n", list[ i ] );
  
  return EXIT_SUCCESS;
}

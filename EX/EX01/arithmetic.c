// Include the stdio header, so you can call printf().  I'll use a comment
// like the following to show where you're expected to add code.

#include <stdio.h>
#include <stdlib.h>

// Write the start of the main function.
int main()
{

  // Declare a variable, sum, with type long (so it will have enough
  // capacity), and initialize it to zero.

  long long sum = 0;
  
  // Use a loop to add up all the integers from 1 to 1000000000 storing the
  // total in the variable, sum.

  for (int i = 1; i <= 1000000000; i++) {
    sum += i;
  }

  // I've written the next line of the code for you, since we haven't
  // yet gone over how to print out numbers.  This prints out a
  // long value stored in the variable sum.

  printf( "%lld\n", sum );

  // Return with successful exit status.

  return EXIT_SUCCESS;
}

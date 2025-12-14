#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LIMIT 80

/** Report that the input is invalid and exit unsuccessfully. */
static void invalid()
{
  printf( "Invalid input\n" );
  exit( EXIT_FAILURE );
}

int main()
{
  // This is the representation of the string we're creating.  It starts out
  // as an empty string.
  char str[ STRING_LIMIT + 1 ] = "";

  // Current length of the string.  We could use strlen(), but it's
  // convenient to also have a variable for this.
  int len = 0;

  // Read the word at the start of each command.
  char in[7];
  int pass = scanf("%s", in);

  while ( pass > 0 ) {
    // Read the operands for the operation.  Make sure it's valid and
    // modify the string if it is.
   
    if (strcmp(in, "insert") == 0) {

        if (len >= STRING_LIMIT)
            invalid();

        int index;
        int indexPass = scanf( "%d", &index );
        if (indexPass <= 0 || index < 0 || index - 1 > len)
            invalid();


        // Now scan for character
        char ch = getchar();
        while (ch < 0x21 || ch > 0x7e || isspace(ch) != 0)
            ch = getchar();


        // Now insert character
        str[len+1] = '\0';

        for (int i = len; i > index; i--)
            str[i] = str[i-1];


        str[index] = ch;
    }
    else if (strcmp(in, "delete") == 0) {

        if (len >= STRING_LIMIT)
            invalid();

        int index;
        int indexPass = scanf( "%d", &index );

        if (indexPass <= 0 || index < 0 || index - 1 > len)
            invalid();

        for (int i = index; i < len; i++)
            str[i] = str[i+1];

    }
    else
          invalid();

    len = strlen(str);
      pass = scanf("%s", in);
    // Print out the current state of the string.
    printf( "%s\n", str );
  }

  return EXIT_SUCCESS;
}

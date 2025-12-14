#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** Maximum length of a words.  Words longer than this are
    truncated. */
#define WORD_MAX 8

/** Record of a word, along with the number of times we've seen it. */
typedef struct WordStruct {
  // Number of occurrences for this word.
  int count;
  
  /// Copy of the word read from the file.
  char str[ WORD_MAX + 1 ];
} Word;

/** Structure representing a whole word list. */
typedef struct {
  // Number of words in the list.
  int count;

  // Array of pointers to words.
  Word **words;
} WordList;

/** Print a usage message, then exit unsuccessfully. */
static void usage()
{
  fprintf( stderr, "usage: wordlist <input-file>\n" );
  exit( EXIT_FAILURE );
}

/** Return true if the given character could be part of a word, either
 a letter or an apostrophe. */
bool wordChar( int ch )
{
  return ( ch >= 'a' && ch <= 'z' ) ||
    ( ch >= 'A' && ch <= 'Z' ) ||
    ch == '\'';
}

/** Return the lower-case version of the given code.  There's a
    standard library function for this, but (when this exercise was
    created) we hadn't seen it yet.*/
int toLower( int ch )
{
  if ( ch >= 'A' && ch <= 'Z' )
    return ch - 'A' + 'a';
  return ch;
}

/** Look through the elements of list to find a record matching str.
    Return its index in the lsit or return -1 if it's not on
    the list. */
int findWord( char const *str, WordList *list )
{
  for ( int i = 0; i < list->count; i++ )
    if ( strcmp( str, list->words[ i ]->str ) == 0 )
      return i;
  return -1;
}

/** Read all the words from the given file, and return them as a
    dynamically allocated instance of WordList. */
WordList *readWords( char const *filename )
{
  // Open the given file.
  FILE *fp = fopen( filename, "r" );
  if ( !fp ) {
    fprintf( stderr, "Can't open file: %s\n", filename );
    usage();
  }

  WordList* list = malloc(sizeof(WordList));
    list->count = 0;
  
  // While reading words, manage array of Word pointers as a resizable array.
  int cap = 2;
  list->words = (Word **) malloc( cap * sizeof( Word * ) );

    for (int i = 0; i < cap; i++ ) {
        list->words[i] = malloc(sizeof( Word * ) );
        list->words[i]->count = 0;
        list->words[i]->str[ 0 ] = '\0';
    }

  int ch = fgetc( fp );
  while ( ch != EOF ) {
    // Skip non-word characters.
    while ( ch != EOF && ! wordChar( ch ) )
      ch = fgetc( fp );

    // Allocate space to read the next word into.
    char *str = (char *) malloc( WORD_MAX + 1);
    
    // Read all the letters and store them as a string the word array.
    int len = 0;
    while ( wordChar( ch ) ) {
      // Discard characters after we reach the capacity of a word.
      if ( len < WORD_MAX )
        str[ len++ ] = toLower( ch );
      ch = fgetc( fp );
    }
    
    // Mark the end of this string.
    str[ len ] = '\0';

    // The most recent word could only be empty if we reached the
    // end-of-file.  If so, return the list.
    if ( len == 0 ) {
      free( str );
      return list;
    }

    // Find this word if it's already on the list.
    int idx = findWord( str, list );
    if ( idx < 0 ) {
      // Enlarge list of Word pointers if needed.
      if ( list->count >= cap ) {
        cap *= 2;
        list->words = (Word **) realloc( list->words, cap * sizeof( Word * ) );
      }

      // Put a new Word struct on the list.
      idx = list->count;
      list->words[ idx ] = (Word *) malloc( sizeof( Word * ) );
      list->count += 1;

      // Initialize it with the new word.
      strcpy( list->words[ idx ]->str, str );
      list->words[ idx ]->count = 0;
    }
    
    // Count another occurrence of this word.
    list->words[ idx ]->count += 1;

    // Free the word we just read.
    free( str );
  }

  // Close the input file and return the word list.
  fclose( fp );
  return list;
}

/** Insertion sort on an array. */
void sortList( WordList *list )
{
  // Put each word in the right place in the list.
  for ( int i = 1; i < list->count; i++ ) {
    Word *w = list->words[ i ];

    // Find where w shold go in the list.
    int pos = i;
    while ( pos > 0 && strcmp( w->str, list->words[ pos - 1 ]->str ) < 0 ) {
      list->words[ pos ] = list->words[ pos - 1 ];
      pos -= 1;
    }
    
    list->words[ pos ] = w;
  }
}

/** Given a sorted list, report the number of occurrences of each word. */
void reportWords( WordList *list )
{
  for ( int i = 0; i < list->count; i++ )
    // Print this word, along with its number of occurrences.
    printf( "%s (%d)\n", list->words[ i ]->str, list->words[ i ]->count );
}

int main( int argc, char *argv[] )
{
  if ( argc != 2 )
    usage();

  // Read al the words from the file given on the command line.
  WordList *list = readWords( argv[ 1 ] );

  // Sort the array of word records.
  sortList( list );

  // Report the number of occurrences of each word.
  reportWords( list );

  // Free the memory for the list of words.
  for ( int i = 0; i < list->count; i++ )
    free( list->words[ i ]);
  free(list->words);
  free( list );

  return EXIT_SUCCESS;
}

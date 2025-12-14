/**
  Main implementation file, responsible for processing all the games and
  printing the summary at the end.
*/
#include <stdlib.h>

// Include headers for other components.
#include "game.h"
#include "input.h"
#include "summary.h"

int main()
{
  // Keep reading games to the end of input.
  char alet, blet;
  while ( ( alet = getInitial() ) != '\0' ) {
    blet = getInitial();

    // Process this game.
    decideGame( alet, blet );
  }

  // Report the summary at the end.
  reportSummary();

  return EXIT_SUCCESS;
}

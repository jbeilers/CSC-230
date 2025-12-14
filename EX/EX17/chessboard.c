/**
  @file chessboard.c
  This program decodes a chessboard description in a compressed representation
  and draws a picture of the chessboard as output.
*/

#include <stdio.h>
#include <wchar.h> // For wide characters.
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/** Maximum number of command-line arguments. */
#define MAX_ARGS 2

/** Width and height of the chessboard. */
#define SIZE 8

/** Piece type value for a pawn. */
#define PAWN 1

/** Piece type value for a knight (shown as N) */
#define NGHT 2

/** Piece type value for a bishop */
#define BSHP 3

/** Piece type value for a rook */
#define ROOK 4

/** Piece type value for a queen */
#define QUEN 5

/** Piece type value for a king */
#define KING 6

/** Representation of a chess piece on the 2D board. */
typedef struct {
  /** True if this is a light-side piece. */
  bool light;
  
  /** Type of the piece, or zero for empty. */
  int type;
} Piece;

/**
  Initiates a blank chessboard.
  @param board Array representing a chessboard.
*/
void initBoard( Piece board[SIZE][SIZE] )
{
  for ( int i = 0; i < SIZE; i++ )
    for ( int j = 0; j < SIZE; j++ )
      board[ i ][ j ] = (Piece) { false, 0 };
}

/**
   Decode a piece from the given input value and fill in the appropriate board
   position.
   @param input Encoding of the piece type and location.
   @param board Representation of the chessboard.
*/
void decodeInput( unsigned short input, Piece board[SIZE][SIZE] )
{
    // Define Masks
    unsigned const short colM = 0x0007, rowM = 0x0038,
                    typeM = 0x01C0, colorM = 0x0200;
                    // unusedM = 0xFC00;
    // Get data
    unsigned const short col = input & colM, row = (input & rowM),
                    type = (input & typeM), color = (input & colorM);
                    // unused = (input & unusedM);
    Piece *piece = &board[row>>3][col];
    piece->light = color >> 9;
    piece->type = type >> 6;
}

/**
  Print the chessboard as ASCII text format. 
  @param board Representation of the chessboard.
*/
void printBoardText( Piece board[SIZE][SIZE] )
{
  for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
          Piece *piece = &board[ i ][ j ];
          bool light = piece->light ? true : false;
          switch (piece->type) {
          case 0:
              printf("-");
              break;
          case PAWN:
              if (light)
                  printf("P");
              else
                  printf("p");
              break;
          case NGHT:
              if (light)
                  printf("N");
              else
                  printf("n");
              break;
          case BSHP:
              if (light)
                  printf("B");
              else
                  printf("b");
              break;
          case ROOK:
              if (light)
                  printf("R");
              else
                  printf("r");
              break;
          case QUEN:
              if (light)
                  printf("Q");
              else
                  printf("q");
              break;
          case KING:
              if (light)
                  printf("K");
              else
                  printf("k");
              break;
          default:
              printf("?");
          }
          if (j < SIZE-1)
              printf(" ");
      }
      printf("\n");
  }
}

/**
   This funciton is just for fun.  It prints the chessboard using unicode
   characters for the chess pieces, rather than letters.  This function has to do
   some things you shouldn't do if you just have to print the board as ACII text.
   @param board Representation of the chessboard.
*/
void printBoardGraphical( Piece board[SIZE][SIZE])
{
  // Wide character code used to print each type of piece.
  static const int symbols[][ 2 ] = { { '-', '-' }, // empty
                                      { 0x2659, 0x265F }, // pawn
                                      { 0x2658, 0x265E }, // knight
                                      { 0x2657, 0x265D }, // bishop
                                      { 0x2656, 0x265C }, // rook
                                      { 0x2655, 0x265B }, // queen
                                      { 0x2654, 0x265A } }; // king
  
  setlocale(LC_CTYPE, "");
  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      // Blanks bttween the pieces.
      if ( c > 0 )
        wprintf( L" " );
      wprintf( L"%lc", symbols[ board[r][c].type ][ board[r][c].light ] );
    }
    wprintf(L"\n");
  }
}

int main(int argc, char *argv[])
{
  // Make an empty chessboard.
  Piece chessboard[SIZE][SIZE];
  initBoard( chessboard );
  
  // Poitner to the function we'll use for printing the board.
  void (*boardPrinter)( Piece board[SIZE][SIZE] ) = printBoardText;

  // Swap out the board printer if the user asks for it.
  if ( argc == MAX_ARGS && strcmp(argv[ 1 ], "-u" ) == 0 )
    boardPrinter = printBoardGraphical;
  

  // Read a sequence of hexadecimal values from standard input, decode them
  // and update the chessboard.

  unsigned short line;
    int check = scanf("%hx\n", &line);
    while (check == 1) {
        decodeInput(line, chessboard);
        check = scanf("%hx\n", &line);
    }

  // Print the board, using the current board printer function.
  boardPrinter( chessboard );
  
  return EXIT_SUCCESS;
}

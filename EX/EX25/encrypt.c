#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

// Print a usage message and exit.
static void usage()
{
  fprintf( stderr, "usage: encrypt <key> <input-filename> <output-filename>\n" );
  exit( EXIT_FAILURE );
}

int main( int argc, char *argv[] )
{
  // Check parameters, and open input and output files.
  if ( argc != 4 )
    usage();

  FILE *infile = fopen( argv[ 2 ], "r" );
  if ( ! infile ) {
    fprintf( stderr, "Can't open input file %s\n", argv[ 2 ] );
    usage();
  }
  
  FILE *outfile = fopen( argv[ 3 ], "wb" );
  if ( ! outfile ) {
    fprintf( stderr, "Can't create output file %s\n", argv[ 3 ] );
    usage();
  }

  // Make an 8-byte copy of the key given on the command line.  The
  // one on the command line might be too short, or too long.  Limit
  // it to just the first 8 bytes if it's too long, and pad with zeros
  // ('\0') if it's not long enough.  This will just be an 8-byte
  // array, not a null-terminated string.
  unsigned char key[ 8 ];
  
  strncpy((char *) key, argv[1], sizeof(key)/sizeof(unsigned char));

  // Fill in the parity bits in the key.
  DES_set_odd_parity( &key );

  // Turn the key into a key schedule object.
  DES_key_schedule schedule;
  DES_set_key_checked( &key, &schedule );
  
  // Read the input file 8 bytes at a time, encrypting each block with
  // the key.  If the last block is too short (i.e., if the file isn't
  // a multiple of 8 bytes in length), pad with zeros.
  unsigned char block[ 8 ];
  int len;
  while ( ( len = fread( block, 1, 8, infile ) ) ) {
    // Pad any short blocks with zeros.

      unsigned char temp[8];
      strncpy((char *) temp, (char *) block, sizeof(temp));
      strncpy((char *) block, (char *) temp, sizeof(temp));
    
    // Encrypt this block with the key schedule.  Encrypting each
    // block independently is what the "ecb" in the "DES_ecb_encrypt"
    // function name means.
    unsigned char output[ 8 ];

    DES_ecb_encrypt(&block, &output, &schedule, DES_ENCRYPT);

    // Write this encrypted, output block out to the output file.

    fwrite(output, sizeof(unsigned char), sizeof(block), outfile);
  }

  // Close the two file and exit.
  fclose( infile );
  fclose( outfile );
  return EXIT_SUCCESS;
}

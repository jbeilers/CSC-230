#!/bin/bash
# This is a shell script to check your program(s) on test cases.
# It usese the same language you normally use to type commands in
# a terminal window.  You can write whole programs in shell.

# Assume we've succeeded until we see otherwise.
FAIL=0

# Print an error message and set the fail flag.
fail() {
    echo "**** $1"
    FAIL=1
}

# Check the exit status.  The actual exit status (ASTATUS) should match
# the expected status (ESTATUS)
checkStatus() {
  ESTATUS="$1"
  ASTATUS="$2"

  # Make sure the program exited with the right exit status.
  if [ "$ASTATUS" -ne "$ESTATUS" ]; then
      fail "FAILED - incorrect program exit status. (expected $ESTATUS,  Got: $ASTATUS)"
      return 1
  fi

  return 0
}

# Check the contents of an a file.  If the expected file (EFILE)
# exists, then the actual file (AFILE) should exist and it should match.
checkFile() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"

  # Make sure we're really expecting this file.
  if [ ! -f "$EFILE" ]; then
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should be empty.
checkFileOrEmpty() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should be
  # empty.
  if [ ! -f "$EFILE" ]; then
      if [ -s "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should be empty."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# The given file should exist but should be empty.
checkEmpty() {
  NAME="$1"
  AFILE="$2"
  
  if [ -s "$AFILE" ]; then
      fail "FAILED - $NAME ($AFILE) should be empty."
      return 1
  fi

  return 0
}

# Test one execution of the encrypt program.
testEncrypt() {
  TESTNO=$1
  ESTATUS=$2

  echo "Encrypt Test $TESTNO"
  rm -f output.bin stderr.txt

  echo "   ./encrypt ${args[@]} output.bin 2> stderr.txt"
  ./encrypt ${args[@]} output.bin 2> stderr.txt
  ASTATUS=$?

  if ! checkStatus "$ESTATUS" "$ASTATUS" ||
     ! checkFile "Ciphertext output" "cipher-$TESTNO.bin" "output.bin" ||
     ! checkFileOrEmpty "Stderr output" "error-$TESTNO.txt" "stderr.txt"
  then
      FAIL=1
      return 1
  fi

  echo "Encrypt Test $TESTNO PASS"
  return 0
}

# Test one execution of the decrypt program.
testDecrypt() {
  TESTNO=$1
  ESTATUS=$2
  EXT=$3

  echo "Decrypt Test $TESTNO"
  rm -f output.$EXT stderr.txt

  echo "   ./decrypt ${args[@]} output.$EXT 2> stderr.txt"
  ./decrypt ${args[@]} output.$EXT 2> stderr.txt
  ASTATUS=$?

  if ! checkStatus "$ESTATUS" "$ASTATUS" ||
     ! checkFile "Plaintext output" "plain-$TESTNO.$EXT" "output.$EXT" ||
     ! checkFileOrEmpty "Stderr output" "error-$TESTNO.txt" "stderr.txt"
  then
      FAIL=1
      return 1
  fi

  echo "Decrypt Test $TESTNO PASS"
  return 0
}

# Get a clean build of the project.
make clean

# Run unit tests for the field component.
echo
echo "Running fieldTest unit tests"
gcc -Wall -std=c99 -g fieldTest.c field.c -o fieldTest

if [ -x fieldTest ]; then
    ./fieldTest
    
    if [ $? -ne 0 ]; then
	echo "**** Your program didn't pass all the fieldTest unit tests."
	FAIL=1
    fi
else
    echo "**** We couldn't build the fieldTest program with your implementation, so we couldn't run these unit tests."
    FAIL=1
fi


# Run unit tests for the aes component.
echo
echo "Running aesTest unit tests"
gcc -Wall -std=c99 -g aesTest.c aes.c aesUtil.c field.c -o aesTest

if [ -x aesTest ]; then
    ./aesTest
    
    if [ $? -ne 0 ]; then
	echo "**** Your program didn't pass all the aesTest unit tests."
	FAIL=1
    fi
else
    echo "**** We couldn't build the aesTest program with your implementation, so we couldn't run these unit tests."
    FAIL=1
fi

# Tests for the encrypt program.
echo
echo "Running encrypt tests"

# Default build should make both the encrypt and decrypt programs.
make

if [ -x encrypt ]; then
    args=(key-01.txt plain-01.txt)
    testEncrypt 01 0
    
    args=(key-02.txt plain-02.bin)
    testEncrypt 02 0
    
    args=(key-03.bin plain-03.bin)
    testEncrypt 03 0
    
    args=(key-04.bin plain-04.bin)
    testEncrypt 04 0
    
    args=(key-05.txt plain-05.txt)
    testEncrypt 05 0
    
    args=(key-06.bin plain-06.bin)
    testEncrypt 06 0
    
    args=(key-07.bin plain-07.txt)
    testEncrypt 07 1
    
    args=(key-08.bin)
    testEncrypt 08 1
else
    fail "Since your encrypt program didn't compile, it couldn't be tested"
fi

# Tests for the decrypt program.
echo
echo "Running decrypt tests"

if [ -x decrypt ]; then
    args=(key-01.txt cipher-01.bin)
    testDecrypt 01 0 txt
    
    args=(key-02.txt cipher-02.bin)
    testDecrypt 02 0 bin
    
    args=(key-03.bin cipher-03.bin)
    testDecrypt 03 0 bin
    
    args=(key-04.bin cipher-04.bin)
    testDecrypt 04 0 bin
    
    args=(key-05.txt cipher-05.bin)
    testDecrypt 05 0 txt
    
    args=(key-06.bin cipher-06.bin)
    testDecrypt 06 0 bin
    
    args=(key-09.bin cipher-09.bin)
    testDecrypt 09 1 bin
else
    fail "Since your decrypt program didn't compile, it couldn't be tested"
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi

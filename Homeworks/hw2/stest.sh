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

# Test the spellcheck program with all input from a single file.
simpleTest() {
    TESTNO="$1"
    ESTATUS="$2"

    echo "Test $TESTNO"
    rm -f output.txt

    echo "   ./spellcheck < input-$TESTNO.txt > output.txt"
    ./spellcheck < input-$TESTNO.txt > output.txt
    ASTATUS=$?

    if ! checkStatus "$ESTATUS" "$ASTATUS"
    then
	return 1
    fi

    if [ "$ESTATUS" -eq 0 ]; then
	if ! checkFileOrEmpty "Terminal output" "expected-$TESTNO.txt" "output.txt"
	then
	    return 1
	fi
    fi

    echo "Test $TESTNO PASS"
    return 0
}

# Test the spellcheck program with all input from a single file.
splitTest() {
    TESTNO="$1"
    CFILE="$2"
    ESTATUS="$3"

    echo "Test $TESTNO"
    rm -f output.txt

    echo "   cat $CFILE document-$TESTNO.txt | ./spellcheck < document-$TESTNO.txt > output.txt"
    cat $CFILE document-$TESTNO.txt | ./spellcheck > output.txt
    ASTATUS=$?

    if ! checkStatus "$ESTATUS" "$ASTATUS"
    then
	return 1
    fi

    if [ "$ESTATUS" -eq 0 ]; then
	if ! checkFileOrEmpty "Terminal output" "expected-$TESTNO.txt" "output.txt"
	then
	    return 1
	fi
    fi

    echo "Test $TESTNO PASS"
    return 0
}

# make a fresh copy of the target programs
make clean
make

if [ $? -ne 0 ]; then
    fail "Make exited unsuccessfully"
fi

# Run test cases
if [ -x spellcheck ]; then
    simpleTest 01 0
    
    simpleTest 02 0
    
    simpleTest 03 0
    
    simpleTest 04 0

    simpleTest 05 0

    simpleTest 06 0

    simpleTest 07 0
    
    simpleTest 08 0
    
    splitTest 09 clist-a.txt 0
    
    splitTest 10 clist-b.txt 0
    
    splitTest 11 clist-b.txt 0
    
    splitTest 12 clist-b.txt 0
    
    simpleTest 13 0
    
    simpleTest 14 0
    
    simpleTest 15 0
    
    simpleTest 16 0
    
    simpleTest 17 100
    
    simpleTest 18 100

    simpleTest 19 100

    simpleTest 20 100

    simpleTest 21 100

    simpleTest 22 101

    simpleTest 23 102
else
    fail "Since your program didn't compile, no tests were run."
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "TESTS SUCCESSFUL"
  exit 0
fi

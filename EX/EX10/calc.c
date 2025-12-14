#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

/** Values of all variables.  For example, var[ 0 ] is the value of
    variable a.  */
long var['z' - 'a' + 1];

/** Given the character code for a lower-case letter, return the value
    of that variable. */
long get(char let)
{
    return var[let - 'a'];
}

/** Given the character code for a lower-case letter, and a long
    value, set the the variable with that name to the given value. */
void set(char let, long val)
{
    var[let - 'a'] = val;
}

/** Given two long values, return false if their sum is an overflow. */
bool checkAdd(long b, long c)
{
    if ((b >= 0 && c >= 0 && b + c < 0) ||
        (b < 0 && c < 0 && b + c >= 0))
        return false;
    return true;
}

/** Given two long values, return false if their difference is an overflow. */
bool checkSub(long b, long c)
{
    // Add code to detect signed overflow during subtraction.
    if ((b < 0 && c >= 0 && b - c > 0) || // -b - c > 0
        (b >= 0 && c < 0 && b - c <= 0)) // b - (-c) < 0
        return false;
    return true;
}

/** Given two long values, return false if their product is an overflow. */
bool checkMul(long b, long c)
{
    if (b > 0 && c > 0) {
        if (b > LONG_MAX / c)
            return false;
    }

    // Add code to handle three other cases, based on the signs of b and c.
    if (b < 0 && c > 0) {
        if (b < LONG_MIN / c)
            return false;
    }

    if (b < 0 && c < 0) {
        if (b < LONG_MAX / c)
            return false;
    }

    if (b > 0 && c < 0) {
        if (b < LONG_MIN / c)
            return false;
    }

    return true;
}

/** Given two long values, return false if their quotient is an overflow. */
bool checkDiv(long b, long c)
{
    // One special case when we can get overflow.
    if (b == LONG_MIN && c == -1)
        return false;

    return true;
}

bool execute(FILE* fp)
{
    // Use ftell to remember where we are in the input file.
    long location = ftell(fp);

    // Short strings for parsing variable names.
    char aStr[2], bStr[2], cStr[2];

    if (fscanf(fp, "%1[a-z] = %1[0-9]", aStr, bStr) == 2) {
        set(*aStr, get(*bStr));
        return true;
    }
    fseek(fp, location, SEEK_CUR);

    // Try to match a statement of the form: a = b + c for any variable names.
    if (fscanf(fp, " %1[a-z] = %1[a-z] + %1[a-z]",
               aStr, bStr, cStr) == 3) {
        // Print an error message if there's an overflow.  We're doing
        // something a little sneaky here.  The variable names are stored
        // in length-1 strings.  Dereferencing the array name will give us
        // the value of just the first character in the string.
        if (!checkAdd(get(*bStr), get(*cStr)))
            fprintf(stderr, "Overflow: %s + %s\n", bStr, cStr);
        else
            set(*aStr, get(*bStr) + get(*cStr));

        return true;
    }
    fseek(fp, 0, location);
    // Add code to match other statement types and check for errors.
    // returning to the same location in the input file before trying to
    // parse.
    if (fscanf(fp, " %1[a-z] = %1[a-z] - %1[a-z]", aStr, bStr, cStr) == 3) {
        if (!checkSub(get(*bStr), get(*cStr))) {
            fprintf(stderr, "Overflow: %s + %s\n", bStr, cStr);
        }
        else {
            set(*aStr, get(*bStr) - get(*cStr));
        }
        return true;
    }
    fseek(fp, location, SEEK_CUR);

    if (fscanf(fp, " %1[a-z] = %1[a-z] * %1[a-z]", aStr, bStr, cStr) == 3) {
        if (!checkMul(get(*bStr), get(*cStr))) {
            fprintf(stderr, "Overflow: %s * %s\n", bStr, cStr);
        }
        else {
            set(*aStr, get(*bStr) * get(*cStr));
        }
        return true;
    }
    fseek(fp, location, SEEK_CUR);

    if (fscanf(fp, " %1[a-z] = %1[a-z] / %1[a-z]", aStr, bStr, cStr) == 3) {
        if (!checkDiv(get(*bStr), get(*cStr))) {
            fprintf(stderr, "Overflow: %s / %s\n", bStr, cStr);
        }
        else {
            set(*aStr, get(*bStr) / get(*cStr));
        }
        return true;
    }
    fseek(fp, location, SEEK_CUR);

    if (fscanf(fp, "%1[a-z] = %1[0-9]", aStr, bStr) == 2) {
        set(*aStr, get(*bStr));
        return true;
    }
    fseek(fp, location, SEEK_CUR);

    if (fscanf(fp, "%1[a-z] = %1[-] %1[0-9]\n", aStr, bStr, cStr) == 3) {
        set(*aStr, get(*cStr) * -1);
        return true;
    }

    // If we didn't match anything, it's either an EOF or a syntax error.
    if (!feof(fp)) {
        fprintf(stderr, "Syntax error\n");
        exit(EXIT_FAILURE);
    }
    return false;
}

int main()
{
    FILE* fp;

    // Open script.txt for reading and report an error message if unsuccessful.
    fp = fopen("script-4.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open script.txt\n");
        exit(EXIT_FAILURE);
    }

    // Execute statements until we hit EOF or a syntax error.
    while (execute(fp));

    // Close the input file and exit succcessfully (even if we hit an error in the
    // script).
    fclose(fp);

    return EXIT_SUCCESS;
}

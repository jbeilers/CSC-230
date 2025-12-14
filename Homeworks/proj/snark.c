/**
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <setjmp.h>
#include "archive.h"
#include "error.h"

/** Minimum name length for showing the contents of an archive. */
#define MIN_NAME_LEN 20

/** Arbitrary command line argument length */
#define ARGUMENT_LEN 1024
/** Maximum valid command length */
#define MAX_COMMAND_LEN 7
/** Arbitrary filename length */
#define FILENAME_LEN (ARGUMENT_LEN - MAX_COMMAND_LEN + 1)
/** When we want the program to exit on an error */
#define EXITPROGRAM true
/** When we don't want the program to exit on an error */
#define NOEXITPROGRAM false

/**
 * Check if the global error message is not null
 * If it isn't, print the message and exit if desired
 * @param exitProgram whether to exit the program or not
 */
void errorCheck(bool exitProgram)
{
    if (getErrorMessage() != NULL) {
        fprintf(stderr, "%s\n", getErrorMessage());
        if (exitProgram)
            exit(EXIT_FAILURE);
        setErrorMessage(NULL);
    }
}

/**
 * Handles the commands of the command line
 * @param arc archive
 * @param line command line arguments
 * @param env environment to jump back to
 * @return true: exit the program | false: continue with the next command
 */
bool handleCommand(Archive *arc, char *line, jmp_buf *env)
{
    // Variables for input
    char command[MAX_COMMAND_LEN + 1], filename[FILENAME_LEN + 1], extra;

    // Initial scan
    sscanf(line, " %[^\n]", line);

    // Scan for command
    int check = sscanf(line, "%7s %s %c", command, filename, &extra);

    // Check for a space line or a commented line
    if (line[0] == '\0' || line[0] == '#')
        return false;

    // Check for validity of scanned input
    if (check != 1 && check != 2) {
        setErrorMessage("Invalid command");
        longjmp(*env, 1);
    }

    // Check if command is valid
    if (strcmp(command, "add") == 0 && check == 2) {
        if (!addArchive(arc, filename))
            longjmp(*env, 1);
    } else if (strcmp(command, "remove") == 0 && check == 2) {
        if (!removeArchive(arc, filename))
            longjmp(*env, 1);
    } else if (strcmp(command, "extract") == 0 && check == 2) {
        if (!extractArchive(arc, filename))
            longjmp(*env, 1);
    } else if (strcmp(command, "show") == 0 && check == 1) {
        // Find the longest filename in the archive
        int longest = 0;
        traverseArchive(arc, &biggestFilename, &longest);
        // Now print them all
        if (longest == 0)
            printf("Archive is empty\n");
        else {
            if (longest < MIN_NAME_LEN) // Increase spacing to min name length
                longest = MIN_NAME_LEN;
            // Header
            printf("File");
            for (int i = 0; i < longest - 4; i++)
                putchar(' ');
            printf(" %8s %8s\n", "orig", "comp");
            traverseArchive(arc, &printFileInfo, &longest);
        }

    } else if (strcmp(command, "save") == 0 && check == 2) {
        if (!saveArchive(arc, filename))
            longjmp(*env, 1);
    } else if (strcmp(command, "quit") == 0 && check == 1) {
        return true;
    } else {
        setErrorMessage("Invalid command");
        longjmp(*env, 1);
    }
    return false;
}

/**
 * Reads and performs commands from a file
 * @param a archive
 * @param fname filename to load commands from
 */
void scriptMode(Archive *a, char const *fname)
{
    // Create jmp_buf variable
    jmp_buf env;

    // Open file
    FILE *fp = fopen(fname, "r");
    if (!fp)
        setErrorMessage("Could not open file (Change this)");
    errorCheck(EXITPROGRAM);

    // Now read each line of the input file
    while (!feof(fp)) {
        char *line = readLine(fp); // Line from input txt file
        int val = setjmp(env);
        // Error handling
        if (val == 0) {
            if (handleCommand(a, line, &env)) { // If quit command, return true
                freeArchive(a);
                free(line);
                fclose(fp);
                return;
            }
        } else
            errorCheck(EXITPROGRAM); // Error occurred, quit the program
    }
    fclose(fp);
}

/**
 * reads commands from stdin
 * @param a archive
 */
void interactiveMode(Archive *a)
{
    jmp_buf env;

    // Now scan user input
    char line[ARGUMENT_LEN + 1];
    int lineLength = 0;
    while (true) {
        int val = setjmp(env);
        printf("cmd> ");
        // Scan user input
        char c;
        while ((c = getchar()) != '\n' && c != EOF)
            line[lineLength++] = c;
        line[lineLength++] = '\0';
        line[lineLength] = '\n';
        if (c == '\377')
            return;

        if (val == 0) {
            printf("%s\n", line);
            if (handleCommand(a, line, &env)) {
                freeArchive(a);
                return;
            }
        } else
            errorCheck(NOEXITPROGRAM); // Error occurred, try again!
        lineLength = 0;
    }
}

/**
 * Checks the input and performs actions accordingly
 * @param argc num of arguments in
 * @param argv command line arguments
 */
void checkInput(int argc, char* argv[])
{
    // Go through each program argument
    switch (argc) {
    case 1: // No flags
        interactiveMode(makeArchive());
        break;
    case 3: // Either flag, check which one!
        if (strcmp(argv[1], "-s") == 0) // Script flag
            scriptMode(makeArchive(), argv[2]);
        else if (strcmp(argv[1], "-a") == 0) // Load archive
            interactiveMode(loadArchive(argv[2]));
        else
            setErrorMessage("usage: snark [-a archive-file] [-s script-file]");
        break;
    case 5: // Both archive and script flag
        // Check which position has each flag, then run script mode
        if (strcmp(argv[1], "-s") == 0) {
            if (strcmp(argv[3], "-a") == 0)
                scriptMode(loadArchive(argv[4]), argv[2]);
            else
                setErrorMessage("usage: snark [-a archive-file] [-s script-file]");
        } else if (strcmp(argv[1], "-a") == 0) {
            if (strcmp(argv[3], "-s") == 0)
                scriptMode(loadArchive(argv[2]), argv[4]);
            else
                setErrorMessage("usage: snark [-a archive-file] [-s script-file]");
        } else {
            setErrorMessage("usage: snark [-a archive-file] [-s script-file]");
        }
        break;
    default:
        setErrorMessage("usage: snark [-a archive-file] [-s script-file]");
    }
    errorCheck(EXITPROGRAM);
}

/**
 * This main function reads the command line inputs and performs the
 * appropriate actions
 * @param argc number of arguments in
 * @param argv command line arguments
 * @return exit code
 */
int main(int argc, char* argv[])
{
    // Check user input, run appropriate program mode
    checkInput(argc, argv);
    // Runtime returns here when user quits
    exit(EXIT_SUCCESS);
}
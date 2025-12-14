/**
 * @file parklist.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains functions that pertain to the creating, reading, sorting, and printing of a ParkList struct
 */

#ifndef HW3_PARKLIST_H
#define HW3_PARKLIST_H
#include <stdbool.h>

#endif //HW3_PARKLIST_H

/** Max park name length */
#define PARK_NAME_LENGTH 40

/** Initial parklist list capacity */
#define INITIAL_PARKLIST_CAPACITY 5

/**
 * Park Structure with fields:
 * id: Park id
 * name: Park name
 * lat: Park latitude
 * lon: Park longitude
 * distance: Distance from target city
 */
typedef struct Park {
    int id;
    char name[PARK_NAME_LENGTH + 1];
    double lat;
    double lon;
    double distance;
} Park;

/**
 * Structure ParkList with fields:
 * list: List of pointers to Parks
 * count: Number of park pointers currently in list
 * capacity: Max number of park pointers allowed in the list
 */
typedef struct ParkList {
    Park **list;
    int count;
    int capacity;
} ParkList;

/**
 * Creates and initializes the fields of a ParkList structure
 * @return ParkList Structure
 */
ParkList *makeParkList();

/**
 * Frees fields of parkList & itself
 * @param parkList ParkList pointer to be freed
 */
void freeParkList (ParkList *parkList);

/**
 * Reads park data from a file and stores new park pointers in parkList's list field
 * @param filename File to be parsed
 * @param parkList ParkList to store parks in
 */
void readParks(char const *filename, ParkList *parkList);

/**
 * Sorts the list of park pointers in parkList using the logic provided by the compare function parameter
 * @param parkList ParkList to be sorted
 * @param compare Compare function to help with sorting
 */
void sortParks(ParkList *parkList, int (*compare) (void const *va, void const *vb));

/**
 * Lists parks to stdout that are marked true by the test function parameter
 * @param parkList Park List to print parks from
 * @param test Function pointer to determine parks to print
 * @param str String to determine which parks to print
 */
void listParks(ParkList *parkList, bool (*test)(Park const *park, char const *str), char const *str);

/**
 * Test function that returns true for all parks
 * @param park Park to be tested
 * @param str String to be used for testing logic
 * @return True: Print this Park | False: Don't print this park
 */
bool listAllParks(Park const *park, char const *str);

/**
 * Returns true for parks with given str pattern in their names
 * @param park Park to be tested
 * @param str String to be used for testing logic
 * @return True: Print this Park | False: Don't print this park
 */
bool searchParksByString(Park const *park, char const *str);

/**
 * Returns true for parks within a specified distance from target city
 * @param park Park to be tested
 * @param str String to be used for testing logic
 * @return True: Print this Park | False: Don't print this park
 */
bool listParksWithinDistance(Park const *park, char const *str);
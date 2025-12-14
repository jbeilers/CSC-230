/**
* @file parklist.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains functions that pertain to the creating, reading, sorting, and printing of a ParkList struct
 */

#include "parklist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

ParkList *makeParkList()
{
    // Allocate memory for new ParkList
    ParkList* parklist = (ParkList*) malloc(sizeof(ParkList));
    // Initialize fields
    parklist->count = 0;
    parklist->capacity = INITIAL_PARKLIST_CAPACITY;
    parklist->list = calloc(parklist->capacity, sizeof(Park *));
    return parklist;
}

void freeParkList (ParkList *parkList)
{
    // Free each individual park
    for (int i = 0; i < parkList->count; i++)
        free((parkList->list)[i]);
    // Free the list of pointers to parks
    free(parkList->list);
    // Free the park list structure
    free(parkList);
}

void readParks(char const *filename, ParkList *parkList)
{
    // File to read
    FILE* fp = fopen(filename, "r");

    while (!feof(fp)) {

        // Store park id, location, and name
        char name[PARK_NAME_LENGTH + 1];
        double lat, lon;
        int id;

        // Scan for data
        char *line1 = readLine(fp);
        char *line2 = readLine(fp);
        const int check1 = sscanf(line1, "%d%lf%lf\n", &id, &lat, &lon);
        const int check2 = sscanf(line2, "%[^\n]\n", name);
        free(line1);
        free(line2);

        // Check validity of data
        if (check1 != 3 || check2 != 1
            || (strnlen(name, PARK_NAME_LENGTH + 1) == PARK_NAME_LENGTH + 1
            && name[PARK_NAME_LENGTH] != '\0')) {
            fprintf(stderr, "Invalid park file: %s\n", filename);
            exit(EXIT_FAILURE);
        }

        // Check to see if ID already exists in parklist
        for (int i = 0; i < parkList->count; i++) {
            Park *p = (parkList->list)[i];
            if (p->id == id) {
                fprintf(stderr, "Invalid park file: %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }

        // check if parkList list needs to be expanded
        if (parkList->count == parkList->capacity) {
            // Double the size of park list
            parkList->list = (Park **) realloc(parkList->list, sizeof(Park *) * (parkList->capacity*=2));
        }

        // Allocate memory for park
        (parkList->list)[parkList->count] = malloc(sizeof(Park));
        Park* park = (parkList->list)[parkList->count];

        // Store scanned data into Park
        park->id = id;
        park->lat = lat;
        park->lon = lon;
        park->distance = 0;
        strncpy(park->name, name, PARK_NAME_LENGTH + 1);

        parkList->count++; // Increment park list count
    }

    // close file pointer
    fclose(fp);
}

void sortParks(ParkList *parkList, int (*compare) (void const *va, void const *vb))
{
    // Sort list of parks
    qsort(parkList->list, parkList->count, sizeof(Park *), compare);
}

void listParks(ParkList *parkList, bool (*test)(Park const *park, char const *str), char const *str)
{
    // If str is a distance value
    if (str != NULL && strtod(str, NULL) != 0) {
        fprintf(stdout, "%-3s %-40s %6s\n", "ID", "Name", "Miles");
        fflush(stdout);
        for (int i = 0; i < parkList->count; i++) {
            Park *park = (parkList->list)[i];
            if (test(park, str))
                fprintf(stdout, "%3d %-40s %6.2f\n", park->id, park->name, park->distance);
        }
    } else { // Print standard format
        fprintf(stdout, "%-3s %-40s %8s %8s\n", "ID", "Name", "Lat", "Lon");
        fflush(stdout);
        for (int i = 0; i < parkList->count; i++) {
            Park *park = (parkList->list)[i];
            if (test(park, str)) {
                fprintf(stdout, "%3d %-40s %8.3f %8.3f\n", park->id, park->name, park->lat, park->lon);
            }
        }
    }
}

bool listAllParks(Park const *park, char const *str)
{
    // return true regardless
    return true;
}

bool searchParksByString(Park const *park, char const *str)
{
    // If the park doesn't contain the str pattern, return false
    const char *index = strstr(park->name, str);
    if (index == NULL || strcmp(park->name, index) == 0)
        return false;
    return true;
}

bool listParksWithinDistance(Park const *park, char const *str)
{
    // If the park is not within distance of the target city, return false
    if (park->distance <= atof(str))
        return true;
    return false;
}


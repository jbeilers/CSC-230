/**
* @file citylist.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains functions that pertain to the creating, reading, sorting, and printing of a CityList struct
 */

#include "citylist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

CityList *makeCityList()
{
    // Allocate memory for new ParkList
    CityList* citylist = (CityList*) malloc(sizeof(CityList));
    // Initialize fields
    citylist->count = 0;
    citylist->capacity = INITIAL_CITYLIST_CAPACITY;
    citylist->list = calloc(citylist->capacity, sizeof(City *));
    return citylist;
}

void freeCityList (CityList *cityList)
{
    // Free every city
    for (int i = 0; i < cityList->count; i++)
        free((cityList->list)[i]);
    // Free the list of pointers to cities
    free(cityList->list);
    // Free the city list structure
    free(cityList);
}

void readCities (char const *filename, CityList *cityList)
{
    // File to read
    FILE* fp = fopen(filename, "r");

    while (!feof(fp)) {
        // Store city name and location
        char name[CITY_NAME_LENGTH + 1];
        double lat, lon;

        // Scan for data
        char *line = readLine(fp);
        const int check = sscanf(line, "%s %lf %lf\n", name, &lat, &lon);
        free(line);

        // Check validity of data
        if (check != 3
            || (strnlen(name, CITY_NAME_LENGTH + 1) == CITY_NAME_LENGTH + 1
            && name[CITY_NAME_LENGTH] != '\0')) {
            fprintf(stderr, "Invalid city file: %s\n", filename);
            exit(EXIT_FAILURE);
        }

        // Check to see if name already exists in citylist
        for (int i = 0; i < cityList->count; i++) {
            const City *c = (cityList->list)[i];
            if (strcmp(c->name, name) == 0) {
                fprintf(stderr, "Invalid city file: %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }

        // check if parkList list needs to be expanded
        if (cityList->count == cityList->capacity
            && cityList->count >= INITIAL_CITYLIST_CAPACITY) {
            // Double the size of park list
            cityList->list = realloc(cityList->list, sizeof(City *) * (cityList->capacity*=2));
        }

        // Allocate memory for city
        (cityList->list)[cityList->count] = malloc(sizeof(City));
        City* city = (cityList->list)[cityList->count];

        // Store scanned data into city
        city->lat = lat;
        city->lon = lon;
        city->distance = 0;
        city->withinDistance = false;
        strncpy(city->name, name, CITY_NAME_LENGTH + 1);

        cityList->count++; // Increment city list count
    }

    // Close file pointer
    fclose(fp);
}

void sortCities(CityList *cityList, int (*compare) (void const *va, void const *vb))
{
    // Sort list of cities
    qsort(cityList->list, cityList->count, sizeof(City *), compare);
}

void listCities(CityList *cityList)
{
    // Check to see if first city has distance other than 0
    if ((cityList->list)[0]->distance == 0) {
        fprintf(stdout, "%-20s %8s %8s\n", "Name", "Lat", "Lon");
        fflush(stdout);
        for (int i = 0; i < cityList->count; i++) {
            City *city = *(cityList->list + i);
            fprintf(stdout, "%-20s %8.3f %8.3f\n", city->name, city->lat, city->lon);
        }
    } else { // Print cities and their distance from target park
        fprintf(stdout, "%-20s %6s\n", "Name", "Miles");
        fflush(stdout);
        for (int i = 0; i < cityList->count; i++) {
            City *city = *(cityList->list + i);
            if (city->withinDistance)
                fprintf(stdout, "%-20s %6.2f\n", city->name, city->distance);
        }
    }
}

void checkCitiesWithinDistance(City *city, double const distance)
{
    // If the city is within distance, mark withinDistance Field as true
    if (city->distance <= distance)
        city->withinDistance = true;
}

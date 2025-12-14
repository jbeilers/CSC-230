/**
 * @file travel.c
 * @author Jordan B. Eilers (jbeilers)
 * This file contains the main function and helper functions to create lists of cities and parks
 * and return a specified output from the command line arguments provided
 */

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parklist.h"
#include "citylist.h"

/** Multiplier for converting degrees to radians */
#define DEG_TO_RAD ( M_PI / 180 )

/** Radius of the earth in miles. */
#define EARTH_RADIUS 3959.0

/** How many files are expected to be opened */
#define FILES_TO_OPEN 2

/** Minimum amount of command line arguments needed */
#define MIN_ARG_COUNT 4

/** Pointer to target city used in "search parks <city name> <miles>" command */
City *targetCity = NULL;

/** Pointer to target park used in "search cities <park id> <miles>" command */ 
Park *targetPark = NULL;

/** Max distance from target Park/City */
double maxDistance;

/**
 * Computes the distance in miles between two points
 * given their latitudes and longitudes
 * @param lat1 latitude of first point
 * @param lon1 longitude of first point
 * @param lat2 latitude of second point
 * @param lon2 longitude of second point
 * @return distance in miles between the two points
 */
static double distance(double lat1, double lon1, double lat2, double lon2)
{
  // Pretending the center of the earth is at the origin, turn these
  // two locations into unit vectors pointing from the origin.
  double clon1 = cos(lon1 * DEG_TO_RAD);
  double slon1 = sin(lon1 * DEG_TO_RAD);
  double clat1 = cos(lat1 * DEG_TO_RAD);
  double slat1 = sin(lat1 * DEG_TO_RAD);
  double v1[] = { clon1 * clat1, slon1 * clat1, slat1 };

  double clon2 = cos(lon2 * DEG_TO_RAD);
  double slon2 = sin(lon2 * DEG_TO_RAD);
  double clat2 = cos(lat2 * DEG_TO_RAD);
  double slat2 = sin(lat2 * DEG_TO_RAD);
  double v2[] = { clon2 * clat2, slon2 * clat2, slat2 };

  // Dot product these two vectors.
  double dp = v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];

  // This should only happen when we get two copies of the same place.
  if ( dp > 1 )
    return 0;
  
  // Compute the angle between the vectors based on the dot product.
  double angle = acos( dp );

  // Return distance based on the radius of the earth.
  return EARTH_RADIUS * angle;
}

/**
 * Checks the count of program arguments and the validity of the files that need to be opened
 * @param argc program argument count
 * @param argv program arguments
 */
void checkInputValidity(const int argc, char *argv[])
{
    if (argc < MIN_ARG_COUNT) {
        fprintf(stderr, "usage: travel park-file city-file command parameter*\n");
        exit(EXIT_FAILURE);
    }
    // Open files to check if NULL, then close files
    for (int i = 0; i < FILES_TO_OPEN; i++) {
        FILE* file = fopen(argv[i+1], "r");
        if (file == NULL) {
            fprintf(stderr, "Can't open file: %s\n", argv[i+1]);
            exit(EXIT_FAILURE);
        }
        fclose(file);
    }
}

/**
 * Throws an error if a command line argument is invalid
 */
void invalidCommand()
{
    fprintf(stdout, "Invalid command\n");
    exit(EXIT_FAILURE);
}

/**
 * Looks through parkList's list of parks, and sets the park with the given ID
 * as the target park
 * @param parkList ParkList to be looked at
 * @param id ID of park that's to be set as target park
 */
void setTargetPark(const ParkList *parkList, const char id[])
{
    // Look through parkList list, if it finds a matching park ID set it as the target park
    for (int i = 0; i < parkList->count; i++) {
        Park *park = parkList->list[i];
        if (park->id == atof(id)) {
            targetPark = park;
            return;
        }
    }
    invalidCommand();
}

/**
 * Looks through cityList's list of cities, and sets the city with the given name
 * as the target park
 * @param cityList CityLIst to be looked at
 * @param name name of city that's to be set as target city
 */
void setTargetCity(const CityList *cityList, char name[])
{
    // Look through cityList list, if it finds a matching city ID set it as the target city
    for (int i = 0; i < cityList->count; i++) {
        City *city = cityList->list[i];
        if (strcmp(city->name, name) == 0) {
            targetCity = city;
            return;
        }
    }
    invalidCommand();
}

/**
 * Sets the max distance for distance checking when printing
 * @param distance Target distance
 */
void setMaxDistance(const double distance)
{
    maxDistance = distance;
}

/**
 * Provides comparison logic for sorting by park ID
 * @param va Pointer to a pointer to a park
 * @param vb Pointer to a pointer to a park
 * @return -1: va before vb | 1: va after vb | 0: same ID
 */
int sortParkID(void const *va, void const *vb)
{
    Park **p1 = (Park **) va, **p2 = (Park **) vb;
    if ((*p1)->id < (*p2)->id)
        return -1;
    else if ((*p1)->id == (*p2)->id)
        return 0;
    return 1;
}

/**
 * Provides comparison logic for sorting by park name
 * @param va Pointer to a pointer to a park
 * @param vb Pointer to a pointer to a park
 * @return -1: va before vb | 1: va after vb | 0: same name
 */
int sortParkName(void const *va, void const *vb)
{
    Park **p1 = (Park **) va, **p2 = (Park **) vb;
    if (strcmp((*p1)->name, (*p2)->name) < 0)
        return -1;
    else if (strcmp((*p1)->name, (*p2)->name) == 0)
        return 0;
    return 1;
}

/**
 * Provides comparison logic for sorting by park distance from target city
 * @param va Pointer to a pointer to a park
 * @param vb Pointer to a pointer to a park
 * @return -1: va before vb | 1: va after vb | 0: same distance
 */
int sortParkDistance(void const *va, void const *vb)
{
    Park **p1 = (Park **) va, **p2 = (Park **) vb;
    (*p1)->distance = distance((*p1)->lat, (*p1)->lon, targetCity->lat, targetCity->lon);
    (*p2)->distance = distance((*p2)->lat, (*p2)->lon, targetCity->lat, targetCity->lon);
    if ((*p1)->distance < (*p2)->distance)
        return -1;
    else if ((*p1)->distance == (*p2)->distance)
        return 0;
    return 1;
}

/**
 * Provides comparison logic for sorting by city name
 * @param va Pointer to a pointer to a city
 * @param vb Pointer to a pointer to a city
 * @return -1: va before vb | 1: va after vb | 0: same name
 */
int sortCityName(void const *va, void const *vb)
{
    City **c1 = (City **) va, **c2 = (City **) vb;
    if (strcmp((*c1)->name, (*c2)->name) < 0)
        return -1;
    else if (strcmp((*c1)->name, (*c2)->name) == 0)
        return 0;
    return 1;
}

/**
 * Provides comparison logic for sorting by city distance from target park
 * @param va Pointer to a pointer to a city
 * @param vb Pointer to a pointer to a city
 * @return -1: va before vb | 1: va after vb | 0: same distance
 */
int sortCityDistance(void const *va, void const *vb)
{
    City **c1 = (City **) va, **c2 = (City **) vb;
    (*c1)->distance = distance((*c1)->lat, (*c1)->lon, targetPark->lat, targetPark->lon);
    (*c2)->distance = distance((*c2)->lat, (*c2)->lon, targetPark->lat, targetPark->lon);
    checkCitiesWithinDistance(*c1, maxDistance);
    if ((*c1)->distance < (*c2)->distance)
        return -1;
    else if ((*c1)->distance == (*c2)->distance)
        return 0;
    return 1;
}

/**
 * Depending on argc and argv values,
 * contains different branches of code for the different possibilities
 * @param parklist ParkList provided
 * @param citylist CityList provided
 * @param argc Command line argument count
 * @param argv Command line arguments
 */
void actionToDo(ParkList *parklist, CityList *citylist, const int argc, char *argv[])
{
    // Run through possibilities
    switch (argc) {
        case 4: // Only list parks
            if (strcmp(argv[3], "list") == 0) {
                sortParks(parklist, sortParkID); // Sort parks by ID
                listParks(parklist, listAllParks, NULL); // List all parks
                break;
            }
            invalidCommand();
        case 5: // List cities or parks, sorted by name
            if (strcmp(argv[3], "list") == 0) {
                if (strcmp(argv[4], "names") == 0) {
                    sortParks(parklist, sortParkName); // Sort parks by name
                    listParks(parklist, listAllParks, NULL); // List all parks
                    break;
                }
                else if (strcmp(argv[4], "cities") == 0) {
                    sortCities(citylist, sortCityName); // Sort city by name
                    listCities(citylist);
                    break;
                }
                invalidCommand();
            }
            invalidCommand();
        case 6: // Search for park by name
            if (strcmp(argv[3], "search") == 0) {
                if (strcmp(argv[4], "parks") == 0) {
                    sortParks(parklist, sortParkID); // Sort parks by ID
                    listParks(parklist, searchParksByString, argv[5]); // List parks with argument in their name
                    break;
                }
                invalidCommand();
            }
            invalidCommand();
        case 7: // Search parks or cities within distance
            if (strcmp(argv[3], "search") == 0) { // Search for parks or cities
                if (strcmp(argv[4], "parks") == 0) { // Search for parks in proximity with city
                    setTargetCity(citylist, argv[5]); // Set target city
                    // Check validity of 7th argument
                    if (atof(argv[6]) == 0.0)
                        invalidCommand();
                    setMaxDistance(atof(argv[6]));
                    sortParks(parklist, sortParkDistance); // Sort parks by distance
                    listParks(parklist, listParksWithinDistance, argv[6]); // List parks only within distance
                    break;
                } else if (strcmp(argv[4], "cities") == 0) { // Search for cities in proximity with park
                    setTargetPark(parklist, argv[5]); // Set target park
                    // Check validity of 7th argument
                    if (atof(argv[6]) == 0.0)
                        invalidCommand();
                    setMaxDistance(atof(argv[6]));
                    sortCities(citylist, sortCityDistance); // Sort cities by distance
                    listCities(citylist);
                    break;
                }
                invalidCommand();
            }
            invalidCommand();
        default: // invalid number of arguments
            fprintf(stderr, "usage: travel park-file city-file command parameter*\n");
            exit(1);
    }
}

/**
 * Main function that checks the validity of files,
 * creates a new ParkList & CityList,
 * Reads data into those lists,
 * Performs actions with the actionToDo function,
 * and frees memory occupied by the ParkList & CityList
 * @param argc command line argument count
 * @param argv command line arguments
 * @return exit code
 */
int main(const int argc, char *argv[])
{
    // Check validity of files and argc
    checkInputValidity(argc, argv);

    // Creat a new ParkList & CityList
    ParkList *parks = makeParkList();
    CityList *cities = makeCityList();

    // Read data from files into these lists
    readParks(argv[1], parks);
    readCities(argv[2], cities);

    // Perform action based on argc & argv
    actionToDo(parks, cities, argc, argv);

    // Free memory n'' stuff & close files
    freeParkList(parks);
    freeCityList(cities);

    return EXIT_SUCCESS;
}


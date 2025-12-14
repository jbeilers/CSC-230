/**
* @file citylist.h
 * @author Jordan B. Eilers (jbeilers)
 * This file contains functions that pertain to the creating, reading, sorting, and printing of a CityList struct
 */

#ifndef HW3_CITYLIST_H
#define HW3_CITYLIST_H
#include <stdbool.h>

#endif //HW3_CITYLIST_H

#define CITY_NAME_LENGTH 20
#define INITIAL_CITYLIST_CAPACITY 5

/**
 * Struct for a City with fields:
 * name: City name
 * lat: city latitude
 * lon: city longitude
 * distance: distance from target park
 * withinDistance: True if within distance of target park
 */
typedef struct City {
    char name[CITY_NAME_LENGTH + 1];
    double lat;
    double lon;
    double distance;
    bool withinDistance;
} City;

/**
 * Struct for a CityList with fields:
 * list: List of pointers to Cities
 * count: Number of city pointers in list
 * capacity: Maximum number of pointers to cities allowed in list
 */
typedef struct CityList {
    City **list;
    int count;
    int capacity;
} CityList;

/**
 * Creates and initializes fields of a CityList struct
 * @return CityList struct
 */
CityList *makeCityList();

/**
 * Frees the fields of cityList and itself
 * @param cityList CityList to be freed
 */
void freeCityList (CityList *cityList);

/**
 * Reads city data from the given file and stores each city in the list of cities of cityList
 * @param filename File to be parsed
 * @param cityList CityList to store cities in
 */
void readCities (char const *filename, CityList *cityList);

/**
 * Sorts the cities in the list field of cityList based on the logic of the given compare function parameter
 * @param cityList CityList to be sorted
 * @param compare Compare fucntion pointer to be used as compare logic
 */
void sortCities(CityList *cityList, int (*compare) (void const *va, void const *vb));

/**
 * Prints each city in the given cityList's list field
 * @param cityList cityList to be printed
 */
void listCities(CityList *cityList);

/**
 * Changes a city's withinDistance field to true if the city is below the given distance
 * @param city City to be checked
 * @param distance Distance the city must be within from target park
 */
void checkCitiesWithinDistance(City *city, double distance);
/** Introduce an incomplete type for letting client code talk about a list. */
typedef struct ListStruct List;

/** Make a new, empty list. 
    @return Pointer to the new list.
 */
List *makeList();

/** Free the given list and all of its nodes.
    @param list pointer to the list to free.
*/
void freeList( List *list );

/** Insert the given value at the front of the list.
    @param list pointer to the list to modify.
    @param val value to insert in the list.
 */
void insertFront( List *list, int val );

/** Insert the given value at the back of the list.
    @param list pointer to the list to modify.
    @param val value to insert in the list.
*/
void insertBack( List *list, int val );

/** Insert the given value in the right position to maintain a sorted list. 
    @param list pointer to the list to modify.
    @param val value to insert in the list.
 */
void insertSorted( List *list, int val );

/** Pirnt out a report of what's on the given list.
    @param list pointer to the list to print.
 */
void printList( List *list );

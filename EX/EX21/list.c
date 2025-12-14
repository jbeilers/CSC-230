// Code to implement a linked list, with the representation hidden from client code.

#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A short type name to use for a node.
typedef struct NodeStruct Node;

// Node for building our linked list.
struct NodeStruct {
  // Value in this node.
  int value;

  // Pointer to the next node.
  Node *next;
};

// Structure representing a whole linked list, with a head pointer and
// a record of how many items are ont he list.  This is kind analogous to
// an object we might build in Java, but in C, all the functions that operate
// on this object are just plain-old functions.  We have to pass them an instance
// of the object they are supposed to work on.
struct ListStruct {
  // Pointer to the first node on a list.
  Node *head;

  // Number of items on the list, just to have something else.
  // to keep on our lists.
  int length;
};

List *makeList()
{
  // Allocate space for a ListStruct.
  List *list = (List *) malloc( sizeof( List ) );

  // Initialize its fields for an empty list.
  list->head = NULL;
  list->length = 0;

  return list;
}

void freeList( List *list )
{
  // Free all the nodes on the list.
  while ( list->head ) {
    Node *n = list->head;
    list->head = n->next;
    free( n );
  }

  // Then, free the ListStruct itself.
  free( list );
}

/** The easiest function.  Use a pointer to the head pointer to insert
    a new node at the start of the lsit. */
void insertFront( List *list, int val )
{
  // Get a pointer to the head pointer (we don't really need to do this in this
  // case, but it'll be a good warmup for the later exercises, which are
  // more interesting.)
  
  // This is a pointer to the pointer we're going to change, to insert a value in the list.
  Node **target = &( list->head );
  
  // Make a new, n, node and store val in it.
  Node *n = malloc(sizeof(Node));
    n->value = val;

  // Link this node in at the head of the list.  This requires two
  // things, both of which we can do via target, without directly
  // accessing the head pointer.

  // First, copy the old value of the head pointer (the pointer target
  // points to) to the next pointer in node n.
  n->next = *target;


  // Now, set the head pointer (the pointer target points to) to n, so
  // our new node is at the front of the list.
    *target = n;

  // We just added a node to this list, so its one node longer now.
  list->length++;
}

/** The second easiest function.  Use a pointer to a Node pointer to
    find the last pointer at the end of the list.  Then, assign it to
    point to a new node instead.  Here, we run though the list every
    time looking for the null pointer at the end.  This could be more
    efficient with a tail pointer. */
void insertBack( List *list, int val )
{
  // Get a pointer to the head pointer.  If the iist is empty, that's
  // the pointer we need to change anyway.
  Node **target = &( list->head );
  
  // Move target ahead until it points to a null pointer.  When it
  // does, we know we've reached the null pointer at the end of the
  // list.
    while (*target != NULL)
        target = &((*target)->next);

  // Make a new, n, node and store val in it.
  Node *n = malloc(sizeof(Node));
    n->value = val;

  // We can add the new node to the end of the list just by using
  // target, the pointer to the link we need to change.
  
  // First, n is our new last node, so its next pointer needs to be
  // null (don't really need target for this but you can use it if you
  // want to, since it should be pointing to the null pointer at the
  // end of the list).
  n->next = NULL;

  // Now, modify the pointer target is pointing to so it points to n
  // instead.  This will add n to the end of the list.
  *target = n;

  // We just added a node to this list, so its one node longer now.
  list->length++;
}

/** The most interesting function.  Use a pointer to a Node pointer to
    find the right place to insert a new node in order keep the list
    sorted.  It's a lot like insertAtTail(), but the condition for
    finding the right link to change is a little more complicated */
void insertSorted( List *list, int val )
{
  // Get a pointer to the head pointer.  If the iist is empty, or if
  // val is smaller than anything on the list, that's the pointer we need to
  // change.
  Node **target = &( list->head );
  
  // Move target ahead until it points to a null pointer or to a poitner
  // to a node with a value larger than val.  Either way, that's when
  // target points to the pointer we need to change.
  while (*target != NULL && (*target)->value < val)
      target = &((*target)->next);

  // Make a new, n, node and store val in it.
  Node *n = malloc(sizeof(Node));
    n->value = val;

  // We can add the new node to the right place in the list via the
  // target pointer, since it points to the pointer we need to change to
  // insert this new node.
  
  // First, make the next pointer out of n point to the node that
  // *target is pointing to.  This will link in that node so it occurs
  // after n in the list.
  n->next = *target;

  // Now, set the pointer target is pointing to to n.  This will finish
  // inserting the new node n right before the first node that contains a
  // value larger than val.
  *target = n;

  // We just added a node to this list, so its one node longer now.
  list->length++;
}

/** Print out the size and contents of the given list. */
void printList( List *list )
{
  // Print the size first, really just so we have a reason for maintaining it.
  printf( "%d elements :", list->length );

  // Then, print the elements on the rest of the line.
  for ( Node *n = list->head; n; n = n->next )
    printf( " %d", n->value );
  printf( "\n" );
  
}

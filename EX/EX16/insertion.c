// Code to read numbers from standard input inserting them into a list
// in sorted order as we read them.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Node for building our linked list.
struct NodeStruct {
  // Value in this node.
  int value;

  // Pointer to the next node.
  struct NodeStruct *next;
};

// A short type name to use for a node.
typedef struct NodeStruct Node;

/*
  Create a node for the given value and insert it into the given list
  in the proper location to keep the list sorted.  Return an updated
  head pointer.  The head should only need to change if this value gets
  inserted at the front of the list.  Otherwise, you can just return
  the old head pointer.
*/
Node *insert( Node *head, int val )
{
    // If head is NULL, add it
    if (head == NULL) {
        head = malloc(sizeof(Node));
        head->value = val;
        head->next = NULL;
        return head;
    }
    // Insert at front
    if (val < head->value) {
        Node *current = malloc(sizeof(Node));
        current->value = val;
        current->next = head;
        return current;
    }
    Node *current = head->next;
    Node *previous = head;
    //
    while (current != NULL) {
        // If value is less than current node value
        if (val < current->value) {
            previous->next = malloc(sizeof(Node));
            previous->next->value = val;
            previous->next->next = current;
            return head;
        }
        // Go to next node
        current = current->next;
        previous = previous->next;
    }
    // If at end of list
    current = malloc(sizeof(Node));
    current->value = val;
    current->next = NULL;
    previous->next = current;
    return head;
}


int main()
{
  // Pointer to the head node.
  Node *head = NULL;

  // Read a list of numbers from building a sorted list as we
  // read them.
  int x;
  while ( scanf( "%d", &x ) == 1 ){
    // Insert the new value, and get back the (possibly updated) head pointer.
    head = insert( head, x );
  }

  // Print out the list.
    Node *current = head;
  while (current != NULL) {
      printf("%d ", current->value);
      current = current->next;
  }
    printf("\n");
  
  // Free all the nodes.
  current = head;
  while (current != NULL) {
      Node *next = current->next;
      free(current);
      current = next;
  }
  
  return 0;
}

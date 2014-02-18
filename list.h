/* 
 * File:   list.h
 * Author: jts
 *
 * Created on 12 January 2014, 12:18 AM
 */

#ifndef LIST_H
#define	LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef struct list_item {
  struct list_item* next;
  struct list_item* prev;
  void* data;
} list_item;

typedef struct {
  list_item* head;
  list_item* tail;
  int size;
} list;

extern list* list_init();
// add a new list_item containing data to the start of the list
extern void list_add(list *this, void* data);
// add a new list_item containing data to the end of the list
extern void list_add_end(list *this, void* data);
// remove a list_item from the front of the list, return that item's data
extern void* list_remove(list* this);
// remove a list_item from the end of the list, return that item's data
extern void* list_remove_end(list* this);
// returns the first item in the list
extern list_item* list_get_first_item(list* this);
extern void* list_get_first(list* this);
// returns the last item in the list
extern list_item* list_get_last_item(list* this);
extern void* list_get_last(list* this);
//  returns the item at index in the list
extern list_item* list_get_item(list* this, int index);
extern void* list_get(list* this, int index);
// sorts the list into ascending order of data
extern int list_quicksort(list* this);
// runs a function on every item in the list
extern void list_foreach(list* this, void (*function)());

// free all memory related to the list, but leave list_item data intact
extern void list_free(list* this);
// free all memory related to the list, including list_item data (it's your fault if it segfaults)
extern void list_free_all(list* this);
// print the data values of all nodes in the list, from head to tail
extern void printlist(list *this);

#endif	/* LIST_H */


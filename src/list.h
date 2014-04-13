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

typedef union {
    int     value;
    double  dvalue;
    void    *data;
} list_data_t;

typedef struct list_item_t {
  struct list_item_t* next;
  struct list_item_t* prev;
  list_data_t data;
} list_item_t;

typedef struct {
  list_item_t* head;
  list_item_t* tail;
  int size;
} list_t;

extern list_t* list_init();
// add a new list_item containing data to the start of the list
extern void list_add(list_t *this, list_data_t data);
// add a new list_item containing data to the end of the list
extern void list_add_end(list_t *this, list_data_t data);
// remove a list_item from the front of the list, return that item's data
extern list_data_t list_remove(list_t* this);
// remove a list_item from the end of the list, return that item's data
extern list_data_t list_remove_end(list_t* this);
// returns the first item in the list
extern list_item_t* list_get_first_item(list_t* this);
extern list_data_t list_get_first(list_t* this);
// returns the last item in the list
extern list_item_t* list_get_last_item(list_t* this);
extern list_data_t list_get_last(list_t* this);
//  returns the item at index in the list
extern list_item_t* list_get_item(list_t* this, int index);
extern list_data_t list_get(list_t* this, int index);
// sorts the list into ascending order of data
extern void list_quicksort(list_t* this);
extern void list_quicksort_f(list_t* this, int(*sort)());
// runs a function on every item in the list
extern void list_foreach(list_t* this, void(*function)());

// free all memory related to the list, but leave list_item data intact
extern void list_free(list_t* this);
// free all memory related to the list, including list_item data (it's your fault if it segfaults)
extern void list_free_all(list_t* this);
// print the data values of all nodes in the list, from head to tail
extern void printlist(list_t *this);

#endif	/* LIST_H */



#include "list.h"

list_t* list_init() {
    list_t* ret = malloc(sizeof (list_t));
    ret->head = NULL;
    ret->tail = NULL;
    ret->size = 0;
    return ret;
}

list_item_t* new_list_item(list_data_t data) {
    list_item_t* ret = malloc(sizeof(list_item_t)); //allocate space for next pointer and data pointer
    ret->next = NULL;
    ret->prev = NULL;
    ret->data = data;
    return ret;
}

/*
 *  add a new list_item containing data to the start of the list
 */
void list_add(list_t *this, list_data_t data) {
    if(this == NULL) {
        return;
    }
    list_item_t* new = new_list_item(data);
    this->size++;
    if (this->head != NULL) {
        this->head->prev = new;
    } else {
        this->tail = new;
    }
    new->next = this->head;
    this->head = new;
}

/*
 *  add a new list_item containing data to the end of the list
 */
void list_add_end(list_t *this, list_data_t data) {
    if(this == NULL) {
        return;
    }
    list_item_t* new = new_list_item(data);
    this->size++;
    if (this->head != NULL) {
        this->tail->next = new;
    } else {
        this->head = new;
    }
    new->prev = this->tail;
    this->tail = new;
}

/*
 *  prepend a list_item to the front of the list
 */
void list_prepend(list_t* this, list_item_t* n) {
    if(this == NULL) {
        return;
    }
    list_item_t* oldhead = this->head;
    this->head = n;
    list_item_t* tmp = n;
    for (this->size++; tmp->next != NULL; tmp = tmp->next) this->size++;
    if (oldhead == NULL) {
        this->tail = tmp;
    } else {
        oldhead->prev = tmp;
    }
    tmp->next = oldhead;
}

/*
 *  append a list_item to the end of the list
 */
void list_append(list_t* this, list_item_t* n) {
    if(this == NULL) {
        return;
    }
    if (this->head == NULL) {
        this->head = n;
        this->tail = n;
    } else {
        this->tail->next = n;
    }
    list_item_t* tmp = this->tail;
    for (this->size++; tmp->next != NULL; tmp = tmp->next) this->size++;
    this->tail = tmp;
}

/*
 *  removes a list_item from the front of the list, but DON'T free() it, and return a pointer to that item
 */
list_item_t* list_unlink(list_t* this) {
    if (this == NULL || this->head == NULL) {
        return NULL;
    }
    this->size--;
    if (this->size == 0) {
        this->tail = NULL;
    }
    list_item_t* ret = this->head;
    this->head = this->head->next;
    if (this->head != NULL) this->head->prev = NULL;
    ret->next = NULL;
    return ret;
}

/*
 *  remove a list_item from the end of the list, but DON'T free() it, and return a pointer to that item
 */
list_item_t* list_unlink_end(list_t* this) {
    if (this == NULL || this->head == NULL) {
        return NULL;
    }
    this->size--;
    if (this->size == 0) {
        this->head = NULL;
    }
    list_item_t* ret = this->tail;
    this->tail = this->tail->prev;
    if (this->tail != NULL) {
        this->tail->next = NULL;
    }
    ret->prev = NULL;
    return ret;
}

/*
 *  remove a list_item from the front of the list, return that item's data
 */
list_data_t list_remove(list_t* this) {
    if (this == NULL || this->head == NULL) {
        return (list_data_t)NULL;
    }
    this->size--;
    if (this->size == 0) {
        this->tail = NULL;
    }
    list_item_t* tmp = this->head;
    list_data_t ret = tmp->data;
    this->head = this->head->next;
    free(tmp);
    return ret;
}

/*
 *  remove a list_item from the end of the list, return that item's data
 */
list_data_t list_remove_end(list_t* this) {
    if (this == NULL || this->head == NULL) {
        return (list_data_t)NULL;
    }
    this->size--;
    if (this->size == 0) {
        this->head = NULL;
    }
    list_item_t* tmp = this->tail;
    this->tail = this->tail->prev;
    list_data_t ret = tmp->data;
    free(tmp);
    return ret;
}

list_item_t* list_get_first_item(list_t* this) {
    if(this == NULL) {
        return NULL;
    }
    return this->head;
}

/*
 *  returns the data of the first item in the list
 */
list_data_t list_get_first(list_t* this) {
    if(this == NULL) {
        return (list_data_t)NULL;
    }
    return this->head == NULL ? (list_data_t)NULL : this->head->data;
}

list_item_t* list_get_last_item(list_t* this) {
    if(this == NULL) {
        return NULL;
    }
    return this->tail;
}

/*
 *  returns the data of the last item in the list
 */
list_data_t list_get_last(list_t* this) {
    if(this == NULL) {
        return (list_data_t)NULL;
    }
    return this->tail == NULL ? (list_data_t)NULL : this->tail->data;
}

list_item_t* list_get_item(list_t* this, int index) {
    if(this == NULL) {
        return NULL;
    }
    // check if we're past half-way in the list
    if(index > this->size / 2) {
        // start from tail
        int x = this->size;
        list_item_t* tmp = this->tail;
        while(tmp != NULL && x > index) {
            tmp = tmp->prev;
            x--;
        }
        return tmp;
    } else {
        // start from head
        int x = 0;
        list_item_t* tmp = this->head;
        while(tmp != NULL && x < index) {
            tmp = tmp->next;
            x++;
        }
        return tmp;
    }
    return NULL;
}

/*
 *  returns the data at index in list
 */
list_data_t list_get(list_t* this, int index) {
    list_item_t* tmp = list_get_item(this, index);
    if(tmp == NULL) {
        return (list_data_t)NULL;
    }
    return tmp->data;
}

/**
 *  sorts the list into ascending order of data
 */
void list_quicksort(list_t* this) {
    if(this == NULL) {
        return;// -1;
    }
    //int datacomparisons = 0;
    list_item_t* pivot = list_unlink(this); //pivot=head of the list, and remove the pivot from the list
    list_t* smaller = list_init();
    list_t* bigger = list_init();
    while (this->head != NULL) {
        list_item_t* tmp = list_unlink(this); //node to sort=head of the list, and remove the node to sort from our list
        //datacomparisons++;
        if (tmp->data.value >= pivot->data.value) {
            list_append(bigger, tmp);
        } else {
            list_append(smaller, tmp);
        }
    }
    if (smaller->size > 1) {
        //if there's more than 1 node in smaller, sort it
        /*datacomparisons += */list_quicksort(smaller);
    }
    if (bigger->size > 1) {
        //if there's more than 1 node in bigger, sort it
        /*datacomparisons += */list_quicksort(bigger);
    }

    //order like so: [smaller][pivot][bigger]
    list_append(smaller, pivot);
    list_append(smaller, bigger->head);
    list_append(this, smaller->head);
    free(bigger); //only want to delete the list, not it's items
    free(smaller); //only want to delete the list, not it's items
    //return datacomparisons;
}

/*
 * sorts the list into ascending order of data,
 * based on a function which determines which item is larger
 */
void list_quicksort_f(list_t* this, int(*sort)()) {
    if(this == NULL) {
        return;// -1;
    }
    int datacomparisons = 0;
    list_item_t* pivot = list_unlink(this); //pivot=head of the list, and remove the pivot from the list
    list_t* smaller = list_init();
    list_t* bigger = list_init();
    while (this->head != NULL) {
        list_item_t* tmp = list_unlink(this); //node to sort=head of the list, and remove the node to sort from our list
        //datacomparisons++;
        if (sort(tmp->data, pivot->data) >= 0) {
            list_append(bigger, tmp);
        } else {
            list_append(smaller, tmp);
        }
    }
    if (smaller->size > 1) {
        //if there's more than 1 node in smaller, sort it
        /*datacomparisons += */list_quicksort_f(smaller, sort);
    }
    if (bigger->size > 1) {
        //if there's more than 1 node in bigger, sort it
        /*datacomparisons += */list_quicksort_f(bigger, sort);
    }

    //order like so: [smaller][pivot][bigger]
    list_append(smaller, pivot);
    list_append(smaller, bigger->head);
    list_append(this, smaller->head);
    free(bigger); //only want to delete the list, not it's items
    free(smaller); //only want to delete the list, not it's items
    //return datacomparisons;
}

/*
 *  calls function on each item in the list
 */
void list_foreach(list_t* this, void(*function)()) {
    if(this == NULL) {
        return;
    }
    list_item_t* tmp = this->head;
    while(tmp != NULL) {
        function(tmp->data);
        tmp = tmp->next;
    }
}

/*
 *  free all memory related to the list, but leave data intact
 */
void list_free(list_t* this) {
    if(this == NULL) {
        return;
    }
    while (this->head != NULL) {
        list_item_t* tmp = this->head;
        this->head = this->head->next;
        free(tmp);
    }
    free(this);
}

/*
 *  free all memory related to the list, including data
 */
void list_free_all(list_t* this) {//(be careful - it'll segfault if you free() a string constant or a non-pointer)
    if(this == NULL) {
        return;
    }
    while (this->head != NULL) {
        list_item_t* tmp = this->head;
        this->head = this->head->next;
        free(tmp->data.data);
        free(tmp);
    }
    free(this);
}

/*
 *  print the data values (either pointers to data, or ints) of all nodes in the list, from head to tail
 */
void printlist(list_t *this) {
    if(this == NULL) {
        return;
    }
    list_item_t* tmp = this->head;
    for (; tmp != NULL;) {
        printf("%d\n", tmp->data);
        tmp = tmp->next;
    }
}

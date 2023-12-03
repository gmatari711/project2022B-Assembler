#ifndef HASHANDLIST_LIST_H
#define HASHANDLIST_LIST_H
#include "misc.h"

#ifndef LIST_TYPE_DEFINITION
#define LIST_TYPE_DEFINITION
typedef         struct list    *List;
#endif

typedef         void (*destroy)(void **);
typedef         int(*objectCMP)(const void * ,const void *);
typedef         int(*object_key_cmp)(const void * ,const void *);

/**
 * Allocates and initialize a List container object.
 * @return An initialized List container object.
 */
List listCreate();

/**
 * Free all allocated memory for a List container object.
 * @param a_list A pointer to a List container object.
 * @param a_delete A destructor function for items containd in *a_list.
 */
void listDestroy(List *a_list, destroy a_delete);

/**
 * Insert an item to the List container object.
 * @param a_list A List container object.
 * @param a_item An item to be inserted.
 */
void listInsert(List a_list , void *a_item);

/**
 * Remove the first item on List container object matching the key a_key.
 * @param a_list A List container object.
 * @param a_key A pointer to a key element.
 * @param f_cmp A comparison function between an item and a_key. the function return 0.
 *              if matched.
 * @return Returns the item that was removed.
 */
void *listRemove(List a_list , void *a_key, objectCMP f_cmp);

/**
 * Search for an item matched with a_key.
 * @param a_list A List container object.
 * @param a_key A pointer to a key element.
 * @param f_cmp A comparison function between an item and a_key. the function return 0.
 * @return Returns The first item matching the search.
 */
void *listSearch(List a_list,const void *a_key, objectCMP);

/**
 * Check if List container object is empty.
 * @param a_list a_list A List container object.
 * @return True(1) if empty. False(0) otherwise.
 */
Boolean listIsEmpty(List a_list);


#endif

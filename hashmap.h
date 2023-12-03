#ifndef HASHANDLIST_HASHMAP_H
#define HASHANDLIST_HASHMAP_H

#include "list.h"
#include "misc.h"
#include <stdlib.h>
#include <stdio.h>

typedef             size_t (*hashfunc)(const void *);
typedef             struct hash_table           *HashMap;

/**
 * 
 * @param a_size
 * @param f_hash
 * @return 
 */
HashMap hashMapCreate(size_t a_size, hashfunc f_hash);

/**
 * Deallocate all memory allocated for HashMap container object. object set to NULL.
 * @param a_map A pointer to a HashMap object.
 * @param destroy a destructor function for items kept in a_map.
 */
void hashMapDestroy(HashMap *a_map,destroy);
/**
 * Insert an item to HashMap container object. If an item with the same key already exists
 * it is replaced by the new item.
 * @param a_map A HashMap container object.
 * @param a_item A pointer containing an address of an item.
 * @param a_key item's key.
 * @param f_cmp a comparison function.
 * @return Returns the replaced item if exists. NULL otherwise.
 */
void *hashMapInsert(HashMap a_map,void *a_item,void *a_key, objectCMP f_cmp);
/**
 * Search for an item by a given key.
 * @param a_map A HashMap container object. 
 * @param a_item A pointer containing an address of an item.
 * @param f_cmp a comparison function.
 * @return An item with a matching key  if exists. NULL otherwise.
 */
void *hashMapSearch(HashMap a_map, const void *a_key, object_key_cmp f_cmp);
/**
 * NOT YET IMPLEMENTED AND NOT RELEVANT FOR THIS PROJECT - SET AS AN API GOAL
 * TO BE IMPLEMENTED FOR A FUTURE PORTFOLIO!!!
 */
void *hashMapRemove(HashMap a_map, const void *a_key, object_key_cmp f_cmp);
/**
 * A version of foreach extended by the use of a predicate function. 
 * The forEach loop apply action function to any item which uphold the predicate.
 * @param a_map A HashMap container object. 
 * @param predicate A function recieving an item and return a Boolean value.
 * @param action A functions recieving an item and a pointer and preform a task the item.
 * @param a_param A parameter for the action function. could be passed with NULL if not needed.
 */
void forEach(HashMap a_map, Boolean (*predicate)(void *), void (*action)(void *, void *), void * param);

/**
 * Utility function that returns True to be send as predicate to apply forEach on all
 * items.
 * param a_item An item. 
 */
Boolean returnTrue(void *);






#endif

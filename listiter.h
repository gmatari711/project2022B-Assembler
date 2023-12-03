#ifndef HASHANDLIST_LISTITER_H
#define HASHANDLIST_LISTITER_H

typedef         void        *ListIterator;
#ifndef LIST_TYPE_DEFINITION
#define LIST_TYPE_DEFINITION
typedef         struct list    *List;
#endif

#include "misc.h"
/**
 * Produce an iterator to the beginning of the List container object.
 * @param a_list A List container object.
 * @return An Iterator to the beginning of a_list.
 */
ListIterator listIterBegin(List a_list);

/**
 * Produce an iterator to the tail (Dummy) of the List container object. THE TAIL DEOS NOT HOLD
 * ANY DATA.
 * @param a_list a_list A List container object.
 * @return An Iterator to the end of a_list.
 */
ListIterator listIterEnd(List a_list);

/**
 * Iterates to the next element in the List container object.
 * @param a_iter A List container object iterator.
 * @return An iterator to the next element.
 */
ListIterator listIterNext(ListIterator a_iter);



/**
 * Retrieve item of pointed element.
 * @param a_iter A List container object iterator.
 * @return Returns current item.
 */
void *listIterGetItem(ListIterator a_iter);

/**
 * Set item value of pointed element.
 * @param a_iter A List container object iterator.
 * @param a_item The new item to be set.
 * @return Returns the existing item.
 */
void *listIterSetItem(ListIterator a_iter, void *a_item);

/**
 * Remove current element refered by iterator
 * @param a_iter A List container object iterator.
 * @return Returns the item of the deleted element.
 * NOTE FOR NOTICE: SHOULD'VE RETURNED ITEM VIA OUTPUT VARIABLE AND RETURN AN ITERATOR
 * FOR THE NEXT ELEMENT. A FUTURE TASK.
 * */
void *listIterRemove(ListIterator a_iter);

/**
 * Test next iteration is possible. If not - current iterator points to tail of list.
 * @param a_iter A List container object iterator.
 * @return True(1) if next iteration is possible. False(0) otherwise.
 */
Boolean listIterHasNext(ListIterator a_iter);

/**
 * Insert a new element before the current.
 * @param a_iter A List container object iterator.
 * @param a_item an item to be held by the new element.
 */
void ListIterInsert(ListIterator a_iter, void *a_item);

#endif
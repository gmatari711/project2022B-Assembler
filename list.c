#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "listiter.h"

typedef struct node{
    void           *m_item;
    struct node    *m_next;
    struct node    *m_prev;
}Node;

struct list{
    Node            m_head;
    Node            m_tail;
};

static struct node *node_create(void *a_item);

struct list *listCreate(){
    struct list         *result = NULL;

    result = calloc(1, sizeof(struct list));
    assert(NULL != result);

    result->m_head.m_next = &result->m_tail;
    result->m_tail.m_prev = &result->m_head;

    return result;
}

static struct node *node_create(void *a_item){
    struct node         *result     = NULL;

    result = calloc(1, sizeof(struct node));
    assert(NULL != result);

    result->m_item = a_item;

    return result;
}

void listDestroy(struct list **a_list, destroy a_delete){
    Node                *current    = NULL;


    if(NULL != *a_list){
        current = (*a_list)->m_head.m_next;

        while (current != &(*a_list)->m_tail){
            if(NULL != a_delete){
                a_delete(current->m_item);
            }
            current = current->m_next;
            free(current->m_prev);
        }
        free(*a_list);
        *a_list = NULL;
    }
}


void listInsert(struct list *a_list , void *a_item){
    Node                *new_node   = NULL;

    new_node = node_create(a_item);
    a_list->m_tail.m_prev->m_next = new_node;

    new_node->m_next = &a_list->m_tail;
    new_node->m_prev = a_list->m_tail.m_prev;
    a_list->m_tail.m_prev = new_node;

}


void *listRemove(struct list *a_list , void *a_key, objectCMP f_comp){
    Node            *current    = NULL;
    void            *result     = NULL;

    current = a_list->m_head.m_next;

    while(current != &a_list->m_tail && NULL == result){
        if(f_comp(current->m_item,a_key) == 0){
            result = current->m_item;
        }
        else {
            current = current->m_next;
        }
    }

    if(NULL != result){
        current->m_next->m_prev = current->m_prev;
        current->m_prev->m_next = current->m_next;
        free(current);
    }
    return result;
}


void *listSearch(struct list *a_list,const void *a_key, objectCMP f_comp){
    Node            *current    = NULL;
    void            *result     = NULL;

    current = a_list->m_head.m_next;

    while(current != &a_list->m_tail && NULL == result){
        if(f_comp(current->m_item,a_key) == 0){
            result = current->m_item;
        }
        current = current->m_next;
    }

    return result;
}


Boolean listIsEmpty(struct list *a_list){
    return (a_list->m_head.m_next == &a_list->m_tail)? True : False;
}




ListIterator listIterBegin(List a_list){
    return a_list->m_head.m_next;
}


ListIterator listIterEnd(List a_list){
    return a_list->m_tail.m_prev;
}


ListIterator listIterNext(ListIterator a_iter){
    return ((Node *)a_iter)->m_next;
}




void *listIterGetItem(ListIterator a_iter){
    return ((Node *)a_iter)->m_item;
}


void *listIterSetItem(ListIterator a_iter, void *a_item){
    void                *result = NULL;

    result = ((Node *)a_iter)->m_item;
    ((Node *)a_iter)->m_item = a_item;

    return result;
}

void ListIterInsert(ListIterator a_iter, void *a_item){
    Node            *iter       = a_iter,
    *new_node   = NULL;

    TEST_NULLS(a_iter,a_item);
    new_node = node_create(a_item);

    /** Connect next and prev to new_node.*/
    new_node->m_next = iter;
    new_node->m_prev = iter->m_prev;

    /** Connect new_node to next and prev.*/
    iter->m_prev->m_next = new_node;
    iter->m_prev = new_node;
}

Boolean listIterHasNext(ListIterator a_iter){
    Node                *current    = (Node *)a_iter;

    return (NULL != current->m_next)? True : False ;
}


void *listIterRemove(ListIterator a_iter){
    Node            *current    = NULL;
    void            *result     = NULL;

    current = ((Node *)a_iter);

    result = current->m_item;
    current->m_prev->m_next = current->m_next;
    current->m_next->m_prev = current->m_prev;

    free(current);

    return result;
}





#include "hashmap.h"
#include <assert.h>
#include "listiter.h"
struct hash_table{
    List                *m_map;
    size_t              m_capacity;
    size_t              m_num_of_items;
    hashfunc            m_hash_func_ptr;
};

HashMap hashMapCreate(size_t a_size, hashfunc f_hash){
    HashMap             result  = NULL;

    result = malloc(sizeof(struct hash_table));
    assert(NULL != result);

    result->m_map = calloc(a_size, sizeof(List));
    assert(NULL != result->m_map);

    result->m_capacity = a_size;
    result->m_num_of_items = 0;
    result->m_hash_func_ptr = f_hash;

    return result;

}

void hashMapDestroy(HashMap *a_map,destroy f_delete){
    size_t          index   = 0;
    HashMap         map     = NULL;

    assert(NULL != a_map);
    map = *a_map;

    if(NULL != map){
        for(index = 0 ; index < map->m_capacity; ++index){
            if(NULL != map->m_map[index]){
                listDestroy(&map->m_map[index],f_delete);
            }
           
        }
        free(map->m_map);
        free(map);
        *a_map = NULL;
    }
}

void *hashMapInsert(HashMap a_map,void *a_item,void *a_key, objectCMP f_cmp){
    size_t          index   = 0;
    ListIterator    iter    = NULL;
    void            *result = NULL;

    index = a_map->m_hash_func_ptr(a_key);

    assert(index >= 0 && index <a_map->m_capacity);
    
    if(NULL == a_map->m_map[index]){
        a_map->m_map[index] = listCreate();
        listInsert(a_map->m_map[index],a_item);
    }
    else {
        iter = listIterBegin(a_map->m_map[index]);
        while(listIterHasNext(iter) == True && NULL == result){
            if(f_cmp(listIterGetItem(iter) , a_item) == 0){
                result = listIterSetItem(iter,a_item);
            }
            else {
                iter = listIterNext(iter);
            }
        }
        if(NULL == result){
            listInsert(a_map->m_map[index],a_item);
            result = a_item;
        }
    }

    return result;

}

void *hashMapSearch(HashMap a_map, const void *a_key, object_key_cmp f_cmp){
    size_t              index   = 0;
    void                *result = NULL;
    ListIterator        iter    = NULL;

    index =a_map->m_hash_func_ptr(a_key);
    assert(0<=index && index<a_map->m_capacity);
    if(NULL != a_map->m_map[index]){
        iter = listIterBegin(a_map->m_map[index]);

        while(listIterHasNext(iter) == True && NULL == result){
            if(f_cmp(listIterGetItem(iter),a_key) == 0){
                result = listIterGetItem(iter);
            }
            else {
                iter = listIterNext(iter);
            }
        }
    }
    return result;
}

void *hashMapRemove(HashMap a_map,const void *a_key, object_key_cmp f_cmp){
    return NULL; /** Not relevant at the moment thus not implemented.*/
}

void forEach(HashMap a_map, Boolean (*predicate)(void *), void (*action)(void *, void *), void * param){
    size_t              index   = 0;
    ListIterator        iter    = NULL;
    void                *item   = NULL;

    for(index = 0; index < a_map->m_capacity; ++index){
        if(NULL != a_map->m_map[index]){
            iter = listIterBegin(a_map->m_map[index]);

            while(listIterHasNext(iter) == True){
                item = listIterGetItem(iter);

                if(predicate(item) == True){
                    action(item, param);
                }
                iter = listIterNext(iter);
            }
        }
    }
}

Boolean returnTrue(void *obj){
    return True;
}
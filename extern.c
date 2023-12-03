#include "extern.h"

#include <assert.h>
#include <string.h>

#define INIT_BLOCK_SIZE 16



struct extern_info{
    char                m_label[31];
    short               *m_addresses;
    size_t              m_size;
    size_t              m_capacity;
};


struct extern_info *externInfoCreate(const char *a_label){
    struct extern_info  *result = NULL;

    assert(NULL != a_label);
    result = malloc(sizeof(struct extern_info));
    assert(NULL != result);
    result->m_addresses = malloc(sizeof(short)*INIT_BLOCK_SIZE);
    assert(NULL != result->m_addresses);
    strcpy(result->m_label,a_label);
    result->m_capacity = INIT_BLOCK_SIZE;
    result->m_size = 0;

    return result;
}


void externInfoDestroy(struct extern_info **a_extern_info_ptr){
    assert(NULL != a_extern_info_ptr);
    if(*a_extern_info_ptr != NULL){
        free((*a_extern_info_ptr)->m_addresses);
        free(*a_extern_info_ptr);
        *a_extern_info_ptr = NULL;
    }
}


void externInfoInsert(struct extern_info *a_extern_info, short a_address){
    short           *ptr    = NULL;
    assert(NULL != a_extern_info);

    if(a_extern_info->m_capacity == a_extern_info->m_size){
        ptr = realloc(a_extern_info->m_addresses,2*a_extern_info->m_capacity* sizeof(short));
        assert(NULL != ptr);
        a_extern_info->m_addresses = ptr;
        a_extern_info->m_capacity*=2;
    }
    a_extern_info->m_addresses[a_extern_info->m_size++] = a_address;
}


short externInfoGetAddress(struct extern_info *a_extern_info, int a_index){
    assert(NULL != a_extern_info);
    assert(a_index>=0 && a_index<a_extern_info->m_size);

    return a_extern_info->m_addresses[a_index];
}


const char *externInfoGetLabel(struct extern_info *a_extern_info){
    assert(NULL != a_extern_info);
    return a_extern_info->m_label;
}

int externInfoKeyCompare(ExternInfo a_extern_info, const char *a_key){
    return strcmp(a_extern_info->m_label,a_key);
}

size_t externInfoGetSize(ExternInfo a_extern_info){
    assert(NULL != a_extern_info);
    return a_extern_info->m_size;
}
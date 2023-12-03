#include "entry.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


struct entry_info{
    char            m_label[31];
    short           m_address;
};


struct entry_info *entryInfoCreate(const char *a_label, short a_address){
    struct entry_info       *result = NULL;

    assert(NULL != a_label);
    result = malloc(sizeof(struct entry_info));
    assert(NULL != result);
    strcpy(result->m_label,a_label);
    result->m_address = a_address;

    return result;
}

void entryInfoDestroy(struct entry_info **a_entry_info_ptr){
    assert(NULL != a_entry_info_ptr);
    if(NULL != *a_entry_info_ptr){
        free(*a_entry_info_ptr);
        *a_entry_info_ptr = NULL;
    }
}


const char *entryInfoGetLabel(struct entry_info *a_entry_info){
    assert(NULL != a_entry_info);
    return a_entry_info->m_label;
}

short entryInfoGetAddress(struct entry_info *a_entry_info){
    assert(NULL != a_entry_info);
    return a_entry_info->m_address;
}

int entryInfoKeyCompare(struct entry_info *a_entry_info, const char *a_key){
    return strcmp(a_entry_info->m_label,a_key);
}
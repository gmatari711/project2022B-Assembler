#include "macro.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define INIT_BLOCK_SIZE 16
#define MAX_LINE_SIZE 80

typedef             char            LINE[MAX_LINE_SIZE + 1];

struct macro{
    LINE                *m_definition;
    char                m_name[31];
    unsigned int        m_size;
    unsigned int        m_capcity;
};


Macro macroCreate(const char *a_label){
    struct macro        *result = NULL;
    assert(NULL != a_label);

    result = malloc(sizeof(struct macro));
    assert(NULL != result);
    result->m_definition = calloc(INIT_BLOCK_SIZE, sizeof(LINE));
    assert(NULL != result->m_definition);
    result->m_size = 0;
    result->m_capcity = INIT_BLOCK_SIZE;
    strcpy(result->m_name,a_label);

    return result;
}

void macroDestroy(Macro *a_macro_ptr){
    assert(NULL != a_macro_ptr);
    if(NULL != *a_macro_ptr){
        free((*a_macro_ptr)->m_definition);
        free((*a_macro_ptr));
        *a_macro_ptr = NULL;
    }
}

void macroAddLine(Macro a_macro, const char *a_line){
    LINE        *array  = NULL;
    assert(NULL != a_macro && NULL != a_line);
    if(a_macro->m_size == a_macro->m_capcity){
        array = realloc(a_macro->m_definition, sizeof(LINE)*2*a_macro->m_capcity);
        assert(NULL != array);
        a_macro->m_capcity*=2;
        a_macro->m_definition = array;
    }
    strcpy(a_macro->m_definition[a_macro->m_size],a_line);
    ++a_macro->m_size;
}

const char *macroGetLine(Macro a_macro,int a_index){
    assert(NULL != a_macro);
    assert(a_index >=0 && a_index < a_macro->m_size);

    return a_macro->m_definition[a_index];
}

const char *macroGetName(Macro a_macro){
    assert(NULL != a_macro);
    return a_macro->m_name;
}

size_t macroGetSize(Macro a_macro){
    assert(NULL != a_macro);
    return a_macro->m_size;
}
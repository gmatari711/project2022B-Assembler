#include "macro_table.h"
#include "hashmap.h"
#include <ctype.h>


#define             ALPHABETS           26


struct macro_table{
    HashMap             m_table;
};

/**
 *
 * @param a_hval
 * @return
 */
static size_t hash_function(char *a_hval);

static int key_compare(Macro a_obj, const char *a_key){
    /* Compares between a_obj label and the given key.*/
    return strcmp(macroGetName(a_obj),a_key);
}


static int symbol_compare(Macro a_lhs, Macro a_rhs){
    return strcmp(macroGetName(a_lhs),macroGetName(a_rhs));
}


Macro macroTableContains(MacroTable a_macro_table, const char *a_label){
    return hashMapSearch(a_macro_table->m_table,a_label,(object_key_cmp)key_compare);
}


Macro macroTableInsert(MacroTable a_macro_table, Macro a_macro){
    return hashMapInsert(a_macro_table->m_table,a_macro,(void*)macroGetName(a_macro),(objectCMP)symbol_compare);
}


static size_t hash_function(char *a_hval){
    size_t result = 0;

    result = tolower(*a_hval) - 'a';

    return result;
}

MacroTable macroTableCreate(){
    MacroTable         result = NULL;

    result = malloc(sizeof(struct macro_table));
    TEST_NULL(result);

    result->m_table = hashMapCreate(ALPHABETS,(hashfunc)hash_function);
    TEST_NULL(result->m_table);

    return result;
}

void macroTableDestroy(MacroTable *a_macro_table_ptr){
    assert(NULL != a_macro_table_ptr);

    if(NULL != *a_macro_table_ptr){
        hashMapDestroy(&(*a_macro_table_ptr)->m_table,(destroy) macroDestroy);
        
        *a_macro_table_ptr = NULL;
    }
}

void macroTableForEach(MacroTable a_macro_table,predicate f_predicate, action f_action, void * param){
    forEach(a_macro_table->m_table,f_predicate,f_action,param);
}


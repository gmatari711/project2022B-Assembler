#include "symbol_table.h"
#include "hashmap.h"
#include <ctype.h>


#define             ALPHABETS           26


struct symbolLUT{
    HashMap             m_table;
};

/**
 *
 * @param a_hval
 * @return
 */
static size_t hash_function(char *a_hval);

static int key_compare(Symbol a_obj, const char *a_key){
    /* Compares between a_obj label and the given key.*/
    return strcmp(symbolGetLabel(a_obj),a_key);
}


static int symbol_compare(Symbol a_lhs, Symbol a_rhs){
    return strcmp(symbolGetLabel(a_lhs),symbolGetLabel(a_rhs));
}


Symbol symbolTableContains(SymbolTable a_symbol_table, const char *a_label){
    return hashMapSearch(a_symbol_table->m_table,a_label,(object_key_cmp)key_compare);
}


Symbol symbolTableInsert(SymbolTable a_symbol_table, Symbol a_symbol){
    return hashMapInsert(a_symbol_table->m_table,a_symbol,symbolGetLabel(a_symbol),(objectCMP)symbol_compare);
}


static size_t hash_function(char *a_hval){
    size_t result = 0;
    /* Converts te first letter to an index between 0 to 25 inclusive. */
    result = tolower(*a_hval) - 'a';

    return result;
}

SymbolTable symbolTableCreate(){
    SymbolTable         result = NULL;

    result = malloc(sizeof(struct symbolLUT));
    TEST_NULL(result);

    result->m_table = hashMapCreate(ALPHABETS,(hashfunc)hash_function);
    TEST_NULL(result->m_table);

    return result;
}

void symbolTableDestroy(SymbolTable *a_symbol_table_ptr){
    assert(NULL != a_symbol_table_ptr);

    if(NULL != *a_symbol_table_ptr){
        hashMapDestroy(&(*a_symbol_table_ptr)->m_table,(destroy) symbolDestroy);
        free(*a_symbol_table_ptr);
    }

}

void symbolTableForEach(SymbolTable a_symbol_table,predicate f_predicate, action f_action, void * param){
    forEach(a_symbol_table->m_table,f_predicate,f_action,param);
}



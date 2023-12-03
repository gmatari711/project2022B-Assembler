#include "symbol.h"


#define MAX_LABEL_SIZE 30




struct symbol{
    char                m_label[MAX_LABEL_SIZE +1];
    short               m_address;
    SymbolType          m_type;
};

/** Ctor & Dtor */

struct symbol *symbolCreate(const char *a_label,SymbolType a_type,short a_address){
    struct symbol             *result  = NULL;

    result = calloc(1,sizeof(struct symbol));
    TEST_NULL(result);

    strcpy(result->m_label,a_label);

    result->m_type = a_type;
    result->m_address = a_address;

    return result;
}

void symbolDestroy(void **a_symbol){
    struct symbol   *symbol_ptr = NULL;
    if(NULL != *a_symbol){
        symbol_ptr = (struct symbol *)*a_symbol;
        free(symbol_ptr);
        *a_symbol = NULL;
    }
}



/** Utility functions. */

int symbolCompare(struct symbol *a_lhs, struct symbol *a_rhs){
    return (strcmp(a_lhs->m_label,a_rhs->m_label));
}



/** Getter functions*/

char *symbolGetLabel(struct symbol *a_symbol){
    return a_symbol->m_label;
}

SymbolType symbolGetType(struct symbol *a_symbol){
    return a_symbol->m_type;
}

short symbolGetAddress(struct symbol *a_symbol){
    return a_symbol->m_address;
}


/**Setter functions*/



Symbol symbolSetAddress(struct symbol *a_symbol, short a_address){
    a_symbol->m_address = a_address;

    return a_symbol;
}

Symbol symbolSetType(struct symbol *a_symbol, SymbolType a_type){
    assert(a_symbol->m_type == st_null);/** KEEP INTEGRITY OF SYMBOL TYPE.*/

    a_symbol->m_type = a_type;

    return a_symbol;
}

Boolean is_symbol_data(Symbol a_symbol){
    return (symbolGetType(a_symbol) == st_directive)? True : False;
}

void update_symbol_address(Symbol a_symbol, const int *a_value){
    int             new_address = 0;

    new_address = (short)*a_value;
    new_address += symbolGetAddress(a_symbol);
    symbolSetAddress(a_symbol,(short)new_address);
}

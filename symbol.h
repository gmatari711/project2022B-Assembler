#ifndef ASSEMBLER_SYMBOL_H
#define ASSEMBLER_SYMBOL_H

#include "misc.h"
#include "hashmap.h"

typedef                 struct symbol               *Symbol;
typedef                 char                        Label[31];

typedef Boolean (*predicate)(void *);
typedef void (*action)(void *, void *);

typedef enum symbol_type{
    st_null,
    st_code,
    st_directive,
    st_extern
}SymbolType;


/**
 * Allocate and initialize a Symbol object
 * @param a_label A c-string to be associated with the Symbol object.
 * @param a_type An identifier of symbol type - code, data or external.
 * @param a_address A memory address associated with the symbol.
 * @return Initialized object.
 */
Symbol symbolCreate(const char *a_label,SymbolType a_type,short a_address);

/**
 * Free all memory allocated to object.
 * @param a_symbol A pointer to a symbol object.
 */
void symbolDestroy(void **a_symbol);


/**
 * Compare between to Symbol Objects.
 * @param a_lhs A Symbol object
 * @param a_rhs A Symbol object
 * @return <0 if a_lhs<a_rhs 0 if a_lhs = a_rhs >0 if a_lhs>a_rhs.
 */
int symbolCompare(Symbol a_lhs, Symbol a_rhs);

/**
 * Return symbol label
 * @param a_symbol A Symbol object.
 * @return a c-string representing symbols label.
 */
char *symbolGetLabel(Symbol a_symbol);

/**
 * Get type of symbol - code, data or extern.
 * @param a_symbol A symbol object.
 * @return Type identifier.
 */
SymbolType symbolGetType(Symbol a_symbol);

/**
 * Get symbol associated address.
 * @param a_symbol assoss
 * @return a short integer representing an address.
 */
short symbolGetAddress(Symbol a_symbol);
/**
 * Set a new memory address to be associated with Symbol object.
 * @param a_symbol A Symbol object
 * @param a_address A short integer representing a memory address.
 * @return
 */
Symbol symbolSetAddress(Symbol a_symbol, short a_address);

/**
 * Set a new type to Symbol object
 * @param a_symbol A Symbol object
 * @param a_type A symbol type identifier
 * @return The Symbol object.
 */
Symbol symbolSetType(Symbol a_symbol, SymbolType a_type);
/**
 * Test if type associated with the Symbol object is of type data.
 * @param a_symbol A Symbol object
 * @return True(1) if is of type data. False(0) otherwise.
 */
Boolean is_symbol_data(Symbol a_symbol);
/**
 * A wrapper function to match the action function of forEach loop/
 * @param a_symbol A Symbol object
 * @param a_value a pointer to const int which represent a value of increment.
 */
void update_symbol_address(Symbol a_symbol, const int *a_value);





#endif
#ifndef ASSEMBLER_SYMBTBL_H
#define ASSEMBLER_SYMBTBL_H

#include "misc.h"
#include "symbol.h"

typedef             struct symbolLUT             *SymbolTable;


/**
 *
 * @param a_symbol_table
 * @param a_label
 * @return
 */
Symbol symbolTableContains(SymbolTable a_symbol_table, const char *a_label);

/**
 *
 * @param a_symbol_table
 * @param a_symbol
 * @return
 */
Symbol symbolTableInsert(SymbolTable a_symbol_table, Symbol a_symbol);

/**
 *
 * @return
 */
SymbolTable symbolTableCreate();

/**
 *
 * @param a_symbol_table_ptr
 */
void symbolTableDestroy(SymbolTable *a_symbol_table_ptr);

/**
 *
 * @param a_symbol_table
 * @param predicate
 * @param action
 * @param param
 */
void symbolTableForEach(SymbolTable a_symbol_table, Boolean (*predicate)(void *), void (*action)(void *, void *), void * param);


#endif

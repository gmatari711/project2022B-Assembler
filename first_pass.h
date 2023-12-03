#ifndef ASSEMBLER_BETA_FIRST_PASS_H
#define ASSEMBLER_BETA_FIRST_PASS_H
#include "word.h"
#include "symbol_table.h"
#include "error_queue.h"

/**
 * Build the symbol table for the assembler, the data image and the most of the code image 
 * with the exception of relocatable and extern operands.
 * @param a_filename A presumed valid file name of an assembly code after macro layouts.
 * @param a_CI A container for the code image.
 * @param a_DI A container for the data image.
 * @param a_table A container for symbols.
 * @param a_queue A queue for errors - if found.
 */
void first_pass(const char *a_filename, Block a_CI, Block a_DI,SymbolTable a_table, ErrQueue a_queue);


#endif

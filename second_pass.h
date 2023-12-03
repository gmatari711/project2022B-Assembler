#ifndef ASSEMBLER_BETA_SECOND_PASS_H
#define ASSEMBLER_BETA_SECOND_PASS_H
#include "word.h"
#include "symbol_table.h"
#include "error_queue.h"

/**
 *
 * @param a_filename A presumed valid file name of an assembly code after macro layouts.
 * @param a_CI A container for the code image.
 * @param a_table A container for symbols.
 * @param a_queue A queue for errors - if found.
 * @param a_ext A List object container of ExternInfo objects
 * @param a_ent A List object container of EntryInfo objects
 */
void second_pass(const char *a_filename, Block a_CI,SymbolTable a_table, ErrQueue a_queue, List a_ext, List a_ent);


#endif
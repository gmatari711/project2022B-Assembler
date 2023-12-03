#ifndef ASSEMBLER_BETA_MACRO_TABLE_H
#define ASSEMBLER_BETA_MACRO_TABLE_H

#include "misc.h"
#include "macro.h"

typedef             struct macro_table             *MacroTable;


/**
 * Test if an element is containd in the macro table with a label matching a_label.
 * @param a_macro_table A MacroTable object.
 * @param a_label a const char pointer to a string that represents a label.
 */
Macro macroTableContains(MacroTable a_macro_table, const char *a_label);

/**
 * Insert a new macro to table.
 * @param a_macro_table A MacroTable object
 * @param a_macro A Macro object.
 * @return Returns a former existing macro with the same label or NULL. 
 */
Macro macroTableInsert(MacroTable a_macro_table, Macro a_macro);

/**
 * Allocates and initialize a new MacroTable object.
 * @return An initialized MacroTable object.
 */
MacroTable macroTableCreate();

/**
 * Free All memory allocated for macro table and its items.
 * @param a_macro_table_ptr A pointer to a MacroTable object.
 */
void macroTableDestroy(MacroTable *a_macro_table_ptr);

/**
 * A version of foreach extended by the use of a predicate function. 
 * The forEach loop apply action function to any item which uphold the predicate.
 * @param a_symbol_table A MacroTable object.
 * @param predicate A function recieving an item and return a Boolean value.
 * @param action A functions recieving an item and a pointer and preform a task the item.
 * @param a_param A parameter for the action function. could be passed with NULL if not needed.
 */
void macroTableForEach(MacroTable a_symbol_table, Boolean (*predicate)(void *), void (*action)(void *, void *), void * param);




#endif

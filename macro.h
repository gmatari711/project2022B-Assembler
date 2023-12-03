#ifndef ASSEMBLER_BETA_MACRO_H
#define ASSEMBLER_BETA_MACRO_H
#include "misc.h"

typedef             struct macro        *Macro;

typedef Boolean (*predicate)(void *);
typedef void (*action)(void *, void *);

/**
 * Create and allocate a new Macro object
 * @param a_label a c-string to be set to object.
 * @return Macro object associated with a_label label.
 */
Macro macroCreate(const char *a_label);
/**
 * Free all memory allocated for Macro object.
 * @param a_macro_ptr A pointer to Macro object.
 */
void macroDestroy(Macro *a_macro_ptr);
/**
 * Insert a line of text in a serial manner to the Macro object.
 * @param a_macro A Macro object.
 * @param a_line a line of text (c-string).
 */
void macroAddLine(Macro a_macro, const char *a_line);
/**
 * Retrieve a line of text from a given position.
 * @param a_macro A Macro object.
 * @param a_index an integer refers to the serial position of the line of text.
 * @return The index serially positioned line of text.
 */
const char *macroGetLine(Macro a_macro,int a_index);
/**
 * Retrieve label associated with macro.
 * @param a_macro A Macro object.
 * @return A c-string representing Macro object label.
 */
const char*macroGetName(Macro a_macro);
/**
 * Get the number of c-strings kept in Macro object.
 * @param a_macro A Macro object.
 * @return An unsigned integer representing the number of c-strings in Macro.
 */
size_t macroGetSize(Macro a_macro);

#endif

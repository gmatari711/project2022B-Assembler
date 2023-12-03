#ifndef ASSEMBLER_BETA_FILE_BUILDER_H
#define ASSEMBLER_BETA_FILE_BUILDER_H
#include "entry.h"
#include "extern.h"
#include "word.h"
#include "list.h"
#include "listiter.h"

/**
 * Build an extern file which contains all external information - Labels and associated addresses.
 * @param a_filename A const char pointer which points to a string representing a name for the file.
 * @param a_external A list of ExternInfo objects.
 */
void build_external_file(const char *a_filename, List a_external);
/**
 * Build an entry file which contains all entries information - Labels and associated addresses.
 * @param a_filename  A const char pointer which points to a string representing a name for the file.
 * @param a_entry A list of EntryInfo objects.
 */
void build_entry_file(const char *a_filename, List a_entry);
/**
 * Build an entry file which contains CI and DI information - Addresses and associated instruction words.
 * @param a_filename A const char pointer which points to a string representing a name for the file.
 * @param a_CI The code image container.
 * @param a_DI The data image container.
 */
void build_object_file(const char *a_filename, Block a_CI, Block a_DI);
/**
 * Open and an assembly file with a given name .
 * @param a_filename A const char pointer to string which holds a name of a file to open (without assembly suffix).
 * @param a_mode A mode to open the file as.
 * @return A pointer to a FILE representing the opened file.
 */
FILE *open_assembly_file(const char *a_filename, const char *a_mode);
/**
 * Open and an extern file with a given name .
 * @param a_filename
 * @param a_mode A mode to open the file as.
 * @return A pointer to a FILE representing the opened file. 
 */
FILE *open_extern_file(const char *a_filename, const char *a_mode);
/**
 * Open and an assembly file with a given name .
 * @param a_filename
 * @param a_mode A mode to open the file as.
 * @return A pointer to a FILE representing the opened file. 
 */
FILE *open_entry_file(const char *a_filename, const char *a_mode);
/**
 * Open and a pre_assembly file with a given name .
 * @param a_filename
 * @param a_mode A mode to open the file as.
 * @return A pointer to a FILE representing the opened file. 
 */
FILE *open_pre_assembler_file(const char *a_filename, const char *a_mode);
#endif

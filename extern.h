#ifndef ASSEMBLER_EXTERN_INFO_H
#define ASSEMBLER_EXTERN_INFO_H
#include "list.h"
#include "listiter.h"
#include "misc.h"

typedef             struct extern_info          *ExternInfo;
typedef             void                        *ExternAddressIterator;

/**
 * Allocate memory for a new ExternInfo object.
 * @param a_label A string representing extern's lable.
 * @return An
 */
ExternInfo externInfoCreate(const char *a_label);
/**
 * Deallocate all memory allocated to pointed ExternInfo object. object set to NULL. 
 * @param a_extern_info_ptr A  pointer to an ExternInfo object.
 */
void externInfoDestroy(ExternInfo *a_extern_info_ptr);
/**
 * Insert a new address sequencially.
 * @param a_extern_info an ExternInfo object.
 * @param a_address a short integer representing an address.
 */
void externInfoInsert(ExternInfo a_extern_info, short a_address);
/**
 * A getter function for the address that was inserted 'a_index' in turn.  
 * @param a_extern_info An ExternInfo Object.
 * @param index an integer represent the position in sequence to retrieve.
 * @return The object on the given index.
 */
short externInfoGetAddress(ExternInfo a_extern_info, int a_index);
/**
 * Retrieve label associated with the ExternInfo object.
 * @param a_extern_info An ExternIInfo object.
 * @return A const char pointer which contains a string representing a_extern_info's label.
 */
const char *externInfoGetLabel(ExternInfo a_extern_info);
/**
 * Utility function to compare between object's label and a given string.
 * @param a_extern_info An ExternIInfo object.
 * @param a_key A pointer to const char which contains a string for comparison.
 * @return <0 value if label < string, 0 if label = string and >0 if label > string.
 */
int externInfoKeyCompare(ExternInfo a_extern_info, const char *a_key);
/**
 * Retrieve the number of addresses associated with a_extern_info
 * @param a_extern_info An ExternIInfo object.
 * @return An unsigned integer representing the number of addresses associated with a_extern_info.
 */
size_t externInfoGetSize(ExternInfo a_extern_info);
#endif

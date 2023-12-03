#ifndef ASSEMBLER_BETA_ENTRY_H
#define ASSEMBLER_BETA_ENTRY_H

typedef             struct entry_info           *EntryInfo;

/**
 * Allocate memory for a new EntryInfo object.
 * @param a_label A string representing entry's lable.
 * @param a_address A short integer representing entry's address.
 * @return An EntryInfo object
 */
EntryInfo entryInfoCreate(const char *a_label, short a_address);
/**
 * Deallocate all memory allocated to pointed EntryInfo object. 
 * The Element gets NULL value after destruction.
 * @param a_entry_info_ptr A pointer to an EntryInfo object to be destroyed.
 */
void entryInfoDestroy(EntryInfo *a_entry_info_ptr);
/**
 * A getter function to retrieve entry's label.
 * @param a_entry_info An EntryInfo object.
 * @return A pointer to const char which holds a_entry_info's label.
 */
const char *entryInfoGetLabel(EntryInfo a_entry_info);
/**
 * A getter function to retrieve entry's address.
 * @param a_entry_info An EntryInfo object.
 * @return A short value representing entry's address.
 */
short entryInfoGetAddress(EntryInfo a_entry_info);
/**
 * Utility function to compare between object's label and a given string.
 * @param a_entry_info An EntryInfo object.
 * @param a_key A pointer to const char which contains a string for comparison.
 * @return <0 value if label < string, 0 if label = string and >0 if label > string.
 */
int entryInfoKeyCompare(EntryInfo a_entry_info, const char *a_key);
#endif

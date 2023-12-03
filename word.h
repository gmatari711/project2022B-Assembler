#ifndef ASSEMBLER_BETA_WORD_H
#define ASSEMBLER_BETA_WORD_H
#include <stdlib.h>

typedef             struct word             *Word;
typedef             struct block            *Block;



/**
 * Allocate memory for a new Block object and returns its instance address.
 * @param a_block_size The initial capacity of allocated block.
 * @return A new Block object address.
 */
Block blockCreate(int a_block_size);

/**
 * Deallocates all memory allocated to object Block *a_block.
 * @param a_block_ptr a pointer to a Block object.
 */
void blockDestroy(Block *a_block_ptr);

/**
 * Returns the number of elements set on a_block.
 * @param a_block The Block object.
 * @return number of elements in a_block.
 */
size_t blockGetSize(Block a_block);


/**
 * Inserts a new value to a_dest Block object.
 * @param a_dest Given Block object.
 * @param a_value The new value to be set on to the next word in the block.
 */
void blockInsert(Block a_dest, short a_value);

/**
 * Retrieve a Word object from a given index in the a_block.
 * @param a_block The block to retrieve the word from.
 * @param a_index The index which from we want to retrieve the word.
 * @return a Word object if valid. NULL otherwise.
 */
Word blockGetWord(Block a_block, unsigned int a_index);


/**
 * Allocate memory for a new Word object and returns its instance address.
 * @param a_value initialization value for the word.
 * @return A new Word object instance address.
 */
Word wordCreate(short a_value);

/**
 * Deallocate memory for Word object, and set its pointer to NULL.
 * @param a_word_ptr a pointer to a Word object.
 */
void wordDestroy(Word *a_word_ptr);

/**
 * Set a new value to a given word.
 * @param a_word The Word object to have its value set.
 * @param a_value The new value.
 * @return a_word.
 */
Word wordSetValue(Word a_word,short a_value);

/**
 * Retrieve the numeric value of a Word object.
 * @param a_word The Word object.
 * @return a_word numeric value as a short int.
 */
short wordGetValue(Word a_word);


#endif

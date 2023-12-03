#include <assert.h>
#include <stdlib.h>
#include "word.h"

#define WORD_SIZE 10

struct block{
    struct word         *m_block;
    unsigned int        m_size;
    unsigned int        m_capacity;
};


struct word{
    int         word:   WORD_SIZE;
};

/**
 * Doubles the capacity of a given struct block.
 * @param a_block The struct block.
 */
static void expand_block(struct block *a_block);

Block blockCreate(int a_block_size){
    struct block        *result = NULL;

    assert(a_block_size > 0);
    /* Allocate memory for new object. */
    result = malloc(sizeof(struct block));
    assert(NULL != result);
    /* Allocate memory for a block of Word objects.*/
    result->m_block = calloc(a_block_size, sizeof(struct word));
    assert(NULL != result->m_block);

    /* Set initial values. */
    result->m_size = 0;
    result->m_capacity = a_block_size;

    return result;
}

void blockDestroy(Block *a_block_ptr){
    assert(NULL != a_block_ptr);

    if(NULL != *a_block_ptr){
        free((*a_block_ptr)->m_block);
        free((*a_block_ptr));
        *a_block_ptr = NULL;
    }
}

size_t blockGetSize(Block a_block){
    assert(NULL != a_block);
    return a_block->m_size;
}

void blockInsert(struct block *a_dest, short a_value){
    assert(NULL != a_dest);
    /* Test if block is full. if it is - double its capacity. */
    if(a_dest->m_size >= a_dest->m_capacity){
        expand_block(a_dest);
    }
    /* Set new value to block. */
    a_dest->m_block[a_dest->m_size++].word = a_value;
}


Word blockGetWord(struct block *a_block, unsigned int a_index){
    assert(a_index < a_block->m_size);
    return a_block->m_block + a_index;
}

Word wordCreate(short a_value){
    struct word *result = malloc(sizeof(struct word));
    assert(NULL != result);
    result->word = a_value;

    return result;
}
void wordDestroy(struct word **a_word_ptr){
    assert(NULL != a_word_ptr);
    if(NULL != *a_word_ptr){
        free(*a_word_ptr);
        *a_word_ptr = NULL;
    }
}

Word wordSetValue(struct word *a_word, short a_value){
    assert(NULL != a_word);
    a_word->word = a_value;
    return a_word;
}

short wordGetValue(struct word *a_word){
    assert(NULL != a_word);
    return (short)a_word->word;
}


static void expand_block(struct block *a_block){
    struct word         *new_block      = NULL;
    size_t              new_capacity    = 0;

    assert(NULL != a_block);

    new_capacity = a_block->m_capacity * 2;
    new_block = realloc(a_block->m_block, new_capacity * sizeof(struct word));
    assert(NULL != new_block);

    a_block->m_capacity = new_capacity;
    a_block->m_block = new_block;
}

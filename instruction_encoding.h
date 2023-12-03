#ifndef ASSEMBLER_BETA_INSTRUCTION_ENCODING_H
#define ASSEMBLER_BETA_INSTRUCTION_ENCODING_H

#include "word.h"
/**
 *
 * @param a_word
 * @param a_are
 */
void set_encoding_value(short *a_word, int a_are);
/**
 *
 * @param a_word
 * @param a_opcode
 */
void set_opcode_value(short *a_word, int a_opcode);
/**
 *
 * @param a_word
 * @param a_addressing_value
 */
void set_source_addressing_type(short *a_word, int a_addressing_value);
/**
 *
 * @param a_word
 * @param a_addressing_value
 */
void set_dest_addressing_type(short *a_word, int a_addressing_value);
/**
 *
 * @param a_word
 * @param a_register_value
 */
void set_source_register_value(short *a_word, int a_register_value);
/**
 *
 * @param a_word
 * @param a_register_value
 */
void set_dest_register_value(short *a_word, int a_register_value);
/**
 *
 * @param a_word
 * @param a_address_value
 */
void set_address_value(short *a_word, int a_address_value);
/**
 *
 * @param a_word
 * @param a_struct_field
 */
void set_struct_field_value(short *a_word, int a_struct_field);

/**
 *
 * @param a_word
 * @param a_struct_field
 */
void set_immediate_value(short *a_word, int a_struct_field);



#endif
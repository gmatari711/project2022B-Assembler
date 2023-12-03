#ifndef ASSEMBLER_BETA_PARSE_DATA_H
#define ASSEMBLER_BETA_PARSE_DATA_H

#include "misc.h"

typedef                 struct args         *ParsedData;

/** Assembler Instructions*/
typedef enum asm_inst{
    inst_none =-1 , inst_mov, inst_cmp , inst_add, inst_sub,
    inst_not, inst_clr, inst_lea, inst_inc, inst_dec, inst_jmp,
    inst_bne, inst_get, inst_prn, inst_jsr, inst_rts, inst_hlt
}AssemblerInst;

/** Addressing Type*/
typedef enum type_addr{
    addr_immed = 0,
    addr_direct,
    addr_struct,
    addr_register,
    addr_none
}AddrType;

/** Encoding Type - ARE*/
typedef enum enc_type{
    enc_absolute = 0,
    enc_external,
    enc_relocate
}EncodeType;

/** Each value represent a specific directive of assembly language.*/
typedef enum dir{
    dir_none = -1,
    dir_data,
    dir_struct,
    dir_string,
    dir_extern,
    dir_entry
}Directive;


/** Each value represent a specific error. */
typedef enum err_list{
    err_none = 0,
    err_illegal_directive,
    err_illegal_instruction,
    err_illegal_comma,
    err_consecutive_comma,
    err_not_integer,
    err_missing_comma,
    err_illegal_string,
    err_extraneous_text,
    err_illegal_operand,
    err_illegal_label,
    err_missing_operand,
    err_illegal_addr_type,
    err_invalid_register,
    err_duplicate_symbol,
    err_symbol_not_found,
    err_illegal_struct_field,
    err_empty_label,
    err_missing_data,
    err_missing_string,
    err_missing_struct,
    err_struct_missing_string
}Status;

typedef enum _group{
    inst_group_1,
    inst_group_2,
    inst_group_3
}InstructionGroup;

/**
 * Parse a given line of code and extract all information needed for the assembler, including errors found.
 * @param a_str A line of assembly code.
 * @return A static ParsedData object containing all information needed for the assembler.
 */
ParsedData process_command(const char *a_str);

/**
 * Tests if the ParsedData object contains information of a directive.
 * @param a_parsed_data A ParsedData object.
 * @return True if the information in a_parsed_data is of a directive. False otherwise.
 */
Boolean parsedDataIsDirective(ParsedData a_parsed_data);

/**
 * Tests if the ParsedData object contains information of a instruction.
 * @param a_parsed_data A ParsedData object.
 * @return True if the information in a_parsed_data is of a instruction. False otherwise.
 */
Boolean parsedDataIsInstruction(ParsedData a_parsed_data);

/**
 * Tests if the ParseData object contains an information of a label.
 * @param a_parsed_data A ParsedData object.
 * @return True if a_parsed_data holds information of a label. False otherwise.
 */
Boolean parsedDataHasLabel(ParsedData a_parsed_data);

/**
 * Tests if the ParseData object contains an information of an error.
 * @param a_parsed_data A ParsedData object.
 * @return True if a_parsed_data holds information of an error. False otherwise.
 */
Boolean parsedDataErrFound(ParsedData a_parsed_data);
/**
 * Extract directive type fro, a_parsed_data.
 * @param a_parsed_data A ParsedData object.
 * @return directive identifier.
 */
Directive parsedDataGetDirective(ParsedData a_parsed_data);
/**
 * Extract instruction identifier,
 * @param a_parsed_data A ParsedData object.
 * @return An identifier of an instruction.
 */
AssemblerInst parsedDataGetInstruction(ParsedData a_parsed_data);
/**
 * Extract instruction group.
 * @param a_parsed_data A ParsedData object.
 * @return instruction group identifier.
 */
InstructionGroup parsedDataGetInstructionGroup(ParsedData a_parsed_data);
/**
 * Extract destination addressing type
 * @param a_parsed_data A ParsedData object.
 * @return An addressing type identifier.
 */
AddrType parsedDataGetDestAddressing(ParsedData a_parsed_data);
/**
 * Extract source addressing type
 * @param a_parsed_data A ParsedData object.
 * @return An addressing type identifier.
 */
AddrType parsedDataGetSourceAddressing(ParsedData a_parsed_data);

/**
 * Get error number
 * @param a_parsed_data A ParsedData object.
 * @return an error identifier.
 */
Status parsedDataGetErrNumber(ParsedData a_parsed_data);

/**
 * Get destination numeric value.
 * @param a_parsed_data A ParsedData object.
 * @return a short integer represents the destination value.
 */
short parsedDataGetDestValue(ParsedData a_parsed_data);
/**
 * Get source numeric value.
 * @param a_parsed_data A ParsedData object.
 * @return a short integer represents the source value.
 */
short parsedDataGetSourceValue(ParsedData a_parsed_data);
/**
 * Extract struct directive integeric associated value
 * @param a_parsed_data A ParsedData object.
 * @return a short integer represents struct integer value.
 */
short parsedDataGetStructInteger(ParsedData a_parsed_data);
/**
 * Extract struct directive string associated value
 * @param a_parsed_data A ParsedData object.
 * @return a c-string represents struct string value.
 */
const char *parsedDataGetStructString(ParsedData a_parsed_data);
/**
 * Extract data array asociated with current data directive.
 * @param a_parsed_data A ParsedData object.
 * @return A pointer to a short integer array.
 */
const short *parsedDataGetDataArray(ParsedData a_parsed_data);
/**
 * Get the size of current data associated array.
 * @param a_parsed_data A ParsedData object.
 * @return An unsigned int representing the size of the array.
 */
size_t  parsedDataGetDataArraySize(ParsedData a_parsed_data);
/**
 * Get label associated with source operand of current instruction.
 * @param a_parsed_data A ParsedData object.
 * @return A c-string representing a label.
 */
const char *parsedDataGetSourceLabel(ParsedData a_parsed_data);
/**
 * Get label associated with destination operand of current instruction. 
 * @param a_parsed_data A ParsedData object.
 * @return A c-string representing a label.
 */
const char *parsedDataGetDestLabel(ParsedData a_parsed_data);
/**
 * Get string associated with current string directive.
 * @param a_parsed_data A ParsedData object
 * @return A c-string associated with string directive.
 */
const char *parsedDataGetString(ParsedData a_parsed_data);
/**
 * Get current label.
 * @param a_parsed_data A ParsedData.
 * @return A c-string representing current label.
 */
const char *parsedDataGetLabel(ParsedData a_parsed_data);
/**
 * Get label associated with current extern directive.
 * @param a_parsed_data a_parsed_data A ParsedData.
 * @return A c-string representing a label associated with extern directive.
 */
const char *parsedDataGetExternLabel(ParsedData a_parsed_data);
/**
 * Get label associated with current entry directive.
 * @param a_parsed_data a_parsed_data A ParsedData.
 * @return A c-string representing a label associated with entry directive.
 */
const char *parsedDataGetEntryLabel(ParsedData a_parsed_data);

#endif

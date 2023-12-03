#include "first_pass.h"
#include "parse_data.h"
#include "instruction_encoding.h"
#include "file_builder.h"
#include "misc.h"
#include <stdlib.h>

/**
 * Extract information from ParsedData object and update Code Image and Symbol Table accordingly.
 * @param a_parsed_data The ParsedData object containing the assembly line information.
 * @param a_CI The Block object representing the current Code Image.
 * @param a_table The Symbol Table holding all current symbols.
 * @param a_queue The error queue.
 * @return The number of words added to the Code Image.
 */
static int process_instruction(ParsedData a_parsed_data, Block a_CI, SymbolTable a_table, ErrQueue a_queue, int a_line);
/**
 * Produce memory words from the given directive.
 * @param a_parsed_data An object of ParsedData which holds parsed information from file.
 * @param a_DI The Block object representing the current Data Image.
 * @param a_table The Symbol Table holding all current symbols.
 * @param a_queue The error queue.
 * @return The number of words added to the Data Image.
 */
static int process_directive(ParsedData a_parsed_data, Block a_DI, SymbolTable a_table, ErrQueue a_queue, int a_line);
/**
 * Builds an instruction word via information contained in the ParsedData object.
 * @param a_parse_data A ParsedData object containing relevant instruction information.
 * @return return a short int representing the extracted instruction word.
 */
static short build_instruction_word(ParsedData a_parsed_data);
/**
 * Produce memory words from instruction found on a_parsed_data which belongs to
 * instruction group 1 and set them to a_CI.
 * @param a_parsed_data A ParsedData object.
 * @param a_CI The Block object representing the current Code Image. 
 */
static void process_instruction_group_one(ParsedData a_parsed_data, Block a_CI);
/**
 * Produce memory words from instruction found on a_parsed_data which belongs to
 * instruction group 2 and set them to a_CI.
 * @param a_parsed_data A ParsedData object.
 * @param a_CI The Block object representing the current Code Image. 
 */
static void process_instruction_group_two(ParsedData a_parsed_data, Block a_CI);
/**
 * process string directive information from a_parsed_data and set them to a_DI.
 * @param a_parsed_data A ParsedData object.
 * @param a_DI The Block object representing the current Data Image. 
 */
static void process_string(ParsedData a_parsed_data, Block a_DI);
/**
 * process struct directive information from a_parsed_data and set them to a_DI.
 * @param a_parsed_data A ParsedData object.
 * @param a_DI The Block object representing the current Data Image. 
 */
static void process_struct(ParsedData a_parsed_data, Block a_DI);
/**
 * process data directive information from a_parsed_data and set them to a_DI.
 * @param a_parsed_data A ParsedData object.
 * @param a_DI The Block object representing the current Data Image. 
 */
static void process_data(ParsedData a_parsed_data, Block a_DI);
/**
 * process data directive information from a_parsed_data and set them to symbol table 
 * a_table.
 * @param a_parsed_data A ParsedData object.
 * @param a_table The Symbol Table holding all current symbols.
 */
static void process_extern(ParsedData a_parsed_data, SymbolTable a_table);


void first_pass(const char *a_filename, Block a_CI, Block a_DI,SymbolTable a_table, ErrQueue a_queue){
    ParsedData          parsed_data                     = NULL;
    int                 CI_counter                      = 0,
                        DI_counter                      = 0,
                        line_counter                    = 1;
    FILE                *current_file                   = NULL;
    char                current_line[MAX_LINE_LENGTH+1] = {0};

    current_file = open_pre_assembler_file(a_filename, "r");
    assert(NULL != current_file);

    while (fgets(current_line, MAX_LINE_LENGTH, current_file)){
        /* Parse line of assembly code. */
        parsed_data = process_command(current_line);
        /* If an error was found - add error information to error queue. */
        if(parsedDataErrFound(parsed_data) == True){
            errQueueEnqueue(a_queue,line_counter,parsedDataGetErrNumber(parsed_data));
        }
        /* If error is found - the building of the DI, CI and symbol table comes to a
         * holt. Duplicate symbols will not be detected.*/
        else if(errQueueIsEmpty(a_queue) == True){
            /* Process directive information and set it to code image (CI). */
            if(parsedDataIsDirective(parsed_data) == True){
                DI_counter += process_directive(parsed_data,a_DI,a_table,a_queue,line_counter);
            }
            /* Process instruction inforrmation and set it to code image (DI). */
            else if(parsedDataIsInstruction(parsed_data) == True){
                CI_counter += process_instruction(parsed_data,a_CI,a_table,a_queue,line_counter);
            }
        }

        ++line_counter;
    }
    fclose(current_file);
}

static int process_instruction(ParsedData a_parsed_data, Block a_CI, SymbolTable a_table, ErrQueue a_queue, int a_line){
    short           current_word_value  =   0,
                    init_CI_size        =   0;
    Symbol          symbol              = NULL;

    init_CI_size = blockGetSize(a_CI);

    if(parsedDataHasLabel(a_parsed_data) == True){
        /* Check for duplicate labels. */
        if(symbolTableContains(a_table,parsedDataGetLabel(a_parsed_data)) == NULL){
            /* Insert address of code image size + threshold (of 100) */
            symbol = symbolCreate(parsedDataGetLabel(a_parsed_data),st_code,blockGetSize(a_CI) + MIN_THRESHOLD);
            symbolTableInsert(a_table,symbol);
        }
        else {
            /* Error detected - we only save records of syntax errors. */
            errQueueEnqueue(a_queue,a_line,err_duplicate_symbol);
        }
    }
    current_word_value = build_instruction_word(a_parsed_data);
    blockInsert(a_CI,current_word_value);
    /* Init memory word. */
    current_word_value = 0x00;
    /* Handle instruction group one. */
    if(parsedDataGetInstructionGroup(a_parsed_data) == inst_group_1){
        process_instruction_group_one(a_parsed_data,a_CI);
    }/* Handle instruction group two. */
    else if(parsedDataGetInstructionGroup(a_parsed_data) == inst_group_2){
        process_instruction_group_two(a_parsed_data,a_CI);
    }
    /* Return the number of words inserted. */
    return blockGetSize(a_CI) - init_CI_size;
}

static int process_directive(ParsedData a_parsed_data, Block a_DI, SymbolTable a_table, ErrQueue a_queue, int a_line){
    Symbol              symbol              = NULL;
    Directive           directive           = dir_none;
    int                 init_DI_size        = 0;

    init_DI_size = blockGetSize(a_DI);
    directive = parsedDataGetDirective(a_parsed_data);

    if(parsedDataHasLabel(a_parsed_data) == True){
        /* Check for duplicate labels. */
        if(symbolTableContains(a_table,parsedDataGetLabel(a_parsed_data)) == NULL){
            /* Insert address of data image size + threshold (of 100) */
            if(directive != dir_extern && directive != dir_entry){
                symbol = symbolCreate(parsedDataGetLabel(a_parsed_data),st_directive,blockGetSize(a_DI) + MIN_THRESHOLD);
                symbolTableInsert(a_table,symbol);
            }
        }
        else {
            /* Error detected - we only save records of syntax errors. */
            errQueueEnqueue(a_queue,a_line,err_duplicate_symbol);
        }
    }
    switch (directive) {
        case dir_string:
        /* Set string to data image. */
            process_string(a_parsed_data,a_DI);
            break;
        case dir_struct:
        /* Set struct to data image. */
            process_struct(a_parsed_data,a_DI);
            break;
        case dir_data:
        /* Set stream of integers to data image. */
            process_data(a_parsed_data,a_DI);
            break;
        case dir_extern:
            if(symbolTableContains(a_table,parsedDataGetExternLabel(a_parsed_data)) != NULL){
                /* Error detected - we only save records of syntax errors. */
                errQueueEnqueue(a_queue,a_line,err_duplicate_symbol);
            }
            else{
                /* Insert extern to table. */
                process_extern(a_parsed_data,a_table);
            }
            break;
        default:
        /* Entries are not handled in the first pass. */
            break;
        }
    /* Return number of inserted mwmory words.  */
    return blockGetSize(a_DI) - init_DI_size;
}



static short build_instruction_word(ParsedData a_parsed_data){
    short               result  = 0x00;

    /* Apply instruction value. */
    set_opcode_value(&result, parsedDataGetInstruction(a_parsed_data));
    /* Apply source addressing value. */
    set_source_addressing_type(&result, parsedDataGetSourceAddressing(a_parsed_data));
    /* Apply destination addressing value. */
    set_dest_addressing_type(&result, parsedDataGetDestAddressing(a_parsed_data));


    return result;
}

static void process_instruction_group_one(ParsedData a_parsed_data, Block a_CI){
    AddrType            source_addressing_type  = addr_none,
                        dest_addressing_type    = addr_none;
    short               current_word_value      = 0x00;

    source_addressing_type = parsedDataGetSourceAddressing(a_parsed_data);
    dest_addressing_type = parsedDataGetDestAddressing(a_parsed_data);

    /* Special case: Both operands are registers - share a single word.*/
    if(source_addressing_type == addr_register && dest_addressing_type == addr_register){
        /* Set encoding type - Absolute. */
        set_encoding_value(&current_word_value,enc_absolute);
        set_source_register_value(&current_word_value,parsedDataGetSourceValue(a_parsed_data));
        set_dest_register_value(&current_word_value,parsedDataGetDestValue(a_parsed_data));
        blockInsert(a_CI,current_word_value);
    }
    else{
        switch (source_addressing_type) {
            case addr_struct:
                /* The first slot reserved to the address which is not yet available.*/
                blockInsert(a_CI,0x00);
                set_struct_field_value(&current_word_value, parsedDataGetSourceValue(a_parsed_data));
                break;
            case addr_register:
                set_encoding_value(&current_word_value,enc_absolute);
                set_source_register_value(&current_word_value,parsedDataGetSourceValue(a_parsed_data));
                break;
            case addr_immed:
                set_encoding_value(&current_word_value,enc_absolute);
                set_immediate_value(&current_word_value,parsedDataGetSourceValue(a_parsed_data));
                break;
            default:
                /* data is handled by the default value  0x00. */
                break;

        }
        blockInsert(a_CI,current_word_value);
        current_word_value = 0x00;
        switch (dest_addressing_type) {
            case addr_struct:
                /* The first slot reserved to the address which is not yet available.*/
                blockInsert(a_CI,0x00);
                set_struct_field_value(&current_word_value, parsedDataGetDestValue(a_parsed_data));
                break;
            case addr_register:
                set_encoding_value(&current_word_value,enc_absolute);
                set_dest_register_value(&current_word_value,parsedDataGetDestValue(a_parsed_data));
                break;
            case addr_immed:
                set_encoding_value(&current_word_value,enc_absolute);
                set_immediate_value(&current_word_value,parsedDataGetDestValue(a_parsed_data));
                break;
                default:
                /* data is handled by the default value  0x00. */
                break;
        }
        blockInsert(a_CI,current_word_value);
    }

}

static void process_instruction_group_two(ParsedData a_parsed_data, Block a_CI){
    short               current_word_value  = 0x00;

    switch (parsedDataGetDestAddressing(a_parsed_data)) {
        case addr_struct:
            /* The first slot reserved to the address which is not yet available.*/
            blockInsert(a_CI,0x00);
            set_struct_field_value(&current_word_value, parsedDataGetDestValue(a_parsed_data));
            break;
        case addr_register:
            set_encoding_value(&current_word_value,enc_absolute);
            set_dest_register_value(&current_word_value,parsedDataGetDestValue(a_parsed_data));
            break;
        case addr_immed:
            set_encoding_value(&current_word_value,enc_absolute);
            set_immediate_value(&current_word_value,parsedDataGetDestValue(a_parsed_data));
            break;
        default:
            break;
    }
    blockInsert(a_CI,current_word_value);
}

static void process_string(ParsedData a_parsed_data, Block a_DI){
    const char        *string = NULL;

    string = parsedDataGetString(a_parsed_data);
    while(*string != 0){
        blockInsert(a_DI,*string++);
    }
    blockInsert(a_DI,*string);
}

static void process_struct(ParsedData a_parsed_data, Block a_DI){
    const char          *string = NULL;

    blockInsert(a_DI,parsedDataGetStructInteger(a_parsed_data));
    string = parsedDataGetStructString(a_parsed_data);
    /* Run through string  */
    while(*string != 0){
        /* Insert current character. */
        blockInsert(a_DI,*string++);
    }
    /* insert 0 as end of word. */
    blockInsert(a_DI,*string);
}

static void process_data(ParsedData a_parsed_data, Block a_DI){
    const short     *data_array = NULL;
    size_t          size        = 0,
                    i           = 0;

    data_array = parsedDataGetDataArray(a_parsed_data);
    size = parsedDataGetDataArraySize(a_parsed_data);

    for(i = 0; i < size ; ++i){
        blockInsert(a_DI,data_array[i]);
    }
}

static void process_extern(ParsedData a_parsed_data, SymbolTable a_table){
    Symbol          symbol  = NULL;

    symbol = symbolCreate(parsedDataGetExternLabel(a_parsed_data),st_extern,0x00);
    symbolTableInsert(a_table,symbol);
}


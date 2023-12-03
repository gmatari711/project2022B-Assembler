#include "second_pass.h"
#include "parse_data.h"
#include "misc.h"
#include "entry.h"
#include "extern.h"
#include "instruction_encoding.h"
#include "file_builder.h"

/**
 *
 * @param a_label
 * @param a_symbol_table
 * @param a_ext
 * @param a_queue
 * @param a_line
 */
static void process_entry(const char *a_label, SymbolTable a_table, List a_ent, ErrQueue a_queue, int a_line);
/**
 *
 * @param a_parsed_data
 * @param a_CI
 * @param a_table
 * @param a_ext
 * @param a_queue
 * @param a_CI_counter
 * @param a_line
 */
static void process_instruction(ParsedData a_parsed_data,Block a_CI, SymbolTable a_table,List a_ext, ErrQueue a_queue,int *a_CI_counter, int a_line);
/**
 *
 * @param a_parsed_data
 * @param a_CI
 * @param a_table
 * @param a_ext
 * @param a_queue
 * @param a_CI_counter
 * @param a_line
 */
static void process_instruction_group_one(ParsedData a_parsed_data,Block a_CI, SymbolTable a_table, List a_ext, ErrQueue a_queue,int *a_CI_counter, int a_line);
/**
 *
 * @param a_parsed_data
 * @param a_CI
 * @param a_table
 * @param a_ext
 * @param a_queue
 * @param a_CI_counter
 * @param a_line
 */
static void process_instruction_group_two(ParsedData a_parsed_data,Block a_CI, SymbolTable a_table, List a_ext, ErrQueue a_queue,int *a_CI_counter, int a_line);
/**
 *
 * @param a_symbol
 * @param a_CI
 * @param a_ext
 * @param a_CI_counter
 */
static void process_extern(Symbol a_symbol, Block a_CI,List a_ext, int a_CI_counter);
/**
 *
 * @param a_CI
 * @param a_CI_counter
 */
static void process_relocatable(Symbol symbol, Block a_CI, int a_CI_counter);
/**
 *
 * @param a_symbol
 * @param a_CI
 * @param a_ext
 * @param a_queue
 * @param a_CI_counter
 * @param a_line
 */
static void process_struct_and_direct(Symbol a_symbol, Block a_CI, List a_ext,int a_CI_counter);

void second_pass(const char *a_filename, Block a_CI,SymbolTable a_table, ErrQueue a_queue, List a_ext, List a_ent){
    ParsedData          parsed_data                     = NULL;
    int                 CI_counter                      = 0,
                        line_counter                    = 1;
    FILE                *current_file                   = NULL;
    char                current_line[MAX_LINE_LENGTH+1] = {0};

    current_file = open_pre_assembler_file(a_filename,"r");
    assert(NULL != current_file);

    /* Given max length of a line to be 80 characters. */
    while (fgets(current_line,MAX_LINE_LENGTH,current_file)){

        /* Parse and extract information relevant for processing. */
        parsed_data = process_command(current_line);

        if(parsedDataIsDirective(parsed_data) == True){
            if(parsedDataGetDirective(parsed_data) == dir_entry){
                process_entry(parsedDataGetEntryLabel(parsed_data),a_table,a_ent,a_queue,line_counter);
            }
        }
        else if(parsedDataIsInstruction(parsed_data) == True){
            process_instruction(parsed_data,a_CI,a_table,a_ext,a_queue,&CI_counter,line_counter);
        }
        ++line_counter;
    }
    fclose(current_file);

}

static void process_entry(const char *a_label, SymbolTable a_table, List a_ent, ErrQueue a_queue, int a_line){
    EntryInfo           entry_info  = NULL;
    Symbol              symbol      = NULL;

    symbol = symbolTableContains(a_table,a_label);
    if(NULL != symbol){
        if(NULL == listSearch(a_ent,a_label,(object_key_cmp)entryInfoKeyCompare)){
            entry_info = entryInfoCreate(a_label, symbolGetAddress(symbol));
            listInsert(a_ent,entry_info);
        }
    }
    else {
        errQueueEnqueue(a_queue,a_line,err_symbol_not_found);
    }
}

static void process_instruction(ParsedData a_parsed_data,Block a_CI, SymbolTable a_table,List a_ext, ErrQueue a_queue,int *a_CI_counter, int a_line){
    if(parsedDataGetInstructionGroup(a_parsed_data) == inst_group_1){
        process_instruction_group_one(a_parsed_data,a_CI,a_table,a_ext,a_queue,a_CI_counter,a_line);
    }
    else if(parsedDataGetInstructionGroup(a_parsed_data) == inst_group_2){
        process_instruction_group_two(a_parsed_data,a_CI,a_table,a_ext,a_queue,a_CI_counter,a_line);
    }
    else {
        /* Group 3 instruction only has one word which is the instruction word. */
        ++*a_CI_counter;
    }
}

static void process_instruction_group_one(ParsedData a_parsed_data,Block a_CI, SymbolTable a_table, List a_ext, ErrQueue a_queue,int *a_CI_counter, int a_line){
    AddrType            source  = addr_none,
                        dest    = addr_none;
    Symbol              symbol  = NULL;

    source = parsedDataGetSourceAddressing(a_parsed_data);
    dest = parsedDataGetDestAddressing(a_parsed_data);
    ++*a_CI_counter;

    if(source == addr_direct || source == addr_struct){
        symbol = symbolTableContains(a_table,parsedDataGetSourceLabel(a_parsed_data));
        if(NULL == symbol){
            errQueueEnqueue(a_queue,a_line,err_symbol_not_found);
        }
        else{
            process_struct_and_direct(symbol,a_CI,a_ext,*a_CI_counter);
        }
        /* Struct type operands occupy two words : address and field. */
        *a_CI_counter += (source == addr_struct)? 2 : 1;
    }
    else if(source != addr_register || dest != addr_register){
        /* Special case: when both operands are registers they share a single word. */
        ++*a_CI_counter;
    }
    if(dest == addr_direct || dest == addr_struct){
        symbol = symbolTableContains(a_table, parsedDataGetDestLabel(a_parsed_data));
        if(NULL == symbol){
            errQueueEnqueue(a_queue,a_line,err_symbol_not_found);
        }
        else{
            process_struct_and_direct(symbol,a_CI,a_ext,*a_CI_counter);
        }
        /* Struct type operands occupy two words : address and field. */
        *a_CI_counter += (dest == addr_struct)? 2 : 1;
    }
    else {
        ++*a_CI_counter;
    }
}

static void process_extern(Symbol a_symbol, Block a_CI,List a_ext, int a_CI_counter){
    ExternInfo          extern_info     = NULL;
    short               current_word    = 0x00;

    set_encoding_value(&current_word,enc_external);
    wordSetValue(blockGetWord(a_CI, a_CI_counter),current_word);
    /* Search for an object with a matching key*/
    extern_info = listSearch(a_ext,symbolGetLabel(a_symbol),(object_key_cmp )externInfoKeyCompare);
    if(NULL == extern_info){
        /* Create a new Extern object and push it to list. */
        extern_info = externInfoCreate(symbolGetLabel(a_symbol));
        listInsert(a_ext,extern_info);
    }
    externInfoInsert(extern_info,(short)a_CI_counter);
}

static void process_relocatable(Symbol symbol, Block a_CI, int a_CI_counter){
    short               current_word    = 0x00;

    set_encoding_value(&current_word,enc_relocate);
    set_address_value(&current_word,symbolGetAddress(symbol));
    wordSetValue(blockGetWord(a_CI, a_CI_counter),current_word);
}

static void process_struct_and_direct(Symbol a_symbol, Block a_CI, List a_ext,int a_CI_counter){

    if(symbolGetType(a_symbol) == st_extern){
        process_extern(a_symbol,a_CI,a_ext,a_CI_counter);
    }
    else {
        process_relocatable(a_symbol,a_CI,a_CI_counter);
    }
}

static void process_instruction_group_two(ParsedData a_parsed_data,Block a_CI, SymbolTable a_table, List a_ext, ErrQueue a_queue,int *a_CI_counter, int a_line){
    AddrType            dest    = addr_none;
    Symbol              symbol  = NULL;

    dest = parsedDataGetDestAddressing(a_parsed_data);
    ++*a_CI_counter;

    if(dest == addr_direct || dest == addr_struct){
        symbol = symbolTableContains(a_table,parsedDataGetDestLabel(a_parsed_data));
        if(NULL == symbol){
            errQueueEnqueue(a_queue,a_line,err_symbol_not_found);
        }
        else{
            process_struct_and_direct(symbol,a_CI,a_ext,*a_CI_counter);
        }
    }
    /* Struct type operands occupy two words : address and field. */
    *a_CI_counter += (dest == addr_struct)? 2 : 1;

}
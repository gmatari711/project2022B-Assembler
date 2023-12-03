#include "parse_data.h"

#include <ctype.h>

#define COMMA_STR ","
#define SPACE_STR " "


#define IMMED_PREFIX '#'
#define REGISTER_PREFIX 'r'
#define COMMENT_DELIM ";"

#define HI_REG 7
#define LO_REG 0
#define MAX_LABEL_SIZE 30
#define GROUP_ONE_OPERANDS 2
#define GROUP_TWO_OPERANDS 1
#define GROUP_THREE_OPERANDS 0

/** Addressing types masks */
#define IMMEDIATE       0x0001
#define DIRECT          0x0002
#define STRUCT          0x0004
#define REGISTER        0x0008
#define NO_OPERAND      0x0000



#define DIR_DATA    ".data"
#define DIR_STRUCT  ".struct"
#define DIR_STRING  ".string"
#define DIR_EXTERN  ".extern"
#define DIR_ENTRY   ".entry"

#define INST_MOV    "mov"
#define INST_CMP    "cmp"
#define INST_ADD    "add"
#define INST_SUB    "sub"
#define INST_NOT    "not"
#define INST_CLR    "clr"
#define INST_LEA    "lea"
#define INST_INC    "inc"
#define INST_DEC    "dec"
#define INST_JMP    "jmp"
#define INST_BNE    "bne"
#define INST_GET    "get"
#define INST_PRN    "prn"
#define INST_JSR    "jsr"
#define INST_RTS    "rts"
#define INST_HLT    "hlt"

#define REG_0 "r0"
#define REG_1 "r1"
#define REG_2 "r2"
#define REG_3 "r3"
#define REG_4 "r4"
#define REG_5 "r5"
#define REG_6 "r6"
#define REG_7 "r7"



#define is_valid_reg_val(X) ((X)>=LO_REG && (X)<=HI_REG)?True : False

typedef struct args{
    Directive           m_dir;
    AssemblerInst       m_inst;
    InstructionGroup    m_group;

    AddrType            m_operands_addr[2];
    short               m_operands_val[2];/** May be passed to m_data(?)*/

    char                m_operands_label[2][31]; /** May be redundant(?) Could be set to string array(?) */


    short               m_data[80];
    size_t              m_data_size;


    char                m_string[81];

    char                m_label[31];
    Boolean             m_label_flag;

    Status              m_err;
}Args;

static void ignore_comments(char *str);

static Status get_struct_value(char *a_str, char *a_struct_label, short *a_field_val);

static Boolean is_reserved_word(const char *a_str);
static Status get_register(const char *a_op, short *a_register);
static Boolean get_immediate(const char *a_op, short *a_immed);

static Boolean is_struct(char *a_op);
static AddrType get_operand_addressing_type(char *a_operand);
static Status parse_instruction_syntax(char *a_str_arr[] , Args *a_args, size_t a_num_of_operands);


/**Params:  A string constant 'a_str' which expected to be a token holding a directive of some sort.
 * Task:    Compares string content with all directive names and if one to match it's corresponding enum is set to result.
 * Output:  Returns a relevant Directive enum value if found a match. Otherwise dir_none is returned.
 * */
static Directive get_directive(const char *a_str);

/**Params:  An array of strings 'a_str_arr' which supposed to hold a valid string and a pointer to Args struct.
 * Task:    Verify that first string on the array start with '\"'(opening quotes), then copy in blocks all strings on
 *          the array separated by space.
 *          After all strings copied - make sure that the last string ended with '\"' (closing quotes).
 *          The string is set to a_args string field and size is set to string size field.
 * Output:  returns err_none if a valid string was copied, relevant error massage otherwise.
 * */
static Status process_string(char *a_str_arr[], Args *a_args);

/**Params:
 * Task:
 * Output:
 * */
static Status process_data(char *a_str_arr[], Args *a_args);

/**Params:
 * Task:
 * Output:
 * */
static Status process_struct(char *a_str_arr[], Args *a_args);

/**Params:
 * Task:
 * Output:
 * */
static Status process_extern(char *a_str_arr[], Args *a_args);

/**Params:
 * Task:
 * Output:
 * */
static Status process_entry(char *a_str_arr[], Args *a_args);


/**Params:
 * Task:
 * Output:
 * */
static Status process_directive(char *a_str_arr[], Args *a_args);

/**Params:
 * Task:
 * Output:
 * */
InstructionGroup get_inst_group(AssemblerInst inst);

/**Params:
 * Task:
 * Output:
 * */
static Status process_instruction(char *a_str_arr[], Args *a_args);


/**Params:  A string 'a_str'.
 * Task:    Test if 'a_str' represent a label.
 * Output:  'True' if 'a_str' represent a label, 'False' otherwise.
 * */
static Boolean is_label(const char *a_str);

/**Params:  An Args object.
 * Task:    Zero all Args values.
 * Output:  Return Args' object address.
 * */
static Args *init_args(Args *a_args);

/**Params:
 * Task:
 * Output:
 * */
static Boolean is_valid_label(const char *a_label);

/**Params:  A string 'a_src' and a string array 'a_dest'.
 * Task:    Break a_src into tokens delimited by spaces, and place the i'th token to a_dest[i].
 * Output:  Void.
 * */
static void break_to_substrings(String a_src, String *a_dest);

/**Params:  A string 'a_src' and a string 'a_dest'.
 * Task:    Copy a_src to a_dest, and implant space before and after each comma.
 * Output:  Void.
 * */
static void insert_spaces(const char *a_src, char *a_dest);

/**Params:  A string constant 'a_str' which expected to be a token.
 * Task:    Compares a_str text with all instruction names and se the relevant enum value to result.
 * Output:  If found, returns the relevant enum value that represents the given instruction. otherwise returns 'inst_none'.
 * */
static AssemblerInst get_instruction(const char *a_str);



/**Params:
 * Task:
 * Output:
 * */
static Status exrtract_integer(const char *a_string, short *a_res);

/**Params:
 * Task:
 * Output:
 * */
static Boolean is_immediate(const char *a_op);

/**Params:
 * Task:
 * Output:
 * */
static Boolean is_register(const char *a_op);


static Args *init_args(Args *a_args){
    memset(a_args, 0 ,sizeof(Args));
    a_args->m_inst = inst_none;
    a_args->m_dir = dir_none;
    return a_args;
}


static void insert_spaces(const char *a_src, char *a_dest){
    size_t              i   = 0,
    j   = 0;

    for(i = 0; i <= strlen(a_src); ++i){
        switch (a_src[i]){
            case ',':
                a_dest[j++] = ' ';
                a_dest[j++] = ',';
                a_dest[j++] = ' ';
                break;
                default:
                    a_dest[j++] = a_src[i];
                    break;
        }
    }
}


static void break_to_substrings(String a_src, String *a_dest){
    size_t              i       = 0;
    String              delim   = " \t\n\r";

    a_dest[i] = strtok(a_src,delim);

    while(a_dest[i] != NULL){
        a_dest[++i] = strtok(NULL, delim);
    }
}



static Boolean is_valid_label(const char *a_label){
    Boolean result              = False;

    if(NULL != a_label && strlen(a_label)<=MAX_LABEL_SIZE && is_reserved_word(a_label) == False){
        result = True;
        if(isalpha(*a_label)){
            ++a_label;
            while(*a_label != 0 && result == True){
                if(isalnum(*a_label) == 0){
                    result = False;
                }
                ++a_label;
            }
        }
        else {
            result = False;
        }
    }
    return result;
}


static Boolean is_label(const char *a_str){/*MAYBE SET OUT TO MACRO*/
    return (a_str[strlen(a_str)-1] == ':')?True : False;
}

/**Params:
 * Task:
 * Output:
 * */
static Boolean is_directive(const char *a_str){ /*MAYBE SET OUT TO MACRO*/
    return *a_str == '.';
}

static Status verify_instruction_addressing(Args *a_args);


static Directive get_directive(const char *a_str){
    char               *directive_list[] = {
            DIR_DATA, DIR_STRUCT,
            DIR_STRING, DIR_EXTERN, DIR_ENTRY
    };

    Directive           result           = dir_none;

    size_t              i;

    for(i = 0 ; i < 5 && result == dir_none ; ++i){ /** Change with a constant.*/
        if(strcmp(a_str,directive_list[i]) == 0){
            result = (Directive)i;
        }
    }

    return result;

}


static AssemblerInst get_instruction(const char *a_str){
    char               *instruction_list[]    = {
            INST_MOV, INST_CMP, INST_ADD,
            INST_SUB, INST_NOT, INST_CLR,
            INST_LEA, INST_INC, INST_DEC,
            INST_JMP, INST_BNE, INST_GET,
            INST_PRN, INST_JSR, INST_RTS,
            INST_HLT
    };
    AssemblerInst       result                = inst_none;

    size_t              i                     = 0;

    for(i = 0; i < 16 && result == inst_none ; ++i){
        if(strcmp(instruction_list[i],a_str) == 0){
            result = (AssemblerInst)i;
        }
    }

    return result;
}


static Status process_string(char *a_str_arr[], Args *a_args){
    size_t              i           = 0,
    size        = 0,
    str_index   = 0;
    Status              result      = err_none;

    if(NULL == *a_str_arr){
        return err_missing_string;
    }
    if(*a_str_arr[i]++ != '\"'){
        result = err_illegal_string;
    }
    for(i = 0; a_str_arr[i] && result == err_none ; ++i){
        if(i != 0){
            a_args->m_string[str_index++] = ' ';
        }

        size = strlen(a_str_arr[i]);
        memcpy(a_args->m_string + str_index,a_str_arr[i],size);
        str_index+=size;
    }
    if('\"' != a_args->m_string[str_index -1]){
        result = err_illegal_string;
    }
    else {
        a_args->m_string[str_index-1] = 0;
    }

    return result;

}


static Status process_data(char *a_str_arr[], Args *a_args){
    size_t              i       = 0;
    Status              result  = err_none;
    short               current = 0;

    while(NULL != a_str_arr[i] && err_none == result){

        if( i % 2 != 0){/** Comma turn */
            if(strcmp(a_str_arr[i],COMMA_STR) != 0){
                result = err_missing_comma;
            }
        }/** Arg turn */
        else if(*a_str_arr[i] == ','){ /** Comma instead of argument. */
            result = (i == 0)?/** Is first arg? */
                    err_illegal_comma:err_consecutive_comma;
        }
        else {/** Argument found - test validity. */
            result = exrtract_integer(a_str_arr[i],&current);
            if(result == err_none){/** Insert value to data array. */
                a_args->m_data[a_args->m_data_size++] = current;
            }
        }
        ++i;
    }
    if(i == 0){
        result = err_missing_data;
    }

    return result;
}
/** Expect a trimmed token. */
static Status exrtract_integer(const char *a_string, short *a_res){
    Status              result  = err_none;
    short              current = 0;
    char               *ptr     = NULL;


    current = strtol(a_string, &ptr, 10);

    if(*ptr != 0){
        result = err_not_integer;
    }
    *a_res = current;

    return result;
}

static Status process_struct(char *a_str_arr[], Args *a_args){
    Status              result  = err_none;
    short               current = 0;

    if(NULL == *a_str_arr){
        return err_missing_struct;
    }
    if(strcmp(*a_str_arr,COMMA_STR) == 0){
        result = err_illegal_comma;
    }
    if(err_none == result){
        result = exrtract_integer(*a_str_arr++, &current);
        if(result == err_none){/* Insert value to data array. */
            a_args->m_data[a_args->m_data_size++] = current;
            if(NULL == *a_str_arr){
                return err_struct_missing_string;
            }
            if(**a_str_arr++ != ','){
                result = err_missing_comma;
            }
            if(result == err_none){
                if(NULL == *a_str_arr){
                    return err_struct_missing_string;
                }
                if(**a_str_arr == ','){
                    result = err_consecutive_comma;
                }
                else {
                    result = process_string(a_str_arr,a_args);
                }
            }

        }
    }

    return result;
}


static Status process_extern(char *a_str_arr[], Args *a_args){
    Status              result  = err_none;

    if(is_valid_label(*a_str_arr) == True){
        strcpy(a_args->m_string,*a_str_arr);
        a_args->m_dir = dir_extern;
    }
    else {
        result = err_illegal_label;
    }
    return result;
}



static Status process_entry(char *a_str_arr[], Args *a_args){
    Status              result  = err_none;

    if(is_valid_label(*a_str_arr) == True){
        strcpy(a_args->m_string,*a_str_arr);
        a_args->m_dir = dir_entry;
    }
    else {
        result = err_illegal_label;
    }
    return result;
}

static Status process_directive(char *a_str_arr[], Args *a_args){
    size_t                  i       = 0;
    Status                  result  = err_none;

    a_args->m_dir = get_directive(a_str_arr[i]);
    ++i;

    switch (a_args->m_dir) {
        case dir_data:
            result = process_data(&a_str_arr[i],a_args);
            break;
        case dir_string:
            result = process_string(&a_str_arr[i],a_args);
            break;
        case dir_struct:
            result = process_struct(&a_str_arr[i],a_args);
            break;
        case dir_extern:
            result = process_extern(&a_str_arr[i],a_args);
            break;
        case dir_entry:
            result = process_entry(&a_str_arr[i],a_args);
            break;
        default:
            result = err_illegal_directive;
            break;
    }

    return result;
}

static Boolean is_reserved_word(const char *a_str){
    static char *dictionary[] = {
            INST_MOV,INST_CMP,INST_ADD,INST_SUB,INST_NOT,
            INST_CLR,INST_LEA,INST_INC,INST_DEC,INST_JMP,
            INST_BNE,INST_GET,INST_PRN,INST_JSR,INST_RTS,INST_HLT,
            REG_0,REG_1,REG_2,REG_3,REG_4,REG_5,REG_6,REG_7,NULL
    };
    Boolean             result  = False;
    size_t              index   = 0;

    while(NULL != dictionary[index] && result == False){
        if(strcmp(dictionary[index++],a_str) == 0){
            result = True;
        }
    }
    return result;
}

InstructionGroup get_inst_group(AssemblerInst a_inst){
    InstructionGroup    result  = inst_group_1;

    switch (a_inst) {
        /** All matching values of group 1*/
        case inst_mov:
        case inst_cmp:
        case inst_add:
        case inst_sub:
        case inst_lea:
            result = inst_group_1;
            break;
                            /** All matching values of group 2*/
        case inst_not:
        case inst_clr:
        case inst_inc:
        case inst_dec:
        case inst_jmp:
        case inst_bne:
        case inst_get:
        case inst_prn:
        case inst_jsr:
            result = inst_group_2;
            break;
                                                                /** All matching values of group */
        case inst_rts:
        case inst_hlt:
            result = inst_group_3;
            break;
        default:
            break;
    }
    return result;
}

static Boolean is_struct(char *a_op){
    Boolean             result      = False;
    char                buff[31]    = {0},
                        *ptr        = NULL;
    size_t              index       = 0;

    while(*a_op !='.' && *a_op != 0){
        buff[index++] = *a_op++;
    }
    buff[index] = 0;
    if(*a_op != 0 && is_valid_label(buff) == True){
        ++a_op;
        /* Test for a number but ignore the result.*/
        strtol(a_op, &ptr, 10);
        /* If ptr points to anything but end of string - it is not an integer. */
        if(*ptr == 0){
            result = True;
        }
    }
    return result;
}



static AddrType get_operand_addressing_type(char *a_operand){
    AddrType            result  = addr_none;

    if(is_immediate(a_operand) == True){
        result = addr_immed;
    }
    else if(is_struct(a_operand) == True){
        result = addr_struct;
    }
    else if(is_valid_label(a_operand) == True){
        result = addr_direct;
    }
    else if(is_register(a_operand) == True){
        result = addr_register;
    }

    return result;
}

static Status get_operand_value(const char *a_src ,AddrType a_addr ,short *a_res){
    Status              result  = err_none;

    if(a_addr == addr_register){
        result = get_register(a_src,a_res);
    }
    else {
        result = get_immediate(a_src,a_res)?err_none : err_not_integer;
    }

    return result;
}

static Status parse_instruction_syntax(char *a_str_arr[] , Args *a_args, size_t a_num_of_operands){
    Status              result      = err_none;
    size_t              index       = 0,
    total_op    = 0,
    addr_idx    = 0;

    AddrType            current     = 0;
    while(total_op < a_num_of_operands && a_str_arr[index] && result == err_none){
        if(index % 2 == 0){
            if(*a_str_arr[index] == ','){
                result = err_illegal_comma;
            }
            else{
                current = get_operand_addressing_type(a_str_arr[index]);
                if(current == addr_none){
                    result = err_illegal_operand;
                }
                else{
                    a_args->m_operands_addr[addr_idx] = current;
                    if(current == addr_immed || current == addr_register){
                        result = get_operand_value(a_str_arr[index], current, &a_args->m_operands_val[addr_idx++]);
                    }
                    else if(current == addr_struct){
                        result = get_struct_value(a_str_arr[index],a_args->m_operands_label[addr_idx],&a_args->m_operands_val[addr_idx]);
                        ++addr_idx;
                    }
                    else{
                        strcpy(a_args->m_operands_label[addr_idx++], a_str_arr[index]);
                    }
                }
                ++total_op;
            }
        }
        else if(*a_str_arr[index] != ','){
            result = err_missing_comma;
        }
        ++index;
    }


    if(result == err_none && total_op == a_num_of_operands){
        if(a_str_arr[index] != NULL){
            result = err_extraneous_text;
        }
    }
    if(result == err_none && total_op < a_num_of_operands){
        result = err_missing_operand;
    }

    return result;
}

static Status get_struct_value(char *a_str, char *a_struct_label, short *a_field_val){
    char            *label  = NULL,
    *field  = NULL,
    *ptr    = NULL,
    *delim  = ".";
    long            digit   = 0;

    Status          result  = err_none;


    label = strtok(a_str,delim);
    field = strtok(NULL, delim);

    strcpy(a_struct_label,label);
    digit = strtol(field,&ptr,10);

    if(digit < 1 || digit > 2){
        result = err_illegal_struct_field;
    }
    else{
        *a_field_val = (short)digit;
    }

    return result;
}

static Status process_instruction(char *a_str_arr[], Args *a_args){
    Status              result  = err_none;

    a_args->m_inst = get_instruction(*a_str_arr);
    if(a_args->m_inst != inst_none){
        a_args->m_group = get_inst_group(a_args->m_inst);


        switch (a_args->m_group) {
            case inst_group_1:
                /*Handle group one type function*/
                result = parse_instruction_syntax(a_str_arr + 1, a_args, GROUP_ONE_OPERANDS);
                break;
            case inst_group_2:
                /*Handle group two type function*/
                result = parse_instruction_syntax(a_str_arr + 1, a_args, GROUP_TWO_OPERANDS);
                break;
            case inst_group_3:
                /*Handle group three type function*/
                result = parse_instruction_syntax(a_str_arr + 1, a_args, GROUP_THREE_OPERANDS);
                break;
            default:
                break;
        }
    }
    else {
        result = err_illegal_instruction;
    }

    return result;

}




Args *process_command(const char *a_str){
    static Args         args;
    char                buffer[241] = {0},
    *str_arr[81] = {NULL};
    Status              result      = err_none;
    size_t              i           = 0;

    insert_spaces(a_str, buffer);                       /* Add spaces before and after each comma. */
    ignore_comments(buffer);
    break_to_substrings(buffer,str_arr);                /* Break assembly line to substrings. */
    init_args(&args);

    if(NULL != *str_arr){
        if(is_label(str_arr[i]) == True){
            if(NULL != str_arr[i+1]){
                str_arr[i][strlen(str_arr[i])-1] = 0;           /** Lose the NO LONGER necessary colon.        */
                if(is_valid_label(str_arr[i]) == True){         /** Label was found. */
                    strcpy(args.m_label, str_arr[i++]);
                    args.m_label_flag = True;                /** May be redundant (?) */
                }
                else {
                    result = err_illegal_label;
                }
            }
            else result = err_empty_label;
        }
        if(result == err_none && is_directive(str_arr[i]) == True){
            result  = process_directive(str_arr + i ,&args);
        }
        else if(result == err_none){
            result = process_instruction(str_arr + i ,&args);
            if(err_none == result){
                result = verify_instruction_addressing(&args);
            }
        }
    }
    args.m_err = result;

    return &args;
}


static byte_t get_addressing_specifier(AddrType a_addr){
    byte_t              result  = NO_OPERAND;

    switch (a_addr) {
        case addr_immed:
            result = IMMEDIATE;
            break;
        case addr_direct:
            result = DIRECT;
            break;
        case addr_struct:
            result = STRUCT;
            break;
        case addr_register:
            result = REGISTER;
            break;
        default:
            break;
    }

    return result;
}


static Status verify_instruction_addressing(Args *a_args){
    Status              result  = err_none;
    byte_t              src_res = DIRECT,
    dest_res= DIRECT;

    static struct{
        byte_t          m_op1_addr,
        m_op2_addr;
    }                   addr_list[] = {
            {IMMEDIATE | DIRECT | STRUCT | REGISTER, DIRECT | STRUCT | REGISTER} ,
            {IMMEDIATE | DIRECT | STRUCT | REGISTER,IMMEDIATE | DIRECT | STRUCT | REGISTER},
            {IMMEDIATE | DIRECT | STRUCT | REGISTER,DIRECT | STRUCT | REGISTER},
            {IMMEDIATE | DIRECT | STRUCT | REGISTER, DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {DIRECT | STRUCT,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,IMMEDIATE | DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,DIRECT | STRUCT | REGISTER},
            {NO_OPERAND,NO_OPERAND}
    };


    if(inst_group_1 == a_args->m_group){
        src_res = get_addressing_specifier(a_args->m_operands_addr[0]) & addr_list[a_args->m_inst].m_op1_addr;
        dest_res = get_addressing_specifier(a_args->m_operands_addr[1]) & addr_list[a_args->m_inst].m_op2_addr;
    }
    else if(inst_group_2 == a_args->m_group){
        src_res = get_addressing_specifier(a_args->m_operands_addr[0]) & addr_list[a_args->m_inst].m_op2_addr;
    }

    if(src_res == 0 || dest_res == 0){
        result = err_illegal_addr_type;
    }
    return result;
}


static Boolean is_immediate(const char *a_op){
    return (*a_op == IMMED_PREFIX)?True:False;
}


static Boolean get_immediate(const char *a_op, short *a_immed){
    return (exrtract_integer(a_op + 1,a_immed) == err_none)? True : False;
}

static Boolean is_register(const char *a_op){
    return (*a_op == REGISTER_PREFIX)?True:False;
}

static Status get_register(const char *a_op, short *a_register){
    Status                  result  = err_none;
    long                    reg_val = 0;
    char                   *ptr     = NULL;


    reg_val = strtol(a_op + 1, &ptr, 10);

    if(*ptr != 0){
        result = err_illegal_operand;
    }
    else if(is_valid_reg_val(reg_val) == True){
        *a_register = (short)reg_val;
    }
    else {
        result = err_invalid_register;
    }

    return result;
}


Boolean parsedDataIsDirective(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return (a_parsed_data->m_dir != dir_none)? True : False;
}


Boolean parsedDataIsInstruction(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return (a_parsed_data->m_inst != inst_none)? True : False;
}

Boolean parsedDataHasLabel(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return a_parsed_data->m_label_flag;
}

/**
 * Tests if the ParseData object contains an information of an error.
 * @param a_parsed_data A ParsedData object.
 * @return True if a_parsed_data holds information of an error. False otherwise.
 */
Boolean parsedDataErrFound(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return (a_parsed_data->m_err != err_none)? True : False;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
Directive parsedDataGetDirective(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return a_parsed_data->m_dir;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
AssemblerInst parsedDataGetInstruction(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return a_parsed_data->m_inst;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
InstructionGroup parsedDataGetInstructionGroup(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return a_parsed_data->m_group;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
AddrType parsedDataGetDestAddressing(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    /* On group 1 destination operator comes second when on group 2 it comes first. */
    return (a_parsed_data->m_group == inst_group_1)?
                a_parsed_data->m_operands_addr[1] : a_parsed_data->m_operands_addr[0];
}
/**
 *
 * @param a_parsed_data
 * @return
 */
AddrType parsedDataGetSourceAddressing(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    /* A source operand is only relevant when handling group 1 instructions. */
    return (a_parsed_data->m_group == inst_group_1)?
                a_parsed_data->m_operands_addr[0] : addr_immed;
}

/**
 *
 * @param a_parsed_data
 * @return
 */
Status parsedDataGetErrNumber(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    return a_parsed_data->m_err;
}

/**
 *
 * @param a_parsed_data
 * @return
 */
short parsedDataGetDestValue(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);

    if (a_parsed_data->m_group == inst_group_1) {
        return a_parsed_data->m_operands_val[1];
    } else {
        return a_parsed_data->m_operands_val[0];
    }
}
/**
 *
 * @param a_parsed_data
 * @return
 */
short parsedDataGetSourceValue(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);

    if (a_parsed_data->m_group == inst_group_1) {
        return a_parsed_data->m_operands_val[0];
    } else {
        return a_parsed_data->m_operands_val[1];
    }
}
/**
 *
 * @param a_parsed_data
 * @return
 */
short parsedDataGetStructInteger(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    assert(a_parsed_data->m_dir == dir_struct);

    return a_parsed_data->m_data[0];
}
/**
 *
 * @param a_parsed_data
 * @return
 */
const char *parsedDataGetStructString(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    assert(a_parsed_data->m_dir == dir_struct);

    return a_parsed_data->m_string;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
const short *parsedDataGetDataArray(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    assert(a_parsed_data->m_dir == dir_data);


    return a_parsed_data->m_data;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
size_t  parsedDataGetDataArraySize(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);

    return a_parsed_data->m_data_size;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
const char *parsedDataGetSourceLabel(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);

    return (a_parsed_data->m_group == inst_group_1)?
                    a_parsed_data->m_operands_label[0] : a_parsed_data->m_operands_label[1];
}
/**
 *
 * @param a_parsed_data
 * @return
 */
const char *parsedDataGetDestLabel(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);

    return (a_parsed_data->m_group == inst_group_1)?
    a_parsed_data->m_operands_label[1] : a_parsed_data->m_operands_label[0];
}
/**
 *
 * @param a_parsed_data
 * @return
 */
const char *parsedDataGetString(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    assert(a_parsed_data->m_dir == dir_string);

    return a_parsed_data->m_string;

}
/**
 *
 * @param a_parsed_data
 * @return
 */
const char *parsedDataGetLabel(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    assert(True == a_parsed_data->m_label_flag);

    return a_parsed_data->m_label;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
const char *parsedDataGetExternLabel(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    assert(a_parsed_data->m_dir == dir_extern);

    return a_parsed_data->m_string;
}
/**
 *
 * @param a_parsed_data
 * @return
 */
const char *parsedDataGetEntryLabel(struct args *a_parsed_data){
    assert(NULL != a_parsed_data);
    assert(a_parsed_data->m_dir == dir_entry);

    return a_parsed_data->m_string;
}

static void ignore_comments(char *a_str){
    while(*a_str != 0 && *a_str != ';'){
        ++a_str;
    }
    *a_str = 0;
}

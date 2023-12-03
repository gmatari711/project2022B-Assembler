#include "instruction_encoding.h"
#include <assert.h>
struct instruction{
    int         ARE     : 2;
    int         dest    : 2;
    int         source  : 2;
    int         opcode  : 4;
};

struct reg{
    int         ARE     : 2;
    int         dest    : 4;
    int         source  : 4;
};

struct direct{
    int         ARE     : 2;
    int         address : 8;
};

struct immediate{
    int         ARE     : 2;
    int         value   : 8;
};

struct word{
    int         word    : 10;
};

union encoder{
    struct instruction  m_inst;
    struct reg          m_register;
    struct direct       m_direct;
    struct immediate    m_immediate;
    struct word         m_word;
};


void set_encoding_value(short *a_word, int a_are){
    union encoder encoder;
    assert(a_are >= 0 && a_are <= 2);

    encoder.m_word.word = *a_word;
    encoder.m_inst.ARE = a_are;

    *a_word = (short)encoder.m_word.word;
}

void set_opcode_value(short *a_word, int a_opcode){
    union encoder encoder;
    assert(a_opcode >= 0 && a_opcode <= 15);

    encoder.m_word.word = *a_word;
    encoder.m_inst.opcode = a_opcode;
    *a_word = (short)encoder.m_word.word;
}

void set_source_addressing_type(short *a_word, int a_addressing_value){
    union encoder encoder;
    assert(a_addressing_value >=0 && a_addressing_value <=3);

    encoder.m_word.word = *a_word;
    encoder.m_inst.source = a_addressing_value;
    *a_word = (short)encoder.m_word.word;
}

void set_dest_addressing_type(short *a_word, int a_addressing_value){
    union encoder encoder;
    assert(a_addressing_value >=0 && a_addressing_value <=3);

    encoder.m_word.word = *a_word;
    encoder.m_inst.dest = a_addressing_value;
    *a_word = (short)encoder.m_word.word;
}

void set_source_register_value(short *a_word, int a_register_value){
    union encoder encoder;
    assert(a_register_value >=0 && a_register_value <=7);

    encoder.m_word.word = *a_word;
    encoder.m_register.source = a_register_value;
    *a_word = (short)encoder.m_word.word;

}

void set_dest_register_value(short *a_word, int a_register_value){
    union encoder encoder;
    assert(a_register_value >=0 && a_register_value <=7);

    encoder.m_word.word = *a_word;
    encoder.m_register.dest = a_register_value;
    *a_word = (short)encoder.m_word.word;
}

void set_address_value(short *a_word, int a_address_value){
    union encoder encoder;

    encoder.m_word.word = *a_word;
    encoder.m_direct.address = a_address_value;
    *a_word = (short)encoder.m_word.word;
}

void set_struct_field_value(short *a_word, int a_struct_field){
    union encoder encoder;

    encoder.m_word.word = *a_word;
    encoder.m_direct.address = a_struct_field;
    *a_word = (short)encoder.m_word.word;
}


void set_immediate_value(short *a_word, int a_struct_field){
    union encoder encoder;

    encoder.m_word.word = *a_word;
    encoder.m_immediate.value = a_struct_field;
    *a_word = (short)encoder.m_word.word;
}

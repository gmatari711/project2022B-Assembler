#include "error_queue.h"
#include "listiter.h"
#include "list.h"

void show_error(unsigned int a_line_number, unsigned int a_identifier){
    static char         *err_list[] ={
            NULL,
            "Unknown directive name.",
            "Unknown instruction name.",
            "Illegal comma.",
            "Multiple consecutive commas.",
            "Not a valid integer.",
            "Expected comma.",
            "Not a valid string.",
            "Extraneous text.",
            "Illegal operand.",
            "Not a valid label.",
            "One operand or more is missing.",
            "Operand is not of a valid addressing type.",
            "Illegal register.",
            "Multiple symbols with the same label.",
            "Symbol was not found.",
            "Not a valid struct field.",
            "Empty label.",
            "Missing values after data directive declaration.",
            "Missing string after string directive declaration.",
            "Missing values after struct directive declaration.",
            "Struct definition missing a string."
    };
    printf("Line %d: Err%d - %s\n",a_line_number,a_identifier,err_list[a_identifier]);
}



struct err_info{
    unsigned int            m_line_number;
    unsigned int            m_err_identifier;
};


struct err_queue{
    List                m_queue;
};

struct err_queue *errQueueCreate(){
    struct err_queue    *result = NULL;

    result = malloc(sizeof(struct err_queue));
    TEST_NULL(result);
    result->m_queue = listCreate();
    return result;
}


Boolean errQueueIsEmpty(struct err_queue *a_errq){
    TEST_NULL(a_errq);
    return (listIsEmpty(a_errq->m_queue))? True : False;
}


void errQueueEnqueue(struct err_queue *a_errq, size_t a_line_n, size_t a_err_id){
    struct err_info         *new_err    = NULL;
    new_err = malloc(sizeof(struct err_info));
    new_err->m_line_number = a_line_n;
    new_err->m_err_identifier = a_err_id;

    listInsert(a_errq->m_queue,new_err);
}

unsigned int errInfoGetIdentifier(ErrInfo a_err_info){
    return a_err_info->m_err_identifier;
}

unsigned int errInfoGetLineNumber(ErrInfo a_err_info){
    return a_err_info->m_line_number;
}

ErrInfo errQueueDequeue(ErrQueue a_errq){
    ListIterator        iterator    = NULL;
    ErrInfo             result      = NULL;

    if(listIsEmpty(a_errq->m_queue) == False){
        iterator = listIterBegin(a_errq->m_queue);
        result = listIterGetItem(iterator);
        listIterRemove(iterator);
    }
    return result;
}

void errQueueDestroy(ErrQueue *a_errq_ptr){
    assert(NULL != a_errq_ptr);
    listDestroy(&(*a_errq_ptr)->m_queue,(destroy)free);
    free(*a_errq_ptr);
    *a_errq_ptr = NULL;
}
#ifndef ASSEMBLER_ERRQ_H
#define ASSEMBLER_ERRQ_H

#include <stdlib.h>
#include "misc.h"

typedef         struct err_queue        *ErrQueue;
typedef         struct err_info         *ErrInfo;
/**
 * Allocate and initialize an ErrQueue object.
 * @return An initialized ErrQueue object.
 */
ErrQueue errQueueCreate();

/**
 * Deallocate all memory allocated for ErrQueue object pointed by a_errq_ptr. object set to NULL.
 * @param a_errq_ptr A pointer to an ErrQueue object to be destructed.
 */
void errQueueDestroy(ErrQueue *a_errq_ptr);

/**
 * Allocate and insert an new ErrorInfo object to the queue.
 * @param a_errq An ErrQueue object.
 * @param a_line_n An integer number represents the line where error was detected.
 * @param a_err_id An integer number representing error identifier.
 */
void errQueueEnqueue(ErrQueue a_errq, size_t a_line_n, size_t a_err_id);

/**
 * Removes the first ErrInfo object from the queue.
 * @param a_errq An errQueue object.
 * @return The first ErrInfo object.
 */
ErrInfo errQueueDequeue(ErrQueue a_errq);

/**
 * Check if queue is empty.
 * @param a_errq An ErrQueue object.
 * @return True (1) if queue is empty. False (0) otherwise.
 */
Boolean errQueueIsEmpty(ErrQueue a_errq);
/**
 * Getter function for ErrInfo's identifiers value.
 * @param a_err_info An ErrInfo object.
 * @return error identifier value associated with a_errq.
 */
unsigned int errInfoGetIdentifier(ErrInfo a_err_info);
/**
 * Getter function for ErrInfo's line number value.
 * @param a_err_info a_err_info An ErrInfo object.
 * @return return error line number value associated with a_errq.
 */
unsigned int errInfoGetLineNumber(ErrInfo a_err_info);
/**
 * Output function - Prints errors details in a formatted manner.
 * @param a_line_number The line number which error was detected.
 * @param a_identifier The error identifier value.
 */
void show_error(unsigned int a_line_number, unsigned int a_identifier);
#endif

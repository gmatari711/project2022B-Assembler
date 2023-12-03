#ifndef ASSEMBLER_MISC_H
#define ASSEMBLER_MISC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define         LINE_BUFFER_SIZE     81
#define         NULL_OBJECT          NULL
#define         MAX_LINE_LENGTH      80
#define         MIN_THRESHOLD        100

#define TEST_NULL(P) assert(NULL != P)
#define TEST_NULLS(P1,P2) assert(NULL != P1 && NULL != P2)

typedef         char                    Line[LINE_BUFFER_SIZE];
typedef         enum{False = 0, True}   Boolean;
typedef         unsigned char                    byte_t;
typedef         char                            *String;



#endif


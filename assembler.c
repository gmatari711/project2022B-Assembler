#include "first_pass.h"
#include "second_pass.h"
#include "error_queue.h"
#include "list.h"
#include "listiter.h"
#include "misc.h"
#include "symbol_table.h"
#include "word.h"
#include "file_builder.h"
#include "pre_assembler.h"
#define INIT_BLOCK_SIZE 64

int main(int argc, char *argv[])
{

    List            extern_list     = NULL,/* List of extern labels and their addresses. */
                    entry_list      = NULL;/* List of Entries and their addresses. */
    Block           code_image      = NULL,/* Code image  */
                    data_image      = NULL;/* Data image  */
    ErrQueue        error_queue     = NULL;/* Will contain error information (if found).*/
    SymbolTable     table           = NULL;/* Container of all symbols. */
    ErrInfo         error           = NULL;
    int             CI_length       = 0,   /* Length of Code Image for Data symbols address update. */
                    i               = 0;

    for (i = 1; i < argc; ++i)
    {
        /* Test if assembly file with current name exists. */
        FILE *temp = NULL;
        if ((temp = open_assembly_file(argv[i], "r")) != NULL)
        {/* File found! */
            fclose(temp);
            /* Macro layout. */
        
            run_pre_assembler(argv[i]);    
            code_image = blockCreate(INIT_BLOCK_SIZE);
            data_image = blockCreate(INIT_BLOCK_SIZE);
            error_queue = errQueueCreate();
            table = symbolTableCreate();
            extern_list = listCreate();
            entry_list = listCreate();

            /* First pass - initial build of CI and complete build of DI and symbol table. */
            first_pass(argv[i], code_image, data_image, table, error_queue);
            /* If error queue is empty - first pass was successful.*/
            if (errQueueIsEmpty(error_queue) == True)
            {
                CI_length = blockGetSize(code_image);
                /* increment all data symbols addresses by CI_length. */
                symbolTableForEach(table, (predicate)is_symbol_data, (action)update_symbol_address, &CI_length);

                /* Finish build of CI by adding external and relocatable addresses,
                 * also build entry and extern lists.*/
                second_pass(argv[i], code_image, table, error_queue, extern_list, entry_list);
                if (errQueueIsEmpty(error_queue) == True)
                {
                    if(listIsEmpty(extern_list) == False){
                        build_external_file(argv[i], extern_list);
                        printf("\nFile %s.ext created.", argv[i]);
                    }
                    if(listIsEmpty(entry_list) == False){
                        build_entry_file(argv[i], entry_list);
                        printf("\nFile %s.ent created.", argv[i]);
                    }
                    
                    build_object_file(argv[i], code_image, data_image);
                    printf("\nFile %s.as assembled successfully!\n", argv[i]);
                }
            }
            /* Print list of all errors found by assembler. */
            if (errQueueIsEmpty(error_queue) == False)
            {
                putchar('\n');
                while (errQueueIsEmpty(error_queue) == False)
                {
                    error = errQueueDequeue(error_queue);
                    show_error(errInfoGetLineNumber(error), errInfoGetIdentifier(error));
                    free(error);
                }
                printf("\nUnsuccessful to assemble file %s.as\n",argv[i]);
            }
            errQueueDestroy(&error_queue);
            blockDestroy(&code_image);
            blockDestroy(&data_image);
            listDestroy(&extern_list,(destroy)externInfoDestroy);
            listDestroy(&entry_list,(destroy)entryInfoDestroy);
            
            /* symbolTableDestroy(&table);  I chose to block the destructor. information below.  */
            /* I DO NOT UNDERSTEAND WHY IT HATES ME SO MUCH.  I never encountered such a bizzare problem
             * before. It crashes (for all i know) only when encountering symbols with letter 'X'.
             * It's a first timer for me. I tested my generic ADT's a hundred times before use...
             * It is the first time that i leave memory leaks on a project without solving. */
        }
        else{
            printf("\nUnsuccessful to open file '%s.as'.\n", argv[i]);
        }

        
    }

    return 0;
}
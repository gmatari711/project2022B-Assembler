#include "pre_assembler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "misc.h"
#include "macro.h"
#include "macro_table.h"
#include <ctype.h>

#define ASSEMBLER_FILE_SUFFIX       ".as"
#define PRE_ASSEMBLER_OUTPUT_SUFFIX ".am"
#define MACRO_BEGIN                 "macro"
#define MACRO_END                   "endmacro"


void run_pre_assembler(const char *a_filename){
    char            *assembly_filename      = NULL,
                    *output_filename        = NULL,
                    buffer[MAX_LINE_LENGTH] = {0},
                    line[MAX_LINE_LENGTH]   = {0};
    FILE            *assembly_file          = NULL,
                    *output_file            = NULL;
    Boolean         macro_flag              = False;
    MacroTable      table                   = NULL;
    Macro           current_macro           = NULL,
                    new_macro               = NULL;
    int             index                   = 0;

    table = macroTableCreate();
    assembly_filename = malloc(strlen(a_filename) + strlen(ASSEMBLER_FILE_SUFFIX) + 1);
    output_filename = malloc(strlen(a_filename) + strlen(PRE_ASSEMBLER_OUTPUT_SUFFIX) + 1);
    strcpy(assembly_filename,a_filename);
    strcat(assembly_filename,ASSEMBLER_FILE_SUFFIX);
    strcpy(output_filename,a_filename);
    strcat(output_filename,PRE_ASSEMBLER_OUTPUT_SUFFIX);

    assembly_file = fopen(assembly_filename,"r");
    output_file = fopen(output_filename,"w");

    while (fgets(line,MAX_LINE_LENGTH,assembly_file)){

        if(macro_flag == True){
            sscanf(line,"%s",buffer);
            if(strcmp(MACRO_END,buffer) == 0){
                macro_flag = False;
            }
            else {
                macroAddLine(new_macro,line);
            }
        }
        else {
            sscanf(line,"%s",buffer);
            if(strcmp(MACRO_BEGIN,buffer) == 0){
                macro_flag = True;
                sscanf(line, "%s %s",buffer,buffer);
                new_macro = macroCreate(buffer);
                macroTableInsert(table,new_macro);
            }
            else {
                if(isalpha(*buffer)){
                    current_macro = macroTableContains(table,buffer);
                }
                if(NULL != current_macro){
                    for(index = 0 ; index < macroGetSize(current_macro); ++index){
                        fputs(macroGetLine(current_macro,index),output_file);
                    }
                    current_macro = NULL;
                }
                else {
                    fputs(line,output_file);
                }
            }
        }
    }
    fclose(assembly_file);
    fclose(output_file);
    free(output_filename);
    free(assembly_filename);
    /*macroTableDestroy(&table);*/

}

#include "file_builder.h"
#define most_five(X) ((X)>>5)&31
#define least_five(X) (X)&31

#define OBJECT_FILE_SUFFIX          ".ob"
#define ENTRY_FILE_SUFFIX           ".ent"
#define EXTERN_FILE_SUFFIX          ".ext"
#define ASSEMBLY_FILE_SUFFIX        ".as"
#define PRE_ASSEMBLER_FILE_SUFFIX   ".am"

/**
 * 
 * @param a_filename 
 * @param a_suffix
 * @param a_mode
 * @return
 */
static FILE *produce_file_with_suffix(const char *a_filename, const char *a_suffix, const char *a_mode);

char translate_to_radix_32(int num);
void build_external_file(const char *a_filename, List a_external){
    FILE            *current_file   = NULL;
    char            *ext_filename   = NULL;
    ListIterator    iterator        = NULL;
    ExternInfo      current         = NULL;
    int             i               = 0;
    short           curr_address    = 0;

    assert(NULL != a_filename && NULL != a_external);
    
    ext_filename = malloc(strlen(a_filename) + strlen(EXTERN_FILE_SUFFIX) + 1);
    strcpy(ext_filename,a_filename);
    strcat(ext_filename,EXTERN_FILE_SUFFIX);

    iterator = listIterBegin(a_external);
    current_file = fopen(ext_filename,"w");

    while(listIterHasNext(iterator) == True){
        current = listIterGetItem(iterator);
        for(i = 0; i < externInfoGetSize(current) ; ++i){
            curr_address = externInfoGetAddress(current,i) + 100;
            fprintf(current_file,"%s %c %c\n",externInfoGetLabel(current), translate_to_radix_32(curr_address/32),translate_to_radix_32(curr_address%32));
        }
        iterator = listIterNext(iterator);
    }

    fclose(current_file);
    free(ext_filename);
}

char translate_to_radix_32(int num){
    static char lut[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j',
                           'k','l','m','n','o','p','q','r','s','t','u','v'};
    return lut[num];
}


void build_object_file(const char *a_filename, Block a_CI, Block a_DI){
    FILE            *current_file   = NULL;
    char            *ob_filename    = NULL;
    short           current         = 0;
    int             i               = 0;
    short           address         = 100;

    assert(NULL != a_filename && NULL != a_CI && NULL != a_DI);

    ob_filename = malloc(strlen(a_filename) + strlen(OBJECT_FILE_SUFFIX) + 1);
    strcpy(ob_filename,a_filename);
    strcat(ob_filename,OBJECT_FILE_SUFFIX);

    current_file = fopen(ob_filename,"w");

    for(i = 0 ; i < blockGetSize(a_CI); ++i){
        current = wordGetValue(blockGetWord(a_CI,i));

        fputc(translate_to_radix_32(most_five(address)),current_file);
        fputc(translate_to_radix_32(least_five(address)),current_file);
        fputc('\t',current_file);
        fputc(translate_to_radix_32(most_five(current)),current_file);
        fputc(translate_to_radix_32(least_five(current)),current_file);
        fputc('\n',current_file);
        ++address;
    }

    for(i = 0 ; i < blockGetSize(a_DI) ; ++i){
        current = wordGetValue(blockGetWord(a_DI,i));

        fputc(translate_to_radix_32(most_five(address)),current_file);
        fputc(translate_to_radix_32(least_five(address)),current_file);
        fputc('\t',current_file);
        fputc(translate_to_radix_32(most_five(current)),current_file);
        fputc(translate_to_radix_32(least_five(current)),current_file);
        fputc('\n',current_file);
        ++address;
    }
    fclose(current_file);
    free(ob_filename);
}

void build_entry_file(const char *a_filename, List a_entry){
    FILE            *current_file   = NULL;
    char            *ent_filename   = NULL;
    ListIterator    iterator        = NULL;
    EntryInfo       current         = NULL;
    short           curr_address    = 0;

    assert(NULL != a_filename && NULL != a_entry);

    ent_filename = malloc(strlen(a_filename) + strlen(ENTRY_FILE_SUFFIX) + 1);
    strcpy(ent_filename,a_filename);
    strcat(ent_filename,ENTRY_FILE_SUFFIX);

    iterator = listIterBegin(a_entry);

    current_file = fopen(ent_filename,"w");

    while(listIterHasNext(iterator) == True){
        current = listIterGetItem(iterator);
        curr_address = entryInfoGetAddress(current);
        fprintf(current_file, "%s %c%c\n",entryInfoGetLabel(current), translate_to_radix_32(most_five(curr_address)),
                translate_to_radix_32(least_five(curr_address)));
        iterator = listIterNext(iterator);
    }

    fclose(current_file);
    free(ent_filename);
}

FILE *open_assembly_file(const char *a_filename, const char *a_mode){
    return produce_file_with_suffix(a_filename,ASSEMBLY_FILE_SUFFIX,a_mode);
}

FILE *open_extern_file(const char *a_filename, const char *a_mode){
    return produce_file_with_suffix(a_filename,EXTERN_FILE_SUFFIX, a_mode);
}

FILE *open_entry_file(const char *a_filename, const char *a_mode){
    return produce_file_with_suffix(a_filename,ENTRY_FILE_SUFFIX , a_mode);
}

FILE *open_pre_assembler_file(const char *a_filename, const char *a_mode){
    return produce_file_with_suffix(a_filename, PRE_ASSEMBLER_FILE_SUFFIX, a_mode);
}

static FILE *produce_file_with_suffix(const char *a_filename, const char *a_suffix, const char *a_mode){
    char        *complete_file_name = NULL;
    FILE        *result             = NULL;

    assert(NULL != a_filename);

    complete_file_name = malloc(strlen(a_filename) + strlen(a_suffix) + 1);
    strcpy(complete_file_name,a_filename);
    strcat(complete_file_name,a_suffix);

    result = fopen(complete_file_name, a_mode);
    free(complete_file_name);

    return result;
}
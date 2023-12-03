cmake_minimum_required(VERSION 3.25)
project(Assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Assembler
        assembler.c
        entry.c
        entry.h
        error_queue.c
        error_queue.h
        extern.c
        extern.h
        file_builder.c
        file_builder.h
        first_pass.c
        first_pass.h
        hashmap.c
        hashmap.h
        instruction_encoding.c
        instruction_encoding.h
        list.c
        list.h
        listiter.h
        macro.c
        macro.h
        macro_table.c
        macro_table.h
        misc.h
        parse_data.c
        parse_data.h
        pre_assembler.c
        pre_assembler.h
        second_pass.c
        second_pass.h
        symbol.c
        symbol.h
        symbol_table.c
        symbol_table.h
        word.c
        word.h)

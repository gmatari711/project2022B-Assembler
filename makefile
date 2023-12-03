assembler:	assembler.o pre_assembler.o parse_data.o first_pass.o second_pass.o instruction_encoding.o file_builder.o entry.o extern.o symbol.o symbol_table.o error_queue.o hashmap.o list.o macro.o macro_table.o word.o
	gcc -ansi -pedantic -Wall -o   assembler assembler.o pre_assembler.o parse_data.o first_pass.o second_pass.o instruction_encoding.o file_builder.o entry.o extern.o symbol.o symbol_table.o error_queue.o hashmap.o list.o macro.o macro_table.o word.o

assembler.o: assembler.c first_pass.h second_pass.h error_queue.h list.h listiter.h misc.h symbol_table.h word.h file_builder.h pre_assembler.h
	gcc -ansi -pedantic -Wall -c assembler.c

entry.o: entry.c entry.h 
	gcc -ansi -pedantic -Wall -c entry.c

extern.o: extern.c extern.h
	gcc -ansi -pedantic -Wall -c extern.c

error_queue.o: error_queue.c error_queue.h misc.h list.h listiter.h
	gcc -ansi -pedantic -Wall -c error_queue.c

file_builder.o: file_builder.c file_builder.h entry.h extern.h word.h list.h listiter.h
	gcc -ansi -pedantic -Wall -c file_builder.c

macro.o: macro.c macro.h misc.h
	gcc -ansi -pedantic -Wall -c macro.c

macro_table.o: macro_table.c macro_table.h hashmap.h macro.h misc.h
	gcc -ansi -pedantic -Wall -c macro_table.c

parse_data.o: parse_data.c parse_data.h misc.h
	gcc -ansi -pedantic -Wall -c parse_data.c

pre_assembler.o: pre_assembler.c pre_assembler.h macro.h macro_table.h misc.h
	gcc -ansi -pedantic -Wall -c pre_assembler.c

second_pass.o: second_pass.c second_pass.h file_builder.h word.h symbol_table.h error_queue.h parse_data.h misc.h extern.h entry.h instruction_encoding.h
	gcc -ansi -pedantic -Wall -c second_pass.c

first_pass.o: first_pass.c first_pass.h file_builder.h misc.h parse_data.h symbol_table.h error_queue.h word.h instruction_encoding.h
	gcc -ansi -pedantic -Wall -c first_pass.c

symbol_table.o: symbol_table.c symbol_table.h symbol.h misc.h
	gcc -ansi -pedantic -Wall -c symbol_table.c

symbol.o: symbol.h symbol.c misc.h hashmap.h
	gcc -ansi -pedantic -Wall -c symbol.c

word.o: word.c word.h
	gcc -ansi -pedantic -Wall -c word.c

list.o: list.c list.h list.h misc.h 
	gcc -ansi -pedantic -Wall -c list.c

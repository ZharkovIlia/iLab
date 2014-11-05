#ifndef STACK_PROCESSOR_H
#define STACK_PROCESSOR_H

void Print_name_oper(const int operation);
//prints name of operation

int Stack_assembler( const char * str_file_in, const char * str_file_out);

int Num_one_oper(const int operation);
//returns the number, which means how many codes this operation contains

void Del_useless_space(char* s);
// deletes all unnecessary spaces in the string s

int Stack_processor(const char* str_file);

int Stack_disassembler(const char * str_file_in, const char * str_file_out);
#endif // STACK_PROCESSOR


#ifndef MY_STACK_H
#define MY_STACK_H

#include <stddef.h> //reqired for size_t

//this is reqired
typedef struct
{
    double * data;
    double * stack_pointer;
    size_t size;
    int last_operation_status; //not reqired, but you can store there error codes
} my_stack_double;

my_stack_double * my_stack_double_new( size_t size);
// if size < 0 or function malloc does not work, returns NULL else returns pointer to my_stack_double

int               my_stack_double_delete( my_stack_double * this);
// deletes my_stack_double if my_stack_double is ok else returns ERROR_MY_STACK_DELETE


size_t my_stack_double_size( my_stack_double * this);
// if my_stack_double is not ok, returns ERROR_STACK_SIZE

int my_stack_double_empty( my_stack_double * this);
// if my_stack_double is not ok, returns ERROR_STACK_EMPTY


int my_stack_double_push( my_stack_double * this, double data);
/* if my_stack_double is not ok, returns ERROR_PUSH
   if my_stack_double is ok, but function realloc does not work, returns ERROR_PUSH_REALLOC */

void my_stack_double_swap( my_stack_double * this);
// if there are more than one element in my_stack_double, changes two top elements

double my_stack_double_top( my_stack_double * this);
// be careful! if there are no elements in my_stack_double or my_stack_double does not exist, program fail because of assert

double my_stack_double_pop( my_stack_double * this);
// be careful! if there are no elements in my_stack_double or my_stack_double does not exist, program fail because of assert

void my_stack_double_dump( my_stack_double * this);

//this is my own functions
int my_stack_double_ok(const my_stack_double * this);
// checks if my_stack_double is ok

void print_stack_code_error(const int code_error);

//this is an additional task
typedef struct
{
    void ** data;
    void ** stack_pointer;
    size_t size;
    size_t size_of_elem;
    int last_operation_status; //not reqired, but you can store there error codes
} my_stack;

my_stack * my_stack_new( size_t size);
int        my_stack_delete( my_stack * this, void ( *element_delete)());

size_t my_stack_size( my_stack * this);
int my_stack_empty( my_stack * this);

int my_stack_push( my_stack * this, void * data);
void my_stack_swap( my_stack * this);
void * my_stack_pop( my_stack * this);
void * my_stack_top( my_stack * this);

void my_stack_dump( my_stack * this, void ( *dump_element)( void *));

#endif  //MY_STACK_H

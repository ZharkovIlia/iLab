#include "my_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
const int ALLRIGHT = 0;
const int ERROR_MY_STACK_DELETE = 1;
const int ERROR_STACK_SIZE = -1;
const int ERROR_STACK_EMPTY = 2;
const int DO_AGAIN = 5;
const int ERROR_PUSH_REALLOC = 3;
const int ERROR_PUSH = 4;

my_stack_double * my_stack_double_new(size_t size)
{
    int i = 0;
    int s = size;
    if (s < 0) {
        return NULL;
    }
    my_stack_double * stack_ptr = NULL;
    stack_ptr = (my_stack_double*) malloc(sizeof(*stack_ptr));
    if (stack_ptr != NULL) {
        stack_ptr->stack_pointer = stack_ptr->data = (double*) malloc(size*sizeof(*(stack_ptr->data)));
    }
    i = 1;
    while ((stack_ptr == NULL || stack_ptr->data == NULL) && i < DO_AGAIN) {
        if (stack_ptr != NULL) {
            free(stack_ptr);
        }
        stack_ptr = (my_stack_double*) malloc(sizeof(*stack_ptr));
        if (stack_ptr != NULL) {
            stack_ptr->stack_pointer = stack_ptr->data = (double*) malloc(size*sizeof(*(stack_ptr->data)));
        }
        i++;
    }
    if (stack_ptr == NULL || stack_ptr->data == NULL) {
        if (stack_ptr != NULL) {
            free(stack_ptr);
        }
        return NULL;
    }
    stack_ptr->size = size;
    return stack_ptr;
}

int my_stack_double_delete( my_stack_double * this)
{
    if (!my_stack_double_ok(this)) {
        return ERROR_MY_STACK_DELETE;
    }
    this->stack_pointer = NULL;
    this->size = 0;
    free(this->data);
    this->data = NULL;
    free(this);
    this = NULL;
    return ALLRIGHT;
}

void print_stack_code_error(const int code_error)
{
    switch (code_error) {
        case 0:
            printf("code_error is ALLRIGHT\n");
            break;
        case 1:
            printf("code_error is ERROR_MY_STACK_DELETE\n");
            break;
        case -1:
            printf("code_error is ERROR_STACK_SIZE\n");
            break;
        case 2:
            printf("code_error is ERROR_STACK_EMPTY\n");
            break;
        case 3:
            printf("code_error is ERROR_PUSH_REALLOC\n");
            break;
        case 4:
            printf("code_error is ERROR_PUSH\n");
            break;
        default:
            printf("this code_error does not exist");
            break;
    }
}

int my_stack_double_ok(const my_stack_double * this)
{
    long long int int_size;
    if (this != NULL) {
        int_size = this->size;
    }
    return (this != NULL &&\
           (this->data) != NULL &&\
           (this->stack_pointer) >= (this->data) &&\
           (int_size >= 0) &&\
           (this->stack_pointer) - (this->data) <= (int_size) );
}

void my_stack_double_dump( my_stack_double * this)
{
    int i = 0, j = 0;
    long long int int_size;
    printf("/* my_stack_double_dump\nmy_stack_double [%p] is ", this);
    if (my_stack_double_ok(this)) {
        printf("ok\n");
    } else {
        printf("BAD!!!\n");
    }
    if (this != NULL) {
        int_size = this->size;
        printf("current size = %d, max size = %d\n", this->stack_pointer - this->data, this->size);
        printf("data [%p] ", this->data);
        if (this->data != NULL) {
            if (this->size == 0) {
                printf("{}\n");
            } else {
                printf("\n{\n");
                for (j = 0; j < 3 && j < (this->stack_pointer - this->data) - 3 && j < int_size - 3; j++) {
                    printf("[%d] = %lg *\n", j, (this->data)[j]);
                }
                if (j == 3) {
                    printf(".......................\n");
                }
                if (this->stack_pointer - this->data <= int_size) {
                    for (i = ((this->stack_pointer - this->data) - 3 >= j) ? (this->stack_pointer - this->data) - 3 : j; i < this->stack_pointer - this->data; i++) {
                        printf("[%d] = %lg *\n", i, (this->data)[i]);
                    }
                    for (j = i; (j < i+3) && (j < int_size - 3); j++) {
                        printf("[%d] = %lg\n", j, (this->data)[j]);
                    }
                    if (j == i+3) {
                        printf(".......................\n");
                    }
                    for (i = (int_size - 3 >= j) ? int_size - 3 : j; i < int_size; i++) {
                        printf("[%d] = %lg\n", i, (this->data)[i]);
                    }
                } else {
                    for (i = (int_size - 3 >= j) ? int_size - 3 : j; i < int_size; i++) {
                        printf("[%d] = %lg *\n", i, (this->data)[i]);
                    }
                }
                printf("}\n");
            }
        }
    }
    printf("*/\n");
}

size_t my_stack_double_size( my_stack_double * this)
{
    if (my_stack_double_ok(this)) {
        return this->size;
    }
    return ERROR_STACK_SIZE;
}

int my_stack_double_empty( my_stack_double * this)
{
    if (my_stack_double_ok(this)) {
        this->stack_pointer = this->data;
        return ALLRIGHT;
    }
    return ERROR_STACK_EMPTY;
}

int my_stack_double_push( my_stack_double * this, double data)
{
    int i = 0;
    double * data_ptr = NULL;
    if (my_stack_double_ok(this)) {
        if ( (this->stack_pointer - this->data) == this->size ) {
            i = 0;
            do {
                if (this->size == 0) {
                    data_ptr = (double*) realloc(this->data, sizeof(*data_ptr));
                } else {
                    data_ptr = (double*) realloc(this->data, (this->size)*2*sizeof(*data_ptr));
                }
                i++;
            } while (i < DO_AGAIN && data_ptr == NULL);
            if (data_ptr == NULL) {
                return ERROR_PUSH_REALLOC;
            } else {
                this->data = data_ptr;
                this->stack_pointer = this->data + this->size;
                if (this->size == 0) {
                    this->size = 1;
                } else {
                    this->size *= 2;
                }
                *(this->stack_pointer++) = data;
                return ALLRIGHT;
            }
        } else {
            *(this->stack_pointer++) = data;
            return ALLRIGHT;
        }
    }
    return ERROR_PUSH;
}

double my_stack_double_pop( my_stack_double * this)
{
    if (my_stack_double_ok(this)) {
        if (this->stack_pointer - this->data > 0) {
            return *(--(this->stack_pointer));
        } else {
            assert(!"ERROR!!! my_stack_double_pop: my_stack_double is already empty.\n");
        }
    }
    assert(!"ERROR!!! my_stack_double_pop: my_stack_double is bad or does not exist");
}

void my_stack_double_swap( my_stack_double * this)
{
    double a = 0, b = 0;
    if (my_stack_double_ok(this)) {
        if (this->stack_pointer - this->data >= 2) {
            a = my_stack_double_pop(this);
            b = my_stack_double_pop(this);
            my_stack_double_push(this, a);
            my_stack_double_push(this, b);
        }
    }
}

double my_stack_double_top( my_stack_double * this)
{
    double a = my_stack_double_pop(this);
    my_stack_double_push(this, a);
    return a;
}





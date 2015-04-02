#include "my_vector.h"
#include <string.h>
#include <iostream>
#include <assert.h>

#define DEBUG_CEXCEPTION_VECT

#ifndef DEBUG_CEXCEPTION_VECT

#define assert(cond)

#endif // DEBUG_CEXCEPTION_VECT

template <typename Elem_T>
CException_vect<Elem_T>::CException_vect(const ERROR_VECTOR err):
    message_(NULL),
    err_(err)
{
    switch (err) {
        case GOOD:
            strcpy(this->message_, "CVector is correct. Use CException_vect has been unnecessary.");
            break;
        case ERR_CTOR:
            strcpy(this->message_, "Invalid parameters of CVector's constructors. May be you have some logical errors.");
            break;
        case ERR_ALLOC:
            strcpy(this->message_, "Some problems with allocating memory, caused by operator new.");
            break;
        default:
            assert(!"Dear developer! You have forgotten to change this function!");
            break;
    }
}

template <typename Elem_T>
CException_vect<Elem_T>::CException_vect()
{
    ; // never use
}

template <typename Elem_T>
CException_vect<Elem_T>::~CException_vect()
{
    delete [] this->message_;
    this->message_ = NULL;
}

template <typename Elem_T>
const char * CException_vect<Elem_T>::what() const
{
    return (const char*) this->message_;
}


#include "my_vector.h"
#include <string.h>
#include <iostream>
#include <assert.h>

#define DEBUG_CEXCEPTION_VECT

#ifndef DEBUG_CEXCEPTION_VECT

#define assert(cond)

#endif // DEBUG_CEXCEPTION_VECT

//Be careful! See MAXLEN_MESSAGE if you want do add new message
CException_vect::CException_vect(const ERROR_VECTOR err):
    //message_(NULL),
    err_(err)
{
    switch (err) {
        case GOOD:
            strcpy(this->message_, "CVector is correct. Use CException_vect has been unnecessary.");
            break;
        case ERR_CTOR:
            strcpy(this->message_, "ERROR! Invalid parameters of CVector's constructors. May be you have some logical errors.");
            break;
        case ERR_ALLOC:
            strcpy(this->message_, "ERROR! Some problems with allocating memory, caused by operator new.");
            break;
        case ERR_SEGMENT:
            strcpy(this->message_, "ERROR! There are no elements in the CVector, but you try to refer to the elements.");
            break;
        case ERR_INDEX:
            strcpy(this->message_, "ERROR! Invalid index of array (negative or too big for current size).");
            break;
        case ERR_RESIZE:
            strcpy(this->message_, "ERROR! You have tried to resize CVector with negative size.");
            break;
        case ERR_OPER:
            strcpy(this->message_, "ERROR! You have tried to do an operation with two CVectors of different sizes.");
            break;
        case ERR_VALUE:
            strcpy(this->message_, "ERROR! Invalid value, which was transferred for division (may be it is zero).");
            break;
        default:
            assert(!"Dear developer! You have forgotten to change this function!");
            break;
    }
}


CException_vect::CException_vect()
{
    ; // never use
}


const char * CException_vect::what() const
{
    return (const char*) this->message_;
}



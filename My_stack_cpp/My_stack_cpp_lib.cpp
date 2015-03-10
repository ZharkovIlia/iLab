#include "My_stack_cpp.h"
#include <string.h>
#include <iostream>
#include <assert.h>
//#define DEBUG_STACK

#ifndef DEBUG_STACK
#define ASSERT_OK_ST
#define assert_ok(cond...);
#endif // DEBUG_STACK

Cstack_double::Cstack_double():
    data_ (new double[1]),
    dataptr_ (NULL),
    szmax_ (1)
{
    this->dataptr_ = this->data_;
}

Cstack_double::Cstack_double(const int size):
    data_ (NULL),
    dataptr_ (NULL),
    szmax_ (0)
{
    if (size <= 0) {
        this->data_ = new double[1];
        this->dataptr_ = this->data_;
        this->szmax_ = 1;
    } else {
        this->data_ = new double[size];
        this->dataptr_ = this->data_;
        this->szmax_ = size;
    }
    memset(this->data_, 0, this->szmax_*sizeof(*(this->data_)));
}

Cstack_double::~Cstack_double()
{
    delete [] this->data_;
    this->data_ = NULL;
    this->dataptr_ = NULL;
    this->szmax_ = -1;
}

ERROR_ST_T Cstack_double::push(const double val)
{
    assert_ok(__FILE__, __LINE__);
    if (this->dataptr_ - this->data_ == this->szmax_) {
        double * doub_ptr = new double[(this->szmax_)*2];
        if (doub_ptr == NULL) {
            return ERR_ALLOC_MEM;
        } else {
            for (int i = 0; i < this->szmax_; i++) {
                doub_ptr[i] = this->data_[i];
            }
            delete [] this->data_;
            this->data_ = doub_ptr;
            this->dataptr_ = this->data_ + this->szmax_;
            this->szmax_ *= 2;
        }
    }
    *(this->dataptr_) = val;
    this->dataptr_++;
    assert_ok(__FILE__, __LINE__);
    return ALLRIGHT;
}

ERROR_ST_T Cstack_double::pop(double * val)
{
    assert_ok(__FILE__, __LINE__);
    if (this->dataptr_ - this->data_ <= 0) {
        return ERR_CONTENTS_STACK;
    }
    *val = *(this->dataptr_ - 1);
    this->dataptr_--;
    assert_ok(__FILE__, __LINE__);
    return ALLRIGHT;
}

ERROR_ST_T Cstack_double::top(double * val) const
{
    assert_ok(__FILE__, __LINE__);
    if (this->dataptr_ - this->data_ <= 0) {
        return ERR_CONTENTS_STACK;
    }
    *val = *(this->dataptr_ - 1);
    assert_ok(__FILE__, __LINE__);
    return ALLRIGHT;
}

int Cstack_double::size() const
{
    assert_ok(__FILE__, __LINE__);
    return (this->dataptr_ - this->data_);
}

void Cstack_double::do_empty()
{
    assert_ok(__FILE__, __LINE__);
    this->dataptr_ = this->data_;
    assert_ok(__FILE__, __LINE__);
}

void Cstack_double::swap()
{
    assert_ok(__FILE__, __LINE__);
    if (this->dataptr_ - this->data_ >= 2) {
        double val1;
        double val2;
        this->pop(&val1);
        assert_ok(__FILE__, __LINE__);
        this->pop(&val2);
        assert_ok(__FILE__, __LINE__);
        this->push(val1);
        assert_ok(__FILE__, __LINE__);
        this->push(val2);
        assert_ok(__FILE__, __LINE__);
    }
    assert_ok(__FILE__, __LINE__);
}

void Cstack_double::dump() const
{
    if (this->ok() == ST_BAD) {
        std::cout << "Cstack_double [" << this << "] is BAD!!!\n";
    } else {
        std::cout << "Cstack_double [" << this << "] is OK.\n";
    }
    std::cout << "data [" << this->data_ << "]\n";
    std::cout << "dataptr [" << this->dataptr_ << "]\n";
    std::cout << "szmax = " << this->szmax_ << "\n";
    std::cout << "sznow = " << this->dataptr_ - this->data_ << "\n";
    if (this->data_ == NULL) {
        return;
    }
    if (this->szmax_ > 0) {
        std::cout << "data {\n";
    }
    for (int i = 0; i < this->szmax_; i++) {
        std::cout << "[" << i << "] = " << this->data_[i];
        if (i < this->dataptr_ - this->data_) {
            std::cout << "*\n";
        } else {
            std::cout << "\n";
        }
    }
    if (this->szmax_ > 0) {
        std::cout << "}\n";
    }
}

ISRIGHT_ST_T Cstack_double::ok() const
{
    /*if (this == NULL) {
        return ST_BAD;
    }*/
    // maybe it is impossible
    if (this->data_ == NULL) {
        return ST_BAD;
    }
    if (this->dataptr_ - this->data_ > this->szmax_ || this->dataptr_ - this->data_ < 0) {
        return ST_BAD;
    }
    if (this->szmax_ <= 0) {
        return ST_BAD;
    }
    return ST_OK;
}

#ifndef ASSERT_OK_ST
void Cstack_double::assert_ok(const char * file, const int line) const {
    if (this->ok() == ST_BAD) {
        std::cout << "SDfDF\n";
        this->dump();
        std::cout << "file - " << file << "\nline = " << line << "\n";
        assert(!"SEE FILE AND LINE IN THE TEXT ABOVE");
    }
}
#endif //ASSERT_OK_ST










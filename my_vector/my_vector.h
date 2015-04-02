#ifndef MY_VECTOR
#define MY_VECTOR

#include <string.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <string>

#define DEBUG_VECTOR

#ifndef DEBUG_VECTOR
#define assert_ok(cond)
#define ASSERT_VECTOR
#endif //DEBUG_VECTOR

typedef enum {
    GOOD = 0,
    ERR_ALLOC = 1,
    ERR_CTOR = 2,
} ERROR_VECTOR;


typedef enum {
    BAD,
    OK,
} VECT_OK;


template <typename Data_T>
class CVector {
    template <typename Friend_T>
    friend std::ostringstream& operator << (std::ostringstream& ostr, const CVector<Friend_T>& vect);
public:
    CVector();
    CVector(int size);
    CVector(const Data_T * arr, const int size_arr);
    ~CVector();

    Data_T& operator [] (const int n) const;

    void resize(const int size);

    bool isdefault() const;
    ERROR_VECTOR status() const;
    std::string dump() const;
private:
    void assert_ok(const char * file, const int line) const;
    VECT_OK ok() const;

    Data_T * data_;
    int size_;
    bool defaultbit_;
    ERROR_VECTOR last_operation_status_;
};

template <typename Elem_T>
class CException_vect {
public:
    CException_vect(const ERROR_VECTOR err);
    ~CException_vect();
    const char * what() const;
private:
    CException_vect();

    CVector<Elem_T>* vectptr;
    char * message_;
    ERROR_VECTOR err_;
};


#ifndef ASSERT_VECTOR
template <typename Data_T>
void CVector<Data_T>::assert_ok(const char * file, const int line) const
{
    if (this->ok() == BAD) {
        std::cout << *this;
        std::cout << file << std::endl << "line == " << line << std::endl;
        assert(!"SEE FILE AND LINE IN THE TEXT ABOVE!");
    }
}
#endif // ASSERT_VECTOR

#undef ASSERT_VECTOR


template <typename Data_T>
CVector<Data_T>::CVector():
    data_(NULL),
    size_(0),
    defaultbit_(1),
    last_operation_status_(GOOD)
{
    ;
}

template <typename Data_T>
CVector<Data_T>::CVector(int size):
    data_(NULL),
    size_(0),
    defaultbit_(0),
    last_operation_status_(GOOD)
{
    if (size < 0) {
        this->last_operation_status_ = ERR_CTOR;
    } else if (size == 0) {
        this->defaultbit_ = 1;
    } else {
        this->data_ = new Data_T[size];
        if (this->data_ == NULL) {
            last_operation_status_ = ERR_ALLOC;
        } else {
            this->size_ = size;
            memset(this->data_, 0, this->size_ * sizeof(Data_T));
            // defaultbit == 0
            // last_operation_status_ == GOOD;
        }
    }
}

template <typename Data_T>
CVector<Data_T>::CVector(const Data_T * arr, const int size_arr):
    data_(NULL),
    size_(0),
    defaultbit_(0),
    last_operation_status_(GOOD)
{
    if (size_arr < 0 || arr == NULL) {
        this->last_operation_status_ = ERR_CTOR;
    } else if (size_arr == 0) {
        this->defaultbit_ = 1;
    } else {
        this->data_ = new Data_T[size_arr];

        if (this->data_ == NULL) {
            this->last_operation_status_ = ERR_ALLOC;
        } else {
            this->size_ = size_arr;
            memcpy(this->data_, arr, size_arr * sizeof(*(this->data_)));
        }
    }
    //this->defaultbit_ = 1;
}

template <typename Data_T>
CVector<Data_T>::~CVector()
{
    delete [] this->data_;
    this->data_ = NULL;
    this->size_ = -1;
}


template <typename Data_T>
Data_T& CVector<Data_T>::operator [] (const int n) const
{
    assert_ok(__FILE__, __LINE__);
    if (this->last_operation_status_ != GOOD) {
        assert(!"UNVALID USE CVECTOR");
    }
    if (this->defaultbit_ == 1) {
        assert(!"SEGMENTATION FAILED");
    }
    if (this->size_ <= n || n < 0) {
        assert(!"INVALID INDEX");
    }
    return this->data_[n];
}


template <typename Data_T>
void CVector<Data_T>::resize(const int size)
{
    ;
}

template <typename Data_T>
ERROR_VECTOR CVector<Data_T>::status() const
{
    assert_ok(__FILE__, __LINE__);
    return this->last_operation_status_;
}


template <typename Data_T>
bool CVector<Data_T>::isdefault() const
{
    assert_ok(__FILE__, __LINE__);
    return this->defaultbit_;
}

template <typename Data_T>
VECT_OK CVector<Data_T>::ok() const
{
    if (this->last_operation_status_ > 2) {
        return BAD;
    }
    if (this->size_ < 0) {
        return BAD;
    }
    if (this->size_ == 0 && (this->defaultbit_ == 0 || this->data_ != NULL) ) {
        return BAD;
    }
    if (this->size_ == 0) {
        /* this->defaultbit_ == 1
         * this->data_ == NULL */
        return OK;
    }
    //this->size_ > 0
    if (this->defaultbit_ == 1 || this->data_ == NULL) {
        return BAD;
    }
    return OK;
}

template <typename Data_T>
std::ostream& operator << (std::ostream& os, const CVector<Data_T>& vect)
{
    os << vect.dump();
    return os;
}

template <typename Data_T>
std::string CVector<Data_T>::dump() const
{
    std::ostringstream ostr;
    VECT_OK test_vect = BAD;
    ostr << "CVector [" << (void*) this << "] is ";
    if ( (test_vect = this->ok()) == BAD) {
        ostr << "BAD!!!" << std::endl;
    } else {
        ostr << "OK." << std::endl;
    }
    ostr << "data_ [" << (void*) this->data_ << "]" << std::endl;
    ostr << "size_ == " << this->size_ << std::endl;
    ostr << "defaultbit_ == " << this->defaultbit_ << std::endl;
    ostr << "last_operation_status_ == " << this->last_operation_status_ << std::endl;
    if (this->size_ <= 0) {
        return ostr.str();
    }
    if (this->data_ != NULL) {
        ostr << "array {" << std::endl;
        for (int i = 0; i < this->size_; i++) {
            ostr << "[" << i << "] " << this->data_[i] << std::endl;
        }
        ostr << "}" << std::endl;
    }
    return ostr.str();
}

#endif // MY_VECTOR

#ifndef MY_VECTOR
#define MY_VECTOR

#include <string.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <string>

#include <exception>


typedef enum {
    BAD,
    OK,
} VECT_OK;


class CException_vect : public std::exception {
public:
    virtual ~CException_vect() throw() { };
    virtual const char * what() const throw() = 0;
protected:
    CException_vect() throw() { };
};


class Vect_Err_ctor : public CException_vect {
public:
    Vect_Err_ctor() throw() { }
    ~Vect_Err_ctor() throw() { };
    const char * what() const throw() {
        return "ERROR! Invalid parameters of CVector's constructors. May be you have some logical errors.";
    }
};


class Vect_Err_segment : public CException_vect {
public:
    Vect_Err_segment() throw() { }
    ~Vect_Err_segment() throw() { };
    const char * what() const throw() {
        return "ERROR! There are no elements in the CVector, but you try to refer to the elements.";
    }
};


class Vect_Err_index : public CException_vect {
public:
    Vect_Err_index() throw() { }
    ~Vect_Err_index() throw() { };
    const char * what() const throw() {
        return "ERROR! Invalid index of array (negative or too big for current size).";
    }
};


class Vect_Err_resize : public CException_vect {
public:
    Vect_Err_resize() throw() { }
    ~Vect_Err_resize() throw() { };
    const char * what() const throw() {
        return "ERROR! You have tried to resize CVector with negative size.";
    }
};


class Vect_Err_oper : public CException_vect {
public:
    Vect_Err_oper() throw() { }
    ~Vect_Err_oper() throw() { };
    const char * what() const throw() {
        return "ERROR! You have tried to do an operation with two CVectors of different sizes.";
    }
};


class Vect_Err_value : public CException_vect {
public:
    Vect_Err_value() throw() { }
    ~Vect_Err_value() throw() { };
    const char * what() const throw() {
        return "ERROR! Invalid value, which was transferred for division (may be it is zero).";
    }
};


template <typename Data_T>
class CVector {

    template <typename Friend_T, typename Other_T>
    friend CVector<Friend_T>& operator += (CVector<Friend_T>& vectto, const CVector<Other_T>& vectfrom);

    template <typename Friend_T, typename Other_T>
    friend CVector<Friend_T> operator + (const CVector<Friend_T>& vectto, const CVector<Other_T>& vectfrom);

    template <typename Friend_T, typename Other_T>
    friend CVector<Friend_T>& operator -= (CVector<Friend_T>& vectto, const CVector<Other_T>& vectfrom);

    template <typename Friend_T, typename Other_T>
    friend CVector<Friend_T> operator - (const CVector<Friend_T>& vectto, const CVector<Other_T>& vectfrom);

    template <typename Friend_T, typename Value_T>
    friend CVector<Friend_T> operator * (const Value_T& value, const CVector<Friend_T>& vect);

    template <typename Friend_Ret_T, typename Friend_T>
    friend Friend_Ret_T operator * (const CVector<Friend_Ret_T>& vect_ret, const CVector<Friend_T>& vect);

public:
    CVector() throw();
    CVector(const int size, const Data_T& data = Data_T() );
    CVector(const Data_T * arr, const int size_arr);
    CVector(const CVector<Data_T>& vect);
    ~CVector() throw();


    template <typename Other_T>
    CVector<Data_T>& operator = (const CVector<Other_T>& vect);
    template <typename Value_T>
    CVector<Data_T>& operator *= (const Value_T& value);
    template <typename Value_T>
    CVector<Data_T> operator * (const Value_T& value) const;
    template <typename Value_T>
    CVector<Data_T>& operator /= (const Value_T& value);
    template <typename Value_T>
    CVector<Data_T> operator / (const Value_T& value) const;


    Data_T& operator [] (const int n) const;
    CVector<Data_T>& operator += (const CVector<Data_T>& vect);
    CVector<Data_T> operator + (const CVector<Data_T>& vect) const;
    CVector<Data_T>& operator -= (const CVector<Data_T>& vect);
    CVector<Data_T> operator - (const CVector<Data_T>& vect) const;
    CVector<Data_T>& operator = (const CVector<Data_T>& vect);
    CVector<Data_T> operator - () const;


    void resize(const int size, const Data_T& data = Data_T());
    void array(const Data_T * arr, const int size_arr);
    const Data_T * array(int * size_arr) const throw();
    inline bool isdefault() const throw();
    std::string dump() const;
private:
    void assert_ok(const char * file, const int line) const;
    VECT_OK ok() const throw();

    Data_T * data_;
    int size_;
    bool defaultbit_;
};



#define DEBUG_VECTOR

#ifndef DEBUG_VECTOR
#define assert_ok(cond...);
#define ASSERT_VECTOR
#endif //DEBUG_VECTOR



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
CVector<Data_T>::CVector() throw():
    data_(NULL),
    size_(0),
    defaultbit_(1)
{
    //std::cout << "ctor default\n";
}


template <typename Data_T>
CVector<Data_T>::CVector(const int size, const Data_T& data):
    data_(NULL),
    size_(0),
    defaultbit_(0)
{
    //std::cout << "ctor size\n";
    if (size < 0) {
        Vect_Err_ctor exc;
        throw exc;
    } else if (size == 0) {
        this->defaultbit_ = 1;
    } else {
        this->data_ = new Data_T[size];

        for (int i = 0; i < size; i++) {
            this->data_[i] = data;
        }
        this->size_ = size;
        // defaultbit == 0
    }
}


template <typename Data_T>
CVector<Data_T>::CVector(const Data_T * arr, const int size_arr):
    data_(NULL),
    size_(0),
    defaultbit_(0)
{
    //std::cout << "ctor array\n";
    if (size_arr < 0 || arr == NULL) {
        Vect_Err_ctor exc;
        throw exc;
    } else if (size_arr == 0) {
        this->defaultbit_ = 1;
    } else {
        this->data_ = new Data_T[size_arr];

        this->size_ = size_arr;
        for (int i = 0; i < size_arr; i++) {
            this->data_[i] = arr[i];
        }
        //this->defaultbit_ = 0;
    }
}


template <typename Data_T>
CVector<Data_T>::CVector(const CVector<Data_T>& vect):
    data_(NULL),
    size_(0),
    defaultbit_(1)
{
    //std::cout << "ctor copy\n";
    #ifdef DEBUG_VECTOR
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (vect.data_ == NULL) {
        ;
    } else {
        this->data_ = new Data_T[vect.size_];

        for (int i = 0; i < vect.size_; i++) {
            this->data_[i] = vect.data_[i];
        }
        this->size_ = vect.size_;
        this->defaultbit_ = vect.defaultbit_;
    }
}


template <typename Data_T>
CVector<Data_T>::~CVector() throw()
{
    delete [] this->data_;
    this->data_ = NULL;
    this->size_ = -1;
    std::cout << "deleting vector" << std::endl;
}


template <typename Data_T>
Data_T& CVector<Data_T>::operator [] (const int n) const
{
    assert_ok(__FILE__, __LINE__);
    if (this->defaultbit_ == 1) {
        Vect_Err_segment exc;
        throw exc;
    }
    if (this->size_ <= n || n < 0) {
        Vect_Err_index exc;
        throw exc;
    }
    return this->data_[n];
}


template <typename Data_T>
CVector<Data_T>& CVector<Data_T>::operator += (const CVector<Data_T>& vect)
{
    assert_ok(__FILE__, __LINE__);
    #ifdef DEBUG_VECTOR
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (this->size_ != vect.size_) {
        Vect_Err_oper exc;
        throw exc;
    }
    if (this->size_ == 0) {
        return *this;
    }
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] += vect.data_[i];
    }
    return *this;
}


template <typename Data_T>
CVector<Data_T> CVector<Data_T>::operator + (const CVector<Data_T>& vect) const
{
    assert_ok(__FILE__, __LINE__);
    #ifdef DEBUG_VECTOR
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    CVector<Data_T> addit_vect = *this;
    addit_vect += vect;
    return addit_vect;
}


template <typename Data_T>
CVector<Data_T>& CVector<Data_T>::operator -= (const CVector<Data_T>& vect)
{
    assert_ok(__FILE__, __LINE__);
    #ifdef DEBUG_VECTOR
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (this->size_ != vect.size_) {
        Vect_Err_oper exc;
        throw exc;
    }
    if (this->size_ == 0) {
        return *this;
    }
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] -= vect.data_[i];
    }
    return *this;
}


template <typename Data_T>
CVector<Data_T> CVector<Data_T>::operator - (const CVector<Data_T>& vect) const
{
    assert_ok(__FILE__, __LINE__);
    #ifdef DEBUG_VECTOR
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    CVector<Data_T> addit_vect = *this;
    addit_vect -= vect;
    return addit_vect;
}


template <typename Data_T>
CVector<Data_T>& CVector<Data_T>::operator = (const CVector<Data_T>& vect)
{
    assert_ok(__FILE__, __LINE__);
    #ifdef DEBUG_VECTOR
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (this == &vect) {
        return *this;
    }
    if (vect.data_ == NULL) {
        delete [] this->data_;
        this->data_ = NULL;
    } else if (this->size_ != vect.size_) {
        Data_T * addit_dataptr = new Data_T[vect.size_];

        delete [] this->data_;
        this->data_ = addit_dataptr;
        for (int i = 0; i < vect.size_; i++) {
            this->data_[i] = vect.data_[i];
        }
    } else {
        for (int i = 0; i < vect.size_; i++) {
            this->data_[i] = vect.data_[i];
        }
    }
    this->defaultbit_ = vect.defaultbit_;
    this->size_ = vect.size_;
    return *this;
}


template <typename Data_T>
CVector<Data_T> CVector<Data_T>::operator - () const
{
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T> addit_vect = *this;
    for (int i = 0; i < addit_vect.size_; i++) {
        addit_vect.data_[i] = -addit_vect.data_[i];
    }
    return addit_vect;
}


template <typename Data_T>
inline std::ostream& operator << (std::ostream& os, const CVector<Data_T>& vect)
{
    os << vect.dump();
    return os;
}


template <typename Data_T>
template <typename Other_T>
CVector<Data_T>& CVector<Data_T>::operator = (const CVector<Other_T>& vect)
{
    assert_ok(__FILE__, __LINE__);
    int size = 0;
    const Other_T * addit_otherdata_ptr = vect.array(&size);
    if (addit_otherdata_ptr == NULL) {
        delete [] this->data_;
        this->data_ = NULL;
    } else if (this->size_ != size) {
        Data_T * addit_dataptr = new Data_T[size];

        delete [] this->data_;
        this->data_ = addit_dataptr;
        for (int i = 0; i < size; i++) {
            this->data_[i] = addit_otherdata_ptr[i];
        }
    } else {
        for (int i = 0; i < size; i++) {
            this->data_[i] = addit_otherdata_ptr[i];
        }
    }
    this->defaultbit_ = vect.isdefault();
    this->size_ = size;
    return *this;
}


template <typename Data_T, typename Other_T>
CVector<Data_T>& operator += (CVector<Data_T>& vectto, const CVector<Other_T>& vectfrom)
{
    #ifdef DEBUG_VECTOR
    vectto.assert_ok(__FILE__, __LINE__);
    vectfrom.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (vectto.size_ != vectfrom.size_) {
        Vect_Err_oper exc;
        throw exc;
    }
    if (vectto.size_ == 0) {
        return vectto;
    }
    for (int i = 0; i < vectto.size_; i++) {
        vectto.data_[i] += vectfrom.data_[i];
    }
    return vectto;
}


template <typename Data_T, typename Other_T>
CVector<Data_T> operator + (const CVector<Data_T>& vectto, const CVector<Other_T>& vectfrom)
{
    #ifdef DEBUG_VECTOR
    vectfrom.assert_ok(__FILE__, __LINE__);
    vectto.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    CVector<Data_T> addit_vect = vectto;
    addit_vect += vectfrom;
    return addit_vect;
}


template <typename Data_T, typename Other_T>
CVector<Data_T>& operator -= (CVector<Data_T>& vectto, const CVector<Other_T>& vectfrom)
{
    #ifdef DEBUG_VECTOR
    vectto.assert_ok(__FILE__, __LINE__);
    vectfrom.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (vectto.size_ != vectfrom.size_) {
        Vect_Err_oper exc;
        throw exc;
    }
    if (vectto.size_ == 0) {
        return vectto;
    }
    for (int i = 0; i < vectto.size_; i++) {
        vectto.data_[i] -= vectfrom.data_[i];
    }
    return vectto;
}


template <typename Data_T, typename Other_T>
CVector<Data_T> operator - (const CVector<Data_T>& vectto, const CVector<Other_T>& vectfrom)
{
    #ifdef DEBUG_VECTOR
    vectfrom.assert_ok(__FILE__, __LINE__);
    vectto.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    CVector<Data_T> addit_vect = vectto;
    addit_vect -= vectfrom;
    return addit_vect;
}


template <typename Data_T>
template <typename Value_T>
CVector<Data_T>& CVector<Data_T>::operator *= (const Value_T& value)
{
    assert_ok(__FILE__, __LINE__);
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] *= value;
    }
    return *this;
}


template <typename Data_T>
template <typename Value_T>
CVector<Data_T> CVector<Data_T>::operator * (const Value_T& value) const
{
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T> addit_vect = *this;
    addit_vect *= value;
    return addit_vect;
}


template <typename Data_T, typename Value_T>
CVector<Data_T> operator * (const Value_T& value, const CVector<Data_T>& vect)
{
    #ifdef DEBUG_VECTOR
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    return vect * value;
}


template <typename Data_T>
template <typename Value_T>
CVector<Data_T>& CVector<Data_T>::operator /= (const Value_T& value)
{
    assert_ok(__FILE__, __LINE__);
    if (value == 0) {
        Vect_Err_value exc;
        throw exc;
    }
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] /= value;
    }
    return *this;
}


template <typename Data_T>
template <typename Value_T>
CVector<Data_T> CVector<Data_T>::operator / (const Value_T& value) const
{
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T> addit_vect = *this;
    addit_vect /= value;
    return addit_vect;
}


template <typename Data_Ret_T, typename Data_T>
Data_Ret_T operator * (const CVector<Data_Ret_T>& vect_ret, const CVector<Data_T>& vect)
{
    #ifdef DEBUG_VECTOR
    vect_ret.assert_ok(__FILE__, __LINE__);
    vect.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (vect_ret.size_ != vect.size_) {
        Vect_Err_oper exc;
        throw exc;
    }
    Data_Ret_T value = 0;
    for (int i = 0; i < vect.size_; i++) {
        value += (vect_ret.data_[i] * vect.data_[i]);
    }
    return value;
}


template <typename Data_T>
void CVector<Data_T>::resize(const int size, const Data_T& data)
{
    assert_ok(__FILE__, __LINE__);
    if (size < 0) {
        Vect_Err_resize exc;
        throw exc;
    }
    if (size == 0) {
        delete [] this->data_;
        this->data_ = NULL;
        this->defaultbit_ = 1;
        this->size_ = 0;
        return;
    }
    if (size == this->size_) {
        return;
    }
    Data_T * addit_dataptr = new Data_T[size];

    if (size >= this->size_) {
        for (int i = 0; i < this->size_; i++) {
            addit_dataptr[i] = this->data_[i];
        }
        for (int i = this->size_; i < size; i++) {
            addit_dataptr[i] = data;
        }
    } else {
        for (int i = 0; i < size; i++) {
            addit_dataptr[i] = this->data_[i];
        }
    }
    delete [] this->data_;
    this->data_ = addit_dataptr;
    this->size_ = size;
    this->defaultbit_ = 0;
    return;
}


template <typename Data_T>
void CVector<Data_T>::array(const Data_T * arr, const int size_arr)
{
    assert_ok(__FILE__, __LINE__);
    if (size_arr < 0) {
        Vect_Err_ctor exc;
        throw exc;
    }
    if (size_arr == 0 || arr == NULL) {
        delete [] this->data_;
        this->data_ = NULL;
        this->size_ = 0;
        this->defaultbit_ = 1;
        return;
    }
    if (size_arr > 0) {
        Data_T * addit_dataptr = new Data_T[size_arr];

        for (int i = 0; i < size_arr; i++) {
            addit_dataptr[i] = arr[i];
        }
        delete [] this->data_;
        this->data_ = addit_dataptr;
        this->size_ = size_arr;
        this->defaultbit_ = 0;
    }
}


template <typename Data_T>
const Data_T * CVector<Data_T>::array(int * size_arr) const throw()
{
    assert_ok(__FILE__, __LINE__);
    if (size_arr != NULL) {
        *size_arr = this->size_;
    }
    return (const Data_T*) this->data_;
}


template <typename Data_T>
inline bool CVector<Data_T>::isdefault() const throw()
{
    assert_ok(__FILE__, __LINE__);
    return this->defaultbit_;
}


template <typename Data_T>
VECT_OK CVector<Data_T>::ok() const throw()
{
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


#ifndef MY_VECTOR
#define MY_VECTOR

#include <string.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <string>

#include <exception>

#define DEBUG_VECTOR

#ifndef DEBUG_VECTOR
#define assert_ok(cond...);
#define ASSERT_VECTOR
#endif //DEBUG_VECTOR


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


class Vect_Err_static_size: public CException_vect {
public:
    Vect_Err_static_size() throw() { }
    ~Vect_Err_static_size() throw() { };
    const char * what() const throw() {
        return "ERROR! Invalid size of Static CVector. Using Static CVector with zero size is prohibited.";
    }
};


class Vect_Err_static_resize: public CException_vect {
public:
    Vect_Err_static_resize() throw() { }
    ~Vect_Err_static_resize() throw() { };
    const char * what() const throw() {
        return "ERROR! Changing the size of Static CVector is prohibited";
    }
};


template <typename Data_T, int Data_size>
class Static {
public:
    Static();
    Static(const int size,  const Data_T& data);
    Static(const Data_T * arr, const int size_arr);
    Static(const Static<Data_T, Data_size>& stat);
    template <typename Other_T,
            template <typename Other_class_T, int Other_class_size> class Other_class,
            int Other_size>
    Static(const Other_class<Other_T, Other_size>& other);
    ~Static() throw() {
        std::cout << "deleting static\n";
    }

    template <typename Other_T,
            template <typename Other_class_T, int Other_class_size> class Other_class,
            int Other_size>
    Static<Data_T, Data_size>& operator = (const Other_class<Other_T, Other_size>& other);

    Static<Data_T, Data_size>& operator = (const Static<Data_T, Data_size>& stat);

    inline const Data_T * array(int * size_arr) const throw();
    void array(const Data_T * arr, const int size_arr);
    virtual std::string dump() const = 0;
protected:
    #ifdef DEBUG_VECTOR
    virtual void assert_ok(const char * file, const int line) const = 0;
    #endif //DEBUG_VECTOR
    VECT_OK ok() const throw();

    Data_T data_[Data_size];
    const int size_;
};

template <typename Data_T, int Data_size>
Static<Data_T, Data_size>::Static():
    size_(Data_size)
{
    std::cout << "static ctor default\n";
    if (Data_size == 0) {
        throw Vect_Err_static_size();
    }
    Data_T temp = Data_T();
    for (int i = 0; i < Data_size; i++) {
        this->data_[i] = temp;
    }
}

template <typename Data_T, int Data_size>
Static<Data_T, Data_size>::Static(const int size,  const Data_T& data):
    size_(size)
{
    std::cout << "static ctor size\n";
    if (Data_size == 0) {
        throw Vect_Err_static_size();
    }
    if (size != Data_size) {
        throw Vect_Err_ctor();
    }
    for (int i = 0; i < Data_size; i++) {
        this->data_[i] = data;
    }
}

template <typename Data_T, int Data_size>
Static<Data_T, Data_size>::Static(const Data_T * arr, const int size_arr):
    size_(Data_size)
{
    std::cout << "static ctor array\n";
    if (Data_size == 0) {
        throw Vect_Err_static_size();
    }
    if (size_arr != Data_size || arr == nullptr) {
        throw Vect_Err_ctor();
    }
    for (int i = 0; i < size_arr; i++) {
        this->data_[i] = arr[i];
    }
}

template <typename Data_T, int Data_size>
Static<Data_T, Data_size>::Static(const Static<Data_T, Data_size>& stat):
    size_(Data_size)
{
    std::cout << "static ctor copy\n";
    #ifdef DEBUG_VECTOR
    stat.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    for (int i = 0; i < Data_size; i++) {
        this->data_[i] = stat.data_[i];
    }
}

template <typename Data_T, int Data_size>
template <typename Other_T,
        template <typename Other_class_T, int Other_class_size> class Other_class,
        int Other_size>
Static<Data_T, Data_size>::Static(const Other_class<Other_T, Other_size>& other):
    size_(Data_size)
{
    std::cout << "static ctor other copy\n";
    if (Data_size == 0) {
        throw Vect_Err_static_size();
    }
    int size = 0;
    const Other_T * addit_otherdata_ptr = other.array(&size);
    if (Data_size != size) {
        throw Vect_Err_static_resize();
    }
    for (int i = 0; i < Data_size; i++) {
        this->data_[i] = addit_otherdata_ptr[i];
    }
}

template <typename Data_T, int Data_size>
template <typename Other_T,
        template <typename Other_class_T, int Other_class_size> class Other_class,
        int Other_size>
Static<Data_T, Data_size>& Static<Data_T, Data_size>::operator = (const Other_class<Other_T, Other_size>& other)
{
    std::cout << "operator = other static\n";
    assert_ok(__FILE__, __LINE__);
    int size = 0;
    const Other_T * addit_otherdata_ptr = other.array(&size);
    if (Data_size != size) {
        throw Vect_Err_static_resize();
    }
    for (int i = 0; i < Data_size; i++) {
        this->data_[i] = addit_otherdata_ptr[i];
    }
    return *this;
}

template <typename Data_T, int Data_size>
Static<Data_T, Data_size>& Static<Data_T, Data_size>::operator = (const Static<Data_T, Data_size>& stat)
{
    std::cout << "operator = same static\n";
    assert_ok(__FILE__, __LINE__);
    #ifdef DEBUG_VECTOR
    stat.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    for (int i = 0; i < Data_size; i++) {
        this->data_[i] = stat.data_[i];
    }
    return *this;
}

template <typename Data_T, int Data_size>
void Static<Data_T, Data_size>::array(const Data_T * arr, const int size_arr)
{
    assert_ok(__FILE__, __LINE__);
    if (size_arr != this->size_ || arr == nullptr) {
        throw Vect_Err_static_resize();
    }
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] = arr[i];
    }
}

template <typename Data_T, int Data_size>
inline const Data_T * Static<Data_T, Data_size>::array(int * size_arr) const throw()
{
    assert_ok(__FILE__, __LINE__);
    if (size_arr != nullptr) {
        *size_arr = this->size_;
    }
    return (const Data_T*) this->data_;
}

template <typename Data_T, int Data_size>
VECT_OK Static<Data_T, Data_size>::ok() const throw()
{
    if (this->size_ < 1) {
        return BAD;
    }
    if (this->size_ != Data_size) {
        return BAD;
    }
    if (this->data_ == nullptr) {
        return BAD;
    }
    return OK;
}




template <typename Data_T, int Data_size>
class Dynamic {
public:
    Dynamic();
    Dynamic(const int size,  const Data_T& data);
    Dynamic(const Data_T * arr, const int size_arr);
    Dynamic(const Dynamic<Data_T, Data_size>& dyn);
    template <typename Other_T,
            template <typename Other_class_T, int Other_class_size> class Other_class,
            int Other_size>
    Dynamic(const Other_class<Other_T, Other_size>& other);
    ~Dynamic() throw();

    template <typename Other_T,
            template <typename Other_class_T, int Other_class_size> class Other_class,
            int Other_size>
    Dynamic<Data_T, Data_size>& operator = (const Other_class<Other_T, Other_size>& other);

    Dynamic<Data_T, Data_size>& operator = (const Dynamic<Data_T, Data_size>& dyn);


    inline const Data_T * array(int * size_arr) const throw();
    void array(const Data_T * arr, const int size_arr);
    void resize(const int size, const Data_T& data = Data_T());
    virtual std::string dump() const = 0;
protected:
    #ifdef DEBUG_VECTOR
    virtual void assert_ok(const char * file, const int line) const = 0;
    #endif //DEBUG_VECTOR
    VECT_OK ok() const throw();

    Data_T * data_;
    int size_;
};

template <typename Data_T, int Data_size>
Dynamic<Data_T, Data_size>::Dynamic():
    data_(nullptr),
    size_(Data_size)
{
    std::cout << "dynamic ctor default\n";
    if (Data_size < 0) {
        throw Vect_Err_ctor();
    } else if (Data_size == 0) {
        ;
    } else {
        Data_T temp = Data_T();
        this->data_ = new Data_T[Data_size];
        for (int i = 0; i < Data_size; i++) {
            this->data_[i] = temp;
        }
    }
}

template <typename Data_T, int Data_size>
Dynamic<Data_T, Data_size>::Dynamic(const int size,  const Data_T& data):
    data_(nullptr),
    size_(size)
{
    std::cout << "dynamic ctor size\n";
    if (size < 0 || Data_size < 0) {
        throw Vect_Err_ctor();
    } else if (size == 0) {
        ;
    } else {
        this->data_ = new Data_T[size];
        for (int i = 0; i < size; i++) {
            this->data_[i] = data;
        }
    }
}

template <typename Data_T, int Data_size>
Dynamic<Data_T, Data_size>::Dynamic(const Data_T * arr, const int size_arr):
    data_(nullptr),
    size_(size_arr)
{
    std::cout << "dynamic ctor array\n";
    if (size_arr < 0 || arr == nullptr || Data_size < 0) {
        throw Vect_Err_ctor();
    } else if (size_arr == 0) {
        ;
    } else {
        this->data_ = new Data_T[size_arr];
        for (int i = 0; i < size_arr; i++) {
            this->data_[i] = arr[i];
        }
    }
}

template <typename Data_T, int Data_size>
Dynamic<Data_T, Data_size>::Dynamic(const Dynamic<Data_T, Data_size>& dyn):
    data_(nullptr),
    size_(0)
{
    std::cout << "dynamic ctor copy\n";
    #ifdef DEBUG_VECTOR
    dyn.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (dyn.data_ == nullptr) {
        ;
    } else {
        this->data_ = new Data_T[dyn.size_];
        for (int i = 0; i < dyn.size_; i++) {
            this->data_[i] = dyn.data_[i];
        }
        this->size_ = dyn.size_;
    }
}

template <typename Data_T, int Data_size>
template <typename Other_T,
        template <typename Other_class_T, int Other_class_size> class Other_class,
        int Other_size>
Dynamic<Data_T, Data_size>::Dynamic(const Other_class<Other_T, Other_size>& other):
    data_(nullptr),
    size_(0)
{
    std::cout << "dynamic ctor other copy\n";
    int size = 0;
    const Other_T * addit_otherdata_ptr = other.array(&size);
    if (addit_otherdata_ptr == nullptr) {
        ;
    } else {
        this->data_ = new Data_T[size];
        for (int i = 0; i < size; i++) {
            this->data_[i] = addit_otherdata_ptr[i];
        }
        this->size_ = size;
    }
}

template <typename Data_T, int Data_size>
Dynamic<Data_T, Data_size>::~Dynamic() throw()
{
    delete [] this->data_;
    this->data_ = nullptr;
    this->size_ = -1;
    std::cout << "deleting dynamic" << std::endl;
}

template <typename Data_T, int Data_size>
template <typename Other_T,
        template <typename Other_class_T, int Other_class_size> class Other_class,
        int Other_size>
Dynamic<Data_T, Data_size>& Dynamic<Data_T, Data_size>::operator =
        (const Other_class<Other_T, Other_size>& other)
{
    std::cout << "operator = other dynamic\n";
    assert_ok(__FILE__, __LINE__);
    int size = 0;
    const Other_T * addit_otherdata_ptr = other.array(&size);
    if (addit_otherdata_ptr == nullptr) {
        delete [] this->data_;
        this->data_ = nullptr;
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
    this->size_ = size;
    return *this;
}

template <typename Data_T, int Data_size>
Dynamic<Data_T, Data_size>& Dynamic<Data_T, Data_size>::operator = (const Dynamic<Data_T, Data_size>& dyn)
{
    std::cout << "operator = same dynamic\n";
    assert_ok(__FILE__, __LINE__);
    #ifdef DEBUG_VECTOR
    dyn.assert_ok(__FILE__, __LINE__);
    #endif //DEBUG_VECTOR
    if (this == &dyn) {
        return *this;
    }
    if (dyn.data_ == NULL) {
        delete [] this->data_;
        this->data_ = nullptr;
    } else if (this->size_ != dyn.size_) {
        Data_T * addit_dataptr = new Data_T[dyn.size_];
        delete [] this->data_;
        this->data_ = addit_dataptr;
        for (int i = 0; i < dyn.size_; i++) {
            this->data_[i] = dyn.data_[i];
        }
    } else {
        for (int i = 0; i < dyn.size_; i++) {
            this->data_[i] = dyn.data_[i];
        }
    }
    this->size_ = dyn.size_;
    return *this;
}

template <typename Data_T, int Data_size>
void Dynamic<Data_T, Data_size>::resize(const int size, const Data_T& data)
{
    assert_ok(__FILE__, __LINE__);
    if (size < 0) {
        throw Vect_Err_resize();
    }
    if (size == 0) {
        delete [] this->data_;
        this->data_ = nullptr;
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
    return;
}

template <typename Data_T, int Data_size>
void Dynamic<Data_T, Data_size>::array(const Data_T * arr, const int size_arr)
{
    assert_ok(__FILE__, __LINE__);
    if (size_arr < 0) {
        throw Vect_Err_ctor();
    }
    if (size_arr == 0 || arr == nullptr) {
        delete [] this->data_;
        this->data_ = nullptr;
        this->size_ = 0;
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
    }
}

template <typename Data_T, int Data_size>
inline const Data_T * Dynamic<Data_T, Data_size>::array(int * size_arr) const throw()
{
    assert_ok(__FILE__, __LINE__);
    if (size_arr != nullptr) {
        *size_arr = this->size_;
    }
    return (const Data_T*) this->data_;
}

template <typename Data_T, int Data_size>
VECT_OK Dynamic<Data_T, Data_size>::ok() const throw()
{
    if (this->size_ < 0) {
        return BAD;
    }
    if (this->size_ == 0 && this->data_ != nullptr) {
        return BAD;
    }
    if (this->size_ == 0) {
        // this->data_ == nullptr
        return OK;
    }
    //this->size_ > 0
    if (this->data_ == nullptr) {
        return BAD;
    }
    return OK;
}






template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size = 0>
class CVector : public Parent<Data_T, Data_size> {
public:
    CVector();
    CVector(const int size, const Data_T& data = Data_T() );
    CVector(const Data_T * arr, const int size_arr);
    CVector(const CVector<Data_T, Parent, Data_size>& vect);
    template <typename Other_Data_T,
            template <typename Other_Parent_T, int Other_Parent_size> class Other_Parent,
            int Other_Data_size>
    CVector(const CVector<Other_Data_T, Other_Parent, Other_Data_size>& vect);
    ~CVector() throw();


    template <typename Other_Data_T,
            template <typename Other_Parent_T, int Other_Parent_size> class Other_Parent,
            int Other_Data_size>
    inline CVector<Data_T, Parent, Data_size>& operator =
            (const CVector<Other_Data_T, Other_Parent, Other_Data_size>& vect);


    template <typename Value_T>
    inline CVector<Data_T, Parent, Data_size>& operator *= (const Value_T& value);
    template <typename Value_T>
    inline CVector<Data_T, Parent, Data_size> operator * (const Value_T& value) const;
    template <typename Value_T>
    CVector<Data_T, Parent, Data_size>& operator /= (const Value_T& value);
    template <typename Value_T>
    inline CVector<Data_T, Parent, Data_size> operator / (const Value_T& value) const;
    inline CVector<Data_T, Parent, Data_size> operator - () const;



    template <typename T, template <typename, int> class P, int Sz,
            template <typename, template <typename, int> class, int> class Vect>
    CVector<Data_T, Parent, Data_size>& operator += (const Vect<T, P, Sz>& vectfrom);

    template <typename T, template <typename, int> class P, int Sz,
            template <typename, template <typename, int> class, int> class Vect>
    inline CVector<Data_T, Parent, Data_size> operator + (const Vect<T, P, Sz>& vectfrom) const;

    template <typename T, template <typename, int> class P, int Sz,
            template <typename, template <typename, int> class, int> class Vect>
    CVector<Data_T, Parent, Data_size>& operator -= (const Vect<T, P, Sz>& vectfrom);

    template <typename T, template <typename, int> class P, int Sz,
            template <typename, template <typename, int> class, int> class Vect>
    inline CVector<Data_T, Parent, Data_size> operator - (const Vect<T, P, Sz>& vectfrom) const;

    template <typename T, template <typename, int> class P, int Sz,
            template <typename, template <typename, int> class, int> class Vect>
    Data_T operator * (const Vect<T, P, Sz>& vectfrom) const;


    inline Data_T& operator [] (const int n);
    inline CVector<Data_T, Parent, Data_size>& operator = (const CVector<Data_T, Parent, Data_size>& vect);



    inline bool isdefault() const throw();
    inline int size() const;
    std::string dump() const;
private:
    #ifdef DEBUG_VECTOR
    void assert_ok(const char * file, const int line) const;
    #endif // DEBUG_VECTOR
};





#ifndef ASSERT_VECTOR
template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
void CVector<Data_T, Parent, Data_size>::assert_ok(const char * file, const int line) const
{
    if (this->ok() == BAD) {
        std::cout << *this;
        std::cout << file << std::endl << "line == " << line << std::endl;
        assert(!"SEE FILE AND LINE IN THE TEXT ABOVE!");
    }
}
#endif // ASSERT_VECTOR

#undef ASSERT_VECTOR


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
CVector<Data_T, Parent, Data_size>::CVector():
    Parent<Data_T, Data_size>()
{
    std::cout << "ctor vector default\n";
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
CVector<Data_T, Parent, Data_size>::CVector(const int size, const Data_T& data):
    Parent<Data_T, Data_size>(size, data)
{
    std::cout << "ctor vector size\n";
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
CVector<Data_T, Parent, Data_size>::CVector(const Data_T * arr, const int size_arr):
    Parent<Data_T, Data_size> (arr, size_arr)
{
    std::cout << "ctor vector array\n";
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
CVector<Data_T, Parent, Data_size>::CVector(const CVector<Data_T, Parent, Data_size>& vect):
    Parent<Data_T, Data_size> ( *((Parent<Data_T, Data_size> *) &vect) )
{
    std::cout << "ctor vector copy\n";
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename Other_Data_T,
        template <typename Other_Parent_T, int Other_Parent_size> class Other_Parent,
        int Other_Data_size>
CVector<Data_T, Parent, Data_size>::CVector
        (const CVector<Other_Data_T, Other_Parent, Other_Data_size>& vect):
     Parent<Data_T, Data_size> ( *((Other_Parent<Other_Data_T, Other_Data_size> *) &vect) )
{
    std::cout << "ctor vector other copy\n";
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
CVector<Data_T, Parent, Data_size>::~CVector() throw()
{
    std::cout << "deleting vector" << std::endl;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
inline Data_T& CVector<Data_T, Parent, Data_size>::operator [] (const int n)
{
    assert_ok(__FILE__, __LINE__);
    if (this->data_ == nullptr) {
        Vect_Err_segment exc;
        throw exc;
    }
    if (this->size_ <= n || n < 0) {
        Vect_Err_index exc;
        throw exc;
    }
    return this->data_[n];
}

template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
inline CVector<Data_T, Parent, Data_size>& CVector<Data_T, Parent, Data_size>::operator =
        (const CVector<Data_T, Parent, Data_size>& vect)
{
    std::cout << "operator = same vector\n";
    *((Parent<Data_T, Data_size> *) this) = *((Parent<Data_T, Data_size> *) &vect);
    return *this;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename Other_Data_T,
        template <typename Other_Parent_T, int Other_Parent_size> class Other_Parent,
        int Other_Data_size>
inline CVector<Data_T, Parent, Data_size>& CVector<Data_T, Parent, Data_size>::operator =
        (const CVector<Other_Data_T, Other_Parent, Other_Data_size>& vect)
{
    std::cout << "operator = other vector\n";
    *((Parent<Data_T, Data_size> *) this) = *((Other_Parent<Other_Data_T, Other_Data_size> *) &vect);
    return *this;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
inline std::ostream& operator << (std::ostream& os, const CVector<Data_T, Parent, Data_size>& vect)
{
    os << vect.dump();
    return os;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename T, template <typename, int> class P, int Sz,
        template <typename, template <typename, int> class, int> class Vect>
CVector<Data_T, Parent, Data_size>& CVector<Data_T, Parent, Data_size>::operator +=
        (const Vect<T, P, Sz>& vectfrom)
{
    std::cout << "operator += other vector\n";
    assert_ok(__FILE__, __LINE__);
    int size = 0;
    const T * addit_otherdata_ptr = vectfrom.array(&size);
    if (this->size_ != size) {
        throw Vect_Err_oper();
    }
    if (this->size_ == 0) {
        return *this;
    }
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] += addit_otherdata_ptr[i];
    }
    return *this;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename T, template <typename, int> class P, int Sz,
        template <typename, template <typename, int> class, int> class Vect>
inline CVector<Data_T, Parent, Data_size> CVector<Data_T, Parent, Data_size>::operator +
        (const Vect<T, P, Sz>& vectfrom) const
{
    std::cout << "operator + other vector\n";
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T, Parent, Data_size> addit_vect = *this;
    addit_vect += vectfrom;
    return addit_vect;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename T, template <typename, int> class P, int Sz,
        template <typename, template <typename, int> class, int> class Vect>
CVector<Data_T, Parent, Data_size>& CVector<Data_T, Parent, Data_size>::operator -=
        (const Vect<T, P, Sz>& vectfrom)
{
    std::cout << "operator -= other vector\n";
    assert_ok(__FILE__, __LINE__);
    int size = 0;
    const T * addit_otherdata_ptr = vectfrom.array(&size);
    if (this->size_ != size) {
        throw Vect_Err_oper();
    }
    if (this->size_ == 0) {
        return *this;
    }
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] -= addit_otherdata_ptr[i];
    }
    return *this;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename T, template <typename, int> class P, int Sz,
        template <typename, template <typename, int> class, int> class Vect>
inline CVector<Data_T, Parent, Data_size> CVector<Data_T, Parent, Data_size>::operator -
        (const Vect<T, P, Sz>& vectfrom) const
{
    std::cout << "operator - other vector\n";
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T, Parent, Data_size> addit_vect = *this;
    addit_vect -= vectfrom;
    return addit_vect;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
inline CVector<Data_T, Parent, Data_size> CVector<Data_T, Parent, Data_size>::operator - () const
{
    std::cout << "unary operator - vector\n";
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T, Parent, Data_size> addit_vect = *this;
    for (int i = 0; i < addit_vect.size_; i++) {
        addit_vect.data_[i] = -addit_vect.data_[i];
    }
    return addit_vect;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename Value_T>
inline CVector<Data_T, Parent, Data_size>& CVector<Data_T, Parent, Data_size>::operator *= (const Value_T& value)
{
    assert_ok(__FILE__, __LINE__);
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] *= value;
    }
    return *this;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename Value_T>
inline CVector<Data_T, Parent, Data_size> CVector<Data_T, Parent, Data_size>::operator * (const Value_T& value) const
{
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T, Parent, Data_size> addit_vect = *this;
    addit_vect *= value;
    return addit_vect;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size,
        typename Value_T>
inline CVector<Data_T, Parent, Data_size> operator *
        (const Value_T& value, const CVector<Data_T, Parent, Data_size>& vect)
{
    std::cout << "operator value * vector\n";
    return vect * value;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename Value_T>
CVector<Data_T, Parent, Data_size>& CVector<Data_T, Parent, Data_size>::operator /= (const Value_T& value)
{
    assert_ok(__FILE__, __LINE__);
    if (value == 0) {
        throw Vect_Err_value();
    }
    for (int i = 0; i < this->size_; i++) {
        this->data_[i] /= value;
    }
    return *this;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename Value_T>
inline CVector<Data_T, Parent, Data_size> CVector<Data_T, Parent, Data_size>::operator / (const Value_T& value) const
{
    assert_ok(__FILE__, __LINE__);
    CVector<Data_T, Parent, Data_size> addit_vect = *this;
    addit_vect /= value;
    return addit_vect;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
template <typename T, template <typename, int> class P, int Sz,
            template <typename, template <typename, int> class, int> class Vect>
Data_T CVector<Data_T, Parent, Data_size>::operator * (const Vect<T, P, Sz>& vectfrom) const
{
    std::cout << "operator * scalar\n";
    assert_ok(__FILE__, __LINE__);
    int size = 0;
    const T * addit_otherdata_ptr = vectfrom.array(&size);
    if (this->size_ != size) {
        throw Vect_Err_oper();
    }
    Data_T value = 0;
    for (int i = 0; i < this->size_; i++) {
        value += (this->data_[i] * addit_otherdata_ptr[i]);
    }
    return value;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
inline bool CVector<Data_T, Parent, Data_size>::isdefault() const throw()
{
    assert_ok(__FILE__, __LINE__);
    if (this->data_ == nullptr) {
        return true;
    } else {
        return false;
    }
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
inline int CVector<Data_T, Parent, Data_size>::size() const
{
    assert_ok(__FILE__, __LINE__);
    return this->size_;
}


template <typename Data_T,
        template <typename Parent_T, int Parent_size> class Parent,
        int Data_size>
std::string CVector<Data_T, Parent, Data_size>::dump() const
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
    if (this->size_ <= 0) {
        return ostr.str();
    }
    if (this->data_ != nullptr) {
        ostr << "array {" << std::endl;
        for (int i = 0; i < this->size_; i++) {
            ostr << "[" << i << "] " << this->data_[i] << std::endl;
        }
        ostr << "}" << std::endl;
    }
    return ostr.str();
}

#endif // MY_VECTOR


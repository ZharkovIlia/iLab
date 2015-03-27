#ifndef MY_STACK_CPP
#define MY_STACK_TEMP
#include <string.h>
#include <iostream>
#include <assert.h>
#include <sstream>

typedef enum {
    ALLRIGHT,
    ERR_ALLOC_MEM,
    ERR_CONTENTS_STACK,
} ERROR_ST_T;

typedef enum {
    ST_OK,
    ST_BAD,
} ISRIGHT_ST_T;

template <typename T>
class Cstack {
    template <typename Friend_T>
    friend std::ostringstream& operator << (std::ostringstream& ostr, const Cstack<Friend_T>& st);

    public:
        Cstack();
        Cstack(const int size);
        Cstack(const Cstack<T> * st);
        ~Cstack();
        ERROR_ST_T push(const T val);
        ERROR_ST_T pop(T* val);
        ERROR_ST_T top(T* val) const;
        int size() const;
        void do_empty();
        void swap();
        void dump_cout() const;
    private:
        ISRIGHT_ST_T ok() const;
        void assert_ok(const char * file, const int line) const;
        T * data_;
        T * dataptr_;
        int szmax_;
};


#define DEBUG_STACK

#ifndef DEBUG_STACK
#define ASSERT_OK_ST
#define assert_ok(cond...);
#endif // DEBUG_STACK

#ifndef ASSERT_OK_ST

template <typename T>
void Cstack<T>::assert_ok(const char * file, const int line) const {
    if (this->ok() == ST_BAD) {
        this->dump_cout();
        std::cout << "file - " << file << std::endl << "line = " << line << std::endl;
        assert(!"SEE FILE AND LINE IN THE TEXT ABOVE");
    }
}
#endif //ASSERT_OK_ST
#undef ASSERT_OK_ST

template <typename T>
Cstack<T>::Cstack():
    data_ (new T[1]),
    dataptr_ (NULL),
    szmax_ (1)
{
    this->dataptr_ = this->data_;
    this->data_[0] = 0;
}

template <typename T>
Cstack<T>::Cstack(const int size):
    data_ (NULL),
    dataptr_ (NULL),
    szmax_ (0)
{
    if (size <= 0) {
        this->data_ = new T[1];
        this->dataptr_ = this->data_;
        this->szmax_ = 1;
    } else {
        this->data_ = new T[size];
        this->dataptr_ = this->data_;
        this->szmax_ = size;
    }
    memset(this->data_, 0, this->szmax_*sizeof(*(this->data_)));
}

template <typename T>
Cstack<T>::Cstack(const Cstack<T> * st):
    data_ (NULL),
    dataptr_ (NULL),
    szmax_ (0)
{
    if (st == NULL) {
        this->data_ = new T[1];
        this->dataptr_ = this->data_;
        this->szmax_ = 1;
        this->data_[0] = 0;
    } else {
        #ifdef DEBUG_STACK
        st->assert_ok(__FILE__, __LINE__);
        #endif // DEBUG_STACK
        this->data_ = new T[st->szmax_];
        this->dataptr_ = this->data_ + (st->dataptr_ - st->data_);
        this->szmax_ = st->szmax_;
        for (int i = 0; i < this->szmax_; i++) {
            this->data_[i] = st->data_[i];
        }
        assert_ok(__FILE__, __LINE__);
    }
}

template <typename T>
Cstack<T>::~Cstack()
{
    delete [] this->data_;
    this->data_ = NULL;
    this->dataptr_ = NULL;
    this->szmax_ = -1;
}

template <typename T>
std::ostringstream& operator << (std::ostringstream& ostr, const Cstack<T>& st)
{
    if (st.ok() == ST_BAD) {
        ostr << "Cstack [" << (void*) (&st) << "] is BAD!!!" << std::endl;
    } else {
        ostr << "Cstack [" << (void*) (&st) << "] is OK." << std::endl;
    }
    ostr << "data [" << (void*) st.data_ << "]" << std::endl;
    ostr << "dataptr [" << (void*) st.dataptr_ << "]" << std::endl;
    ostr << "szmax = " << st.szmax_ << std::endl;
    ostr << "sznow = " << (st.dataptr_ - st.data_) << std::endl;
    if (st.data_ == NULL) {
        return ostr;
    }
    if (st.szmax_ > 0) {
        ostr << "data {" << std::endl;
    }
    for (int i = 0; i < st.szmax_; i++) {
        ostr << "[" << i << "] = " << st.data_[i];
        if (i < st.dataptr_ - st.data_) {
            ostr << "*" << std::endl;
        } else {
            ostr << std::endl;
        }
    }
    if (st.szmax_ > 0) {
        ostr << "}" << std::endl;
    }
    return ostr;
}

template <typename T>
ERROR_ST_T Cstack<T>::push(const T val)
{
    assert_ok(__FILE__, __LINE__);
    if (this->dataptr_ - this->data_ == this->szmax_) {
        T * T_ptr = new T[(this->szmax_)*2];
        if (T_ptr == NULL) {
            return ERR_ALLOC_MEM;
        } else {
            for (int i = 0; i < this->szmax_; i++) {
                T_ptr[i] = this->data_[i];
            }
            delete [] this->data_;
            this->data_ = T_ptr;
            this->dataptr_ = this->data_ + this->szmax_;
            this->szmax_ *= 2;
        }
    }
    *(this->dataptr_) = val;
    this->dataptr_++;
    assert_ok(__FILE__, __LINE__);
    return ALLRIGHT;
}

template <typename T>
ERROR_ST_T Cstack<T>::pop(T* val)
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

template <typename T>
ERROR_ST_T Cstack<T>::top(T* val) const
{
    assert_ok(__FILE__, __LINE__);
    if (this->dataptr_ - this->data_ <= 0) {
        return ERR_CONTENTS_STACK;
    }
    *val = *(this->dataptr_ - 1);
    assert_ok(__FILE__, __LINE__);
    return ALLRIGHT;
}

template <typename T>
int Cstack<T>::size() const
{
    assert_ok(__FILE__, __LINE__);
    return (this->dataptr_ - this->data_);
}

template <typename T>
void Cstack<T>::do_empty()
{
    assert_ok(__FILE__, __LINE__);
    this->dataptr_ = this->data_;
    assert_ok(__FILE__, __LINE__);
}

template <typename T>
void Cstack<T>::swap()
{
    assert_ok(__FILE__, __LINE__);
    if (this->dataptr_ - this->data_ >= 2) {
        T val1;
        T val2;
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

template <typename T>
void Cstack<T>::dump_cout() const
{
    std::ostringstream ostr;
    ostr << *this;
    std::cout << ostr.str();
}

template <typename T>
ISRIGHT_ST_T Cstack<T>::ok() const
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


#endif // MY_STACK_TEMP


#ifndef MY_STACK_CPP
#define MY_STACK_CPP

typedef enum {
    ALLRIGHT,
    ERR_ALLOC_MEM,
    ERR_CONTENTS_STACK,
} ERROR_ST_T;

typedef enum {
    ST_OK,
    ST_BAD,
} ISRIGHT_ST_T;

class Cstack_double {
    public:
        Cstack_double();
        Cstack_double(const int size);
        ~Cstack_double();
        ERROR_ST_T push(const double val);
        ERROR_ST_T pop(double * val);
        ERROR_ST_T top(double * val) const;
        int size() const;
        void do_empty();
        void swap();
        void dump() const;
    private:
        ISRIGHT_ST_T ok() const;
        void assert_ok(const char * file, const int line) const;
        double * data_;
        double * dataptr_;
        int szmax_;
};



#endif // MY_STACK_CPP

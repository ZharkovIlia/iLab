#include <iostream>
#include "My_stack_temp.h"
#include <assert.h>
#include <sstream>
#include <string>
#include <fstream>


int main(void) {
    std::ostringstream ostr;
    Cstack<int> s1;
    Cstack<int> s2(1);
    /*ISRIGHT_ST_T is_right;
    ERROR_ST_T err;*/

    for (int i = 0; i < 5; i++) {
        assert(!s1.push(i));
        ostr << s1;
        assert(!s2.push(i));
        ostr << s2;
    }
    for (int i = 0; i < 5; i++) {
        int val = 0;
        assert(!s1.pop(&val));
        ostr << val << std::endl;
        ostr << s1;
    }
    for (int i = 0; i < 3; i++) {
        int val;
        s2.top(&val);
        ostr << val << std::endl;
        ostr << s2;
    }
    ostr << std::endl;
    s2.do_empty();
    s1.do_empty();
    ostr << s1;
    ostr << s2;
    ostr << "size s1 = " << s1.size() << std::endl;
    for (int i = 0; i < 10; i++) {
        s1.push(i);
    }
    ostr << "size s1 = " << s1.size() << std::endl;

    std::cout << ostr.str();
    /*Cstack<int> s3(&s1);
    s3.dump();
    Cstack<int> s4(NULL);
    s4.dump();*/

    return 0;
}


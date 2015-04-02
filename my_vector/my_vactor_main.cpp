#include <iostream>
#include "my_vector.h"

int main(void) {
    char * a = new char [2];
    a[0] = 'a';
    a[1] = 'b';
    CVector<char> arr(a, 2);
    std::cout << arr.isdefault();
    std::cout << " " << arr.status() << std::endl;
    std::cout << arr[0] << " " << arr[1] << std::endl;
    //std::cout << arr[2];
    char str[] = "df";
    std::cout << sizeof(str);
    return 0;
}

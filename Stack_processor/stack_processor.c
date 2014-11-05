#include "stack_processor.h"
#include <stdio.h>
#include <stdlib.h>
const int MAX_SZNAME_FILE = 100;

int main(void)
{
    int err = 0;
    int ch = 0, i = 0;
    char * file_fir = (char*) malloc(MAX_SZNAME_FILE*sizeof(file_fir));
    char * file_sec = (char*) malloc(MAX_SZNAME_FILE*sizeof(file_sec));

    printf("enter name of file, which contains the program\n");
    for (i = 0; (ch = getchar()) != '\n'; i++) {
        file_fir[i] = ch;
    }
    file_fir[i] = '\0';

    printf("enter name of file, where you want to compile the program\n");
    for (i = 0; (ch = getchar()) != '\n'; i++) {
        file_sec[i] = ch;
    }
    file_sec[i] = '\0';

    err = Stack_assembler(file_fir, file_sec);
    if (err == -1) {
        printf("compilation error");
        return 0;
    } else if (err == 2) {
        printf("no such file or directory");
        return 0;
    }
    printf("compilation has finished successfully\n");
    printf("result of your program:\n");
    err = Stack_processor(file_sec);
    if (err == 1) {
        printf("there are some problems with function realloc");
        return 0;
    }

    printf("enter name of file, which you want to recover from the sequence of numbers\n");
    for (i = 0; (ch = getchar()) != '\n'; i++) {
        file_fir[i] = ch;
    }
    file_fir[i] = '\0';

    printf("enter name of file, where you want to write original codes\n");
    for (i = 0; (ch = getchar()) != '\n'; i++) {
        file_sec[i] = ch;
    }
    file_sec[i] = '\0';

    err = Stack_disassembler(file_fir, file_sec);
    if (err == 3) {
        printf("you offer to Stack_disassembler invalid file");
        return 0;
    } else if (err == 2) {
        printf("no such file or directory");
        return 0;
    }
    printf("Stack_disassembler has finished successfully\n");
    return 0;
}

#include "stack_processor.h"
#include "my_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#undef assert
#define assert(cond)

const int MAX_SZNAME_LINE = 100;
const int MAX_SZDATA_OF_LABEL = 100;
const int MAX_SZDATA_OF_REGISTRS = 4;
const int ERR_COMPILATION = -1;
const int ERR_REALLOC = 1;
const int ERR_OPEN_FILE = 2;
const int ERR_DISASSEMBLER = 3;

void Print_name_oper(const int operation)
{
    switch (operation) {
        case 1:
            printf("push st");
            break;
        case 2:
            printf("pop st");
            break;
        case 3:
            printf("add");
            break;
        case 4:
            printf("mul");
            break;
        case 5:
            printf("cmp");
            break;
        case 6:
            printf("JMP lb");
            break;
        case 7:
            printf("out");
            break;
        case 8:
            printf("end");
            break;
        case 9:
            printf("pop to");
            break;
        case 10:
            printf("JMP st");
            break;
        case 11:
            printf("JNZ");
            break;
        case 12:
            printf("JZ");
            break;
        case 13:
            printf("push from");
        default:
            printf("no such name of operation");
            break;
    }
}

int Num_one_oper(const int operation)
{
    switch (operation) {
        case 1:
            return 2;
        case 2:
            return 1;
        case 3:
            return 1;
        case 4:
            return 1;
        case 5:
            return 1;
        case 6:
            return 2;
        case 7:
            return 1;
        case 8:
            return 1;
        case 9:
            return 2;
        case 10:
            return 1;
        case 11:
            return 2;
        case 12:
            return 2;
        case 13:
            return 2;
    }
    return 0;
}

int Stack_assembler(const char * str_file_in, const char * str_file_out)
{
    FILE * f_in = fopen(str_file_in, "r");
    if (f_in == NULL) {
        return ERR_OPEN_FILE;
    }
    FILE * f_out = fopen(str_file_out, "w");
    int i = 0, num_oper = 0, num_label = 0, num_reg = 0, is_empty_line = 0;
    double number = 0;
    int i_label = 0, i_reg = 0;
    int * data_label = (int *) malloc(MAX_SZDATA_OF_LABEL*sizeof(*data_label));
    char ** data_reg = (char**)malloc(MAX_SZDATA_OF_REGISTRS*sizeof(*data_reg));
    char ** data_strlabel = (char**) malloc(MAX_SZDATA_OF_LABEL*sizeof(*data_strlabel));
    char ch = 0;
    char *pstr1 = NULL, *pstr2 = NULL;
    char *oper = (char*) malloc(MAX_SZNAME_LINE*sizeof(*oper));

    for (i = 0; i < MAX_SZDATA_OF_LABEL; i++) {
        data_strlabel[i] = (char*) malloc(MAX_SZNAME_LINE*sizeof(*(data_strlabel[i])));
    }
    for (i = 0; i < MAX_SZDATA_OF_REGISTRS; i++) {
        data_reg[i] = (char*) malloc(MAX_SZNAME_LINE*sizeof(*(data_reg[i])));
    }

    do {
        i = 0;
        while (fscanf(f_in, "%c", &ch) == 1) {
            if (ch == '\n') {
                oper[i++] = '\0';
                break;
            } else {
                oper[i++] = ch;
            }
        }
        if (i == 0) {
            return ERR_COMPILATION;
        } else {
            is_empty_line = 1;
            for (i = 0; oper[i] != '\0'; i++) {
                if (oper[i] != ' ' && oper[i] != '\t') {
                    is_empty_line = 0;
                }
            }
        }
        if (!is_empty_line) {
            Del_useless_space(oper);

            if ((pstr1 = strchr(oper, ':')) != NULL && (pstr2 = strchr(oper, ' ')) != NULL && pstr1 < pstr2) {
                strncpy(data_strlabel[num_label], oper, pstr1 - oper);
                data_strlabel[num_label][pstr1-oper] = '\0';
                data_label[num_label] = num_oper;
                num_label++;
                strcpy(oper, pstr2+1);
            }
            if ((pstr1 = strchr(oper, ' ')) != NULL) {
                if ((pstr2 = strchr(oper, ':')) != NULL) {
                    if (strncmp(oper, "JMP", 3) == 0) {
                        num_oper += Num_one_oper(6);
                    } else if (strncmp(oper, "JNZ", 3) == 0) {
                        num_oper += Num_one_oper(11);
                    } else if (strncmp(oper, "JZ", 2) == 0) {
                        num_oper += Num_one_oper(12);
                    } else {
                        assert(0); // variant is impossible
                        return ERR_COMPILATION;
                    }
                } else {
                    if ( sscanf(pstr1, "%lf", &number) != 0) {
                        num_oper += 2;
                    } else {
                        for (i = 0; i < num_reg && strcmp(data_reg[i], pstr1+1) != 0; i++);
                        if (i == num_reg) {
                            strcpy(data_reg[num_reg++], pstr1+1);
                        }
                        num_oper += 2;
                    }
                }
            } else {
                if (strcmp(oper, "pop") == 0) {
                    num_oper += Num_one_oper(2);
                } else if (strcmp(oper, "add") == 0) {
                    num_oper += Num_one_oper(3);
                } else if (strcmp(oper, "mul") == 0) {
                    num_oper += Num_one_oper(4);
                } else if (strcmp(oper, "cmp") == 0) {
                    num_oper += Num_one_oper(5);
                } else if (strcmp(oper, "out") == 0) {
                    num_oper += Num_one_oper(7);
                } else if (strcmp(oper, "end") == 0) {
                    num_oper += Num_one_oper(8);
                } else if (strcmp(oper, "JMP") == 0) {
                    num_oper += Num_one_oper(10);
                } else {
                    assert(0); //variant is impossible
                    return ERR_COMPILATION;
                }
            }
        }
    } while (strcmp(oper, "end") != 0 );

    fclose(f_in);

    f_in = fopen(str_file_in, "r");
    do {
        i = 0;
        while (fscanf(f_in, "%c", &ch) == 1) {
            if (ch == '\n') {
                oper[i++] = '\0';
                break;
            } else {
                oper[i++] = ch;
            }
        }
        if (i == 0) {
            return ERR_COMPILATION;
        } else {
            is_empty_line = 1;
            for (i = 0; oper[i] != '\0'; i++) {
                if (oper[i] != ' ' && oper[i] != '\t') {
                    is_empty_line = 0;
                }
            }
        }
        if (!is_empty_line) {

            Del_useless_space(oper);

            if ((pstr1 = strchr(oper, ':')) != NULL && (pstr2 = strchr(oper, ' ')) != NULL && pstr1 < pstr2) {
                strcpy(oper, pstr2+1);
            }
            if ((pstr1 = strchr(oper, ' ')) != NULL) {
                if ((pstr2 = strchr(oper, ':')) != NULL) {
                    for (i_label = 0; strcmp(data_strlabel[i_label], pstr2+1) != 0; i_label++);
                    if (strncmp(oper, "JMP", 3) == 0) {
                        fprintf(f_out, "%d\n", 6);
                        fprintf(f_out, "%d\n", data_label[i_label]);
                    } else if (strncmp(oper, "JNZ", 3) == 0) {
                        fprintf(f_out, "%d\n", 11);
                        fprintf(f_out, "%d\n", data_label[i_label]);
                    } else if (strncmp(oper, "JZ", 2) == 0) {
                        fprintf(f_out, "%d\n", 12);
                        fprintf(f_out, "%d\n", data_label[i_label]);
                    } else {
                        assert(0); //variant is impossible
                        return ERR_COMPILATION;
                    }
                } else {
                    if ( sscanf(pstr1, "%lf", &number) != 0) {
                        if (strncmp(oper, "push", 4) == 0) {
                            fprintf(f_out, "%d\n", 1);
                            fprintf(f_out, "%lf\n", number);
                        } else {
                            assert(0); // variant is impossible
                            return ERR_COMPILATION;
                        }
                    } else {
                        for (i_reg = 0; strcmp(pstr1+1, data_reg[i_reg]) != 0; i_reg++);
                        if (strncmp(oper, "pop", 3) == 0) {
                            fprintf(f_out, "%d\n", 9);
                            fprintf(f_out, "%d\n", i_reg);
                        } else if (strncmp(oper, "push", 4) == 0) {
                            fprintf(f_out, "%d\n", 13);
                            fprintf(f_out, "%d\n", i_reg);
                        } else {
                            assert(0); // variant is impossible
                            return ERR_COMPILATION;
                        }
                    }
                }
            } else {
                if (strcmp(oper, "pop") == 0) {
                    fprintf(f_out, "%d\n", 2);
                } else if (strcmp(oper, "add") == 0) {
                    fprintf(f_out, "%d\n", 3);
                } else if (strcmp(oper, "mul") == 0) {
                    fprintf(f_out, "%d\n", 4);
                } else if (strcmp(oper, "cmp") == 0) {
                    fprintf(f_out, "%d\n", 5);
                } else if (strcmp(oper, "out") == 0) {
                    fprintf(f_out, "%d\n", 7);
                } else if (strcmp(oper, "end") == 0) {
                    fprintf(f_out, "%d\n", 8);
                } else if (strcmp(oper, "JMP") == 0) {
                    fprintf(f_out, "%d\n", 10);
                } else {
                    assert(0); // variant is impossible
                    return ERR_COMPILATION;
                }
            }
        }
    } while (strcmp(oper, "end") != 0);

    fclose(f_in);
    fclose(f_out);
    free(data_label);
    for (i = 0; i < num_reg; i++) {
        free(data_reg[i]);
    }
    free(data_reg);
    for (i = 0; i < num_label; i++) {
        free(data_strlabel[i]);
    }
    free(data_strlabel);
    free(oper);
    return 0;
}

void Del_useless_space(char* s)
{
    char * nows = (char*) malloc(MAX_SZNAME_LINE*sizeof(*nows));
    int i = 0, lens = 0, lennows = 0;
    int pos = 0;
    lens = strlen(s);
    for (i = 0; i < lens; i++) {
        switch (pos) {
            case 0:
                if (s[i] != ' ') {
                    pos = 1;
                    nows[lennows++] = s[i];
                }
                break;
            case 1:
                if (s[i] == ' ') {
                    pos = 0;
                    nows[lennows++] = s[i];
                } else {
                    nows[lennows++] = s[i];
                }
                break;
            default:
                assert(0); // variant is impossible
                break;
        }
    }
    if (nows[lennows-1] == ' ') {
        nows[lennows-1] = '\0';
    } else {
        nows[lennows] = '\0';
    }
    strcpy(s, nows);
    free(nows);
}

int Stack_processor(const char* str_file)
{
    FILE * f_in = fopen(str_file, "r");
    double * data_oper = (double*)malloc(sizeof(*data_oper));
    double * data_reg = (double*) malloc(MAX_SZDATA_OF_REGISTRS*sizeof(*data_reg));
    double * pdoub = NULL;
    double num1 = 0, num2 = 0;
    int num_oper = 0, num_oper_max = 1;
    int position = 0;
    my_stack_double *stack = my_stack_double_new(1);

    while (fscanf(f_in, "%lf", &data_oper[num_oper]) == 1) {
        num_oper++;
        if (num_oper == num_oper_max) {
            pdoub = (double*) realloc(data_oper, sizeof(*pdoub)*num_oper_max*2);
            if (pdoub == NULL) {
                return ERR_REALLOC;
            } else {
                data_oper = pdoub;
                num_oper_max *= 2;
            }
        }
    }
    position = 0;
    while ((int)data_oper[position] != 8) {
        switch ((int)data_oper[position]) {
            case 1:
                my_stack_double_push(stack, data_oper[position+1]);
                position += Num_one_oper(1);
                break;
            case 2:
                my_stack_double_pop(stack);
                position += Num_one_oper(2);
                break;
            case 3:
                num1 = my_stack_double_pop(stack);
                num2 = my_stack_double_pop(stack);
                my_stack_double_push(stack, num1+num2);
                position += Num_one_oper(3);
                break;
            case 4:
                num1 = my_stack_double_pop(stack);
                num2 = my_stack_double_pop(stack);
                my_stack_double_push(stack, num1*num2);
                position += Num_one_oper(4);
                break;
            case 5:
                num1 = my_stack_double_pop(stack);
                num2 = my_stack_double_pop(stack);
                my_stack_double_push(stack, num2);
                my_stack_double_push(stack, num1);
                if (num1 > num2) {
                    my_stack_double_push(stack, 1);
                } else if (num2 == num1) {
                    my_stack_double_push(stack, 0);
                } else {
                    my_stack_double_push(stack, -1);
                }
                position += Num_one_oper(5);
                break;
            case 6:
                position = (int)data_oper[position+1];
                break;
            case 7:
                num1 = my_stack_double_pop(stack);
                printf("%.5lf\n", num1);
                position += Num_one_oper(7);
                break;
            case 8:
                assert(0); // variant is impossible
                break;
            case 9:
                data_reg[(int)data_oper[position+1]] = my_stack_double_pop(stack);
                position += Num_one_oper(9);
                break;
            case 10:
                position = (int)my_stack_double_top(stack);
                break;
            case 11:
                if ((int)my_stack_double_top(stack) == 0) {
                    position += Num_one_oper(11);
                } else {
                    position = (int)data_oper[position+1];
                }
                break;
            case 12:
                if ((int)my_stack_double_top(stack) == 0) {
                    position = (int)data_oper[position+1];
                } else {
                    position += Num_one_oper(12);
                }
                break;
            case 13:
                my_stack_double_push(stack, data_reg[(int)data_oper[position+1]]);
                position += Num_one_oper(13);
                break;
            default:
                assert(0); // variant is impossible
                break;
        }
    }
    my_stack_double_delete(stack);
    fclose(f_in);
    free(data_oper);
    free(data_reg);
    return 0;
}

int Stack_disassembler(const char * str_file_in, const char * str_file_out)
{
    FILE * f_in = fopen(str_file_in, "r");
    if (f_in == NULL) {
        return ERR_OPEN_FILE;
    }
    FILE * f_out = fopen(str_file_out, "w");
    int num_oper = 0, num_str_oper = 0, num_str_oper_max = 1, num_label = 0;
    int i = 0, j = 0;
    double additional_doub = 0, ind_oper = 0;
    int * data_pos_oper = (int*) malloc(sizeof(*data_pos_oper));
    int * data_ind_label = (int*) malloc(sizeof(*data_ind_label)*MAX_SZDATA_OF_LABEL);
    int * data_flag_oper;
    int * pint = NULL;

    while (fscanf(f_in, "%lf", &ind_oper) == 1) {
        switch ((int)ind_oper) {
            case 1:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(1);
                fscanf(f_in, "%lf", &additional_doub);
                break;
            case 2:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(2);
                break;
            case 3:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(3);
                break;
            case 4:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(4);
                break;
            case 5:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(5);
                break;
            case 6:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(6);
                fscanf(f_in, "%lf", &additional_doub);
                for (i = 0; i < num_label && data_ind_label[i] != (int)additional_doub; i++);
                if (i == num_label) {
                    data_ind_label[num_label++] = (int)additional_doub;
                }
                break;
            case 7:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(7);
                break;
            case 8:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(8);
                break;
            case 9:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(9);
                fscanf(f_in, "%lf", &additional_doub);
                break;
            case 10:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(10);
                break;
            case 11:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(11);
                fscanf(f_in, "%lf", &additional_doub);
                for (i = 0; i < num_label && data_ind_label[i] != (int)additional_doub; i++);
                if (i == num_label) {
                    data_ind_label[num_label++] = (int)additional_doub;
                }
                break;
            case 12:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(12);
                fscanf(f_in, "%lf", &additional_doub);
                for (i = 0; i < num_label && data_ind_label[i] != (int)additional_doub; i++);
                if (i == num_label) {
                    data_ind_label[num_label++] = (int)additional_doub;
                }
                break;
            case 13:
                data_pos_oper[num_str_oper++] = num_oper;
                num_oper += Num_one_oper(13);
                fscanf(f_in, "%lf", &additional_doub);
                break;
            default:
                return ERR_DISASSEMBLER;
                break;
        }

        if (num_str_oper == num_str_oper_max) {
            pint = (int*) realloc(data_pos_oper, sizeof(*pint)*num_str_oper_max*2);
            if (pint == NULL) {
                assert(0);
                return ERR_REALLOC;
            } else {
                data_pos_oper = pint;
                num_str_oper_max *= 2;
            }
        }
    }
    data_flag_oper = (int*)malloc(num_str_oper*sizeof(*data_flag_oper));
    for (i = 0; i < num_str_oper; i++) {
        data_flag_oper[i] = -1;
    }
    for (i = 0; i < num_label; i++) {
        for (j = 0; j < num_str_oper; j++) {
            if (data_ind_label[i] == data_pos_oper[j]) {
                data_flag_oper[j] = i;
            }
        }
    }

    fclose(f_in);

    f_in = fopen(str_file_in, "r");
    num_str_oper = 0;
    while (fscanf(f_in, "%lf", &ind_oper) == 1) {
        if (data_flag_oper[num_str_oper] != -1) {
            fprintf(f_out, "lb%d: ", data_flag_oper[num_str_oper]);
        }
        switch ((int)ind_oper) {
            case 1:
                fscanf(f_in, "%lf", &additional_doub);
                fprintf(f_out, "push %lf\n", additional_doub);
                break;
            case 2:
                fprintf(f_out, "pop\n");
                break;
            case 3:
                fprintf(f_out, "add\n");
                break;
            case 4:
                fprintf(f_out, "mul\n");
                break;
            case 5:
                fprintf(f_out, "cmp\n");
                break;
            case 6:
                fscanf(f_in, "%lf", &additional_doub);
                for (i = 0; data_ind_label[i] != (int)additional_doub; i++);
                fprintf(f_out, "JMP :lb%d\n", i);
                break;
            case 7:
                fprintf(f_out, "out\n");
                break;
            case 8:
                fprintf(f_out, "end\n");
                break;
            case 9:
                fscanf(f_in, "%lf", &additional_doub);
                fprintf(f_out, "pop REGISTR_%d\n", (int)additional_doub);
                break;
            case 10:
                fprintf(f_out, "JMP\n");
                break;
            case 11:
                fscanf(f_in, "%lf", &additional_doub);
                for (i = 0; data_ind_label[i] != (int)additional_doub; i++);
                fprintf(f_out, "JNZ :lb%d\n", i);
                break;
            case 12:
                fscanf(f_in, "%lf", &additional_doub);
                for (i = 0; data_ind_label[i] != (int)additional_doub; i++);
                fprintf(f_out, "JZ :lb%d\n", i);
                break;
            case 13:
                fscanf(f_in, "%lf", &additional_doub);
                fprintf(f_out, "push REGISTR_%d\n", (int)additional_doub);
                break;
            default:
                return ERR_DISASSEMBLER;
                break;
        }
        num_str_oper++;
    }
    free(data_flag_oper);
    free(data_pos_oper);
    free(data_ind_label);
    fclose(f_in);
    fclose(f_out);
    return 0;
}




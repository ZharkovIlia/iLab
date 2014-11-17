#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
const unsigned int NUMBER_VALUE_HASH_FUNC = 100;
const int MAX_LEN_WORD = 100;
typedef enum {
    NOW_LETTER,
    NOW_NOT_LETTER,
} POSITION_T;

int Str_insert (void * data_to, const void * data_from);
int Str_delete (void * data_del);
int Str_cmp (const void * data1, const void * data2, int * result);
int Str_dump (const void * data_dump);
unsigned int HASH_FUNC (const char * s);

int main(void)
{
    List_t ** list = (List_t**) malloc(NUMBER_VALUE_HASH_FUNC*sizeof(*list));
    FILE * f_in;
    char file_str[100];
    char * word = (char*) malloc(MAX_LEN_WORD*sizeof(*word));
    char ch = 0;
    int i = 0, len_word = 0;
    POSITION_T pos = NOW_NOT_LETTER;

    for (i = 0; i < NUMBER_VALUE_HASH_FUNC; i++) {
        list[i] = List_new(Str_cmp, Str_insert, Str_delete, sizeof(char*), "sort");
    }
    printf("enter name of file, which contains the text\n");
    for (i = 0; (ch = getchar()) != '\n'; i++) {
        file_str[i] = ch;
    }
    file_str[i] = '\0';

    f_in = fopen(file_str, "r");
    while (fread(&ch, sizeof(char), 1, f_in)) {
        switch (pos) {
            case NOW_NOT_LETTER:
                if ((0 <= ch - 'a' && ch - 'a' <= 25) || (0 <= ch - 'A' && ch - 'A' <= 25)) {
                    pos = NOW_LETTER;
                    len_word = 1;
                    word[0] = ch;
                } else {
                    ;
                }
                break;
            case NOW_LETTER:
                if ((0 <= ch - 'a' && ch - 'a' <= 25) || (0 <= ch - 'A' && ch - 'A' <= 25)) {
                    word[len_word++] = ch;
                } else {
                    pos = NOW_NOT_LETTER;
                    word[len_word] = '\0';
                    List_insert_sort(list[HASH_FUNC(word)], (void*) &word);
                }
                break;
            default: break;
        }
    }
    for (i = 0; i < NUMBER_VALUE_HASH_FUNC; i++) {
        List_dump(list[i], Str_dump);
    }

    // if I improve List_t I will use the codes below

    /*ERROR_ENUM err = ALLRIGHT;
    List_t * list = List_new(Str_cmp, Str_insert, Str_delete, sizeof(char*), "sort");
    List_elem_t * list_elem;
    void * p = NULL;
    char * s = (char*) malloc(10*sizeof(char));
    int res_del = 0;
    int count_of_elem = 0;

    p = &s;
    s = "SDF";
    List_insert_sort(list, p);
    List_assert(list, Str_dump, __LINE__, __FILE__);
   // err = List_dump(list, Str_dump);
    s = "SEF";
    List_insert_sort(list, p);
    List_assert(list, Str_dump, __LINE__, __FILE__);
    //err = List_dump(list, Str_dump);
    s = "SCF";
    List_insert_sort(list, p);
    List_assert(list, Str_dump, __LINE__, __FILE__);
   // err = List_dump(list, Str_dump);
    s = "SDF";
    List_insert_sort(list, p);
    List_assert(list, Str_dump, __LINE__, __FILE__);
   // err = List_dump(list, Str_dump);
    s = "SDP";
    List_insert_sort(list, p);
    List_assert(list, Str_dump, __LINE__, __FILE__);

    err = List_dump(list, Str_dump);

    err = List_find_elem_contents(list, p, &list_elem, &count_of_elem);
    if (list_elem) printf("data = %s\n, count = %d\n", *((char**)(list_elem->data)), count_of_elem);
    else printf("NO\n");

    s = "SDF";
    err = List_find_elem_contents(list, p, &list_elem, &count_of_elem);
    if (list_elem) printf("data = %s\n, count = %d\n", *((char**)(list_elem->data)), count_of_elem);
    else printf("NO\n");

    s = "sdf";
    err = List_find_elem_contents(list, p, &list_elem, &count_of_elem);
    if (list_elem) printf("data = %s\n, count = %d\n", *((char**)(list_elem->data)), count_of_elem);
    else printf("NO\n");

    s = "SDP";
    err = List_delete_elem_if_exist(list, p, 0, &res_del);
    err = List_dump(list, Str_dump);

    s = "SEF";
    err = List_delete_elem_if_exist(list, p, 0, &res_del);
    err = List_dump(list, Str_dump);

    s = "sdf";
    err = List_delete_elem_if_exist(list, p, 0, &res_del);
    err = List_dump(list, Str_dump);

    s = "SDF";
    err = List_delete_elem_if_exist(list, p, 0, &res_del);
    printf("res_del = %d\n", res_del);
    err = List_dump(list, Str_dump);

    s = "SCF";
    err = List_delete_elem_if_exist(list, p, 0, &res_del);
    err = List_dump(list, Str_dump);

    err = List_delete(list);
    //printf("%d\n", err);*/

    for (i = 0; i < NUMBER_VALUE_HASH_FUNC; i++) {
        List_delete(list[i]);
    }
    free(list);
    free(word);
    return 0;
}

unsigned int HASH_FUNC (const char * s)
{
    unsigned int hash = 0;
    for (; *s; s++)
    {
        hash += *s;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % NUMBER_VALUE_HASH_FUNC;
}

int Str_insert(void * data_to, const void * data_from)
{
    int len = strlen(*((char**) data_from));
    *((char**) data_to) = (char*) malloc((len+1)*sizeof(char));
    strcpy(*((char**) (data_to)), *((char**) data_from));
    return 0;
}

int Str_cmp(const void * data1, const void * data2, int * result)
{
    *result = strcmp(*((char**) data1), *((char**) data2));
    return 0;
}


int Str_delete(void * data_del)
{
    free(*((char**) data_del));
    *((char**) data_del) = NULL;
    return 0;
}

int Str_dump(const void * data_dump)
{
    if (data_dump == NULL) {
        assert(0);
    }
    printf("%s\n", *((char**) data_dump));
    return 0;
}


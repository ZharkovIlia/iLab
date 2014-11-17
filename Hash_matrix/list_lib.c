#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
//#undef assert
//#define assert(cond)


List_t * List_new (int (*Cmp_elem_func) (const void *, const void *, int *), \
                   int (*Insert_elem_func) (void *, const void *), \
                   int (*Delete_elem_func) (void *), \
                   size_t size_one_element, \
                   char * option )
{
    if (strcmp(option, "sort") != 0) {
        return NULL;
    }
    List_t * list = (List_t*) malloc(sizeof(*list));
    list->first = NULL;
    list->current = NULL;
    list->size_all = 0;
    list->size_uniq = 0;
    list->size_element = size_one_element;
    list->Cmp_elem = Cmp_elem_func;
    list->Insert_elem = Insert_elem_func;
    list->Delete_elem = Delete_elem_func;
    strcpy(list->option_list, option);
    return list;
}

int List_OK (List_t * list, size_t * real_size)
{
    int is_right = 1;
    int is_right_data = 1;
    size_t real_size_all = 0;
    int is_current_right = 0;
    List_elem_t * p_elem;
    List_elem_t * p_elem_prev = NULL;
    if (list == NULL) {
        return 0;
    }
    p_elem = list->first;
    for (*real_size = 0; p_elem != NULL && is_right; (*real_size)++) {
        is_current_right = (is_current_right || list->current == p_elem);
        real_size_all += p_elem->count;

        if ((*real_size) == 0) {
            is_right = p_elem->prev == NULL;
            is_right_data = (is_right_data && \
                            p_elem->count > 0 && \
                            p_elem->data != NULL);
            p_elem_prev = p_elem;
            p_elem = p_elem->next;
        } else {
            is_right = p_elem->prev == p_elem_prev && \
                       p_elem_prev->next == p_elem;
            is_right_data = (is_right_data && \
                            p_elem->count > 0 && \
                            p_elem->data != NULL);
            p_elem_prev = p_elem;
            p_elem = p_elem->next;
        }
    }
    if (list->size_uniq == 0 && list->current == NULL) {
        is_current_right = 1;
    }
    is_right = is_right && \
               ((*real_size) == list->size_uniq) && \
               list->size_all == real_size_all && \
               is_right_data && \
               is_current_right;
    return is_right;
}

ERROR_ENUM List_dump (List_t * list, int (*Dump_elem) (const void *))
{
    size_t real_size = 0;
    int err = 0;
    int i = 0;
    List_elem_t * p_elem;
    if (!List_OK(list, &real_size)) {
        printf("List[%p] is BAD!!!\n", list);
    } else {
        printf("List[%p] is OK.\n", list);
    }
    if (list != NULL) {
        printf("size_uniq = %d, real size = %d, size_all = %d\nfirst [%p], current [%p]\n", \
               list->size_uniq, real_size, list->size_all, list->first, list->current);
        p_elem = list->first;
        for (i = 0; i < real_size; i++) {

            if (p_elem == NULL) assert(0);

            printf("[%d] list_elem [%p], next [%p], prev [%p]\n", i, p_elem, p_elem->next, p_elem->prev);
            printf("count of data = %d\ndata [%p]\n{\n", p_elem->count, p_elem->data);
            if (p_elem->data != NULL) {
                err = Dump_elem(p_elem->data);
            } else {
                err = 0;
            }
            if (err) {
                return ERR_DUMP;
            }
            p_elem = p_elem->next;
            printf("}\n");
        }
    }
    return ALLRIGHT;
}

ERROR_ENUM List_assert (List_t * list, int (*Dump_elem) (const void *),\
                                      int number_of_line, char * file)
{
    size_t real_size;
    if (!List_OK(list, &real_size)) {
        List_dump(list, Dump_elem);
        printf("%s\nline = %d\n", file, number_of_line);
        assert(!"SEE LINE AND FILE IN THE TEXT ABOVE");
    }
    return ALLRIGHT;
}

ERROR_ENUM List_insert_sort (List_t * list, void * data_new)
{
    int res_cmp;
    List_elem_t * list_elem_new = NULL;
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    if (strcmp(list->option_list, "sort") != 0) {
        return ERR_INSERT_SORT;
    }
    if (list->current == NULL) {
        assert(!list->size_uniq); // impossible! it means that list->current != NULL
        list->first = (List_elem_t*) malloc(sizeof(*(list->first)));
        list->first->data = (void*) malloc(list->size_element);
        list->first->prev = NULL;
        list->first->next = NULL;
        if ((list->Insert_elem)(list->first->data, data_new)) {
            assert(0);
            return ERR_INSERT_SORT;
        }
        list->first->count = 1;
        list->current = list->first;
        (list->size_uniq)++;
        list->size_all++;
    } else {
        if ((list->Cmp_elem)(list->current->data, data_new, &res_cmp)) {
            assert(0);
            return ERR_INSERT_SORT;
        }
        if (res_cmp > 0) {
            while (list->current->prev != NULL) {
                list->current = list->current->prev;
                if ((list->Cmp_elem)(list->current->data, data_new, &res_cmp)) {
                    assert(0);
                    return ERR_INSERT_SORT;
                }
                if (!res_cmp) {
                    break;
                } else if (res_cmp < 0) {
                    list_elem_new = (List_elem_t*) malloc(sizeof(*list_elem_new));
                    list_elem_new->prev = list->current;
                    list_elem_new->next = list->current->next;
                    list->current->next->prev = list_elem_new;
                    list->current->next = list_elem_new;
                    list->current = list_elem_new;
                    break;
                } else {
                    ; // nothing to do
                }
            }
            if (res_cmp > 0) {
                list_elem_new = (List_elem_t*) malloc(sizeof(*list_elem_new));
                list_elem_new->prev = NULL;
                list_elem_new->next = list->current;
                list->first = list_elem_new;
                list->current->prev = list_elem_new;
                list->current = list_elem_new;
            }
        } else if (res_cmp < 0) {
            while (list->current->next != NULL) {
                list->current = list->current->next;
                if ((list->Cmp_elem)(list->current->data, data_new, &res_cmp)) {
                    assert(0);
                    return ERR_INSERT_SORT;
                }
                if (!res_cmp) {
                    break;
                } else if (res_cmp > 0) {
                    list_elem_new = (List_elem_t*) malloc(sizeof(*list_elem_new));
                    list_elem_new->prev = list->current->prev;
                    list_elem_new->next = list->current;
                    list->current->prev->next = list_elem_new;
                    list->current->prev = list_elem_new;
                    list->current = list_elem_new;
                    break;
                } else {
                    ; // nothing to do
                }
            }
            if (res_cmp < 0) {
                list_elem_new = (List_elem_t*) malloc(sizeof(*list_elem_new));
                list_elem_new->next = NULL;
                list_elem_new->prev = list->current;
                list->current->next = list_elem_new;
                list->current = list_elem_new;
            }
        }
        if (res_cmp == 0) {
            (list->current->count)++;
            list->size_all++;
        } else {
            list->current->data = (void*) malloc(list->size_element);
            if ((list->Insert_elem)(list->current->data, data_new)) {
                assert(0);
                return ERR_INSERT_SORT;
            }
            list->current->count = 1;
            (list->size_uniq)++;
            list->size_all++;
        }
    }
    return ALLRIGHT;
}

ERROR_ENUM List_find_elem_contents (List_t * list, void * data_search, List_elem_t ** result, int * count_of_elem)
{
    int res_cmp;
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    if (strcmp(list->option_list, "sort") == 0) {
        if (list->current == NULL) {
            assert(!list->size_uniq); // impossible! it means that list->current != NULL
            *result = NULL;
            *count_of_elem = 0;
            return ALLRIGHT;
        } else {
            if ((list->Cmp_elem)(list->current->data, data_search, &res_cmp)) {
                assert(0);
                return ERR_FIND_ELEM;
            }
            if (res_cmp > 0) {
                while (list->current->prev != NULL) {
                    list->current = list->current->prev;
                    if ((list->Cmp_elem)(list->current->data, data_search, &res_cmp)) {
                        assert(0);
                        return ERR_FIND_ELEM;
                    }
                    if (!res_cmp) {
                        *result = list->current;
                        *count_of_elem = list->current->count;
                        return ALLRIGHT;
                    } else if (res_cmp < 0) {
                        *result = NULL;
                        *count_of_elem = 0;
                        return ALLRIGHT;
                    } else {
                        ; // nothing to do
                    }
                }
                if (res_cmp > 0) {
                    *result = NULL;
                    *count_of_elem = 0;
                    return ALLRIGHT;
                }
            } else if (res_cmp < 0) {
                while (list->current->next != NULL) {
                    list->current = list->current->next;
                    if ((list->Cmp_elem)(list->current->data, data_search, &res_cmp)) {
                        assert(0);
                        return ERR_FIND_ELEM;
                    }
                    if (!res_cmp) {
                        *result = list->current;
                        *count_of_elem = list->current->count;
                        return ALLRIGHT;
                    } else if (res_cmp > 0) {
                        *result = NULL;
                        *count_of_elem = 0;
                        return ALLRIGHT;
                    } else {
                        ; // nothing to do
                    }
                }
                if (res_cmp < 0) {
                    *result = NULL;
                    *count_of_elem = 0;
                    return ALLRIGHT;
                }
            }
            if (res_cmp == 0) {
                *result = list->current;
                *count_of_elem = list->current->count;
                return ALLRIGHT;
            }
        }
    }
    return ALLRIGHT;
}

ERROR_ENUM List_delete_elem_if_exist (List_t * list, void * data_delete, size_t number_of_elem, int * result)
{
    int res_cmp;
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    if (strcmp(list->option_list, "sort") == 0) {
        if (list->current == NULL) {
            assert(!list->size_uniq); // impossible! it means that list->current != NULL
            *result = 0;
            return ALLRIGHT;
        } else {
            if ((list->Cmp_elem)(list->current->data, data_delete, &res_cmp)) {
                assert(0);
                return ERR_DELETE_ELEM_IF_EXIST;
            }
            if (res_cmp > 0) {
                while (list->current->prev != NULL) {
                    list->current = list->current->prev;
                    if ((list->Cmp_elem)(list->current->data, data_delete, &res_cmp)) {
                        assert(0);
                        return ERR_DELETE_ELEM_IF_EXIST;
                    }
                    if ( !res_cmp) {
                        if ( !number_of_elem || number_of_elem >= list->current->count) {
                            *result = list->current->count;
                            if (List_delete_elem_ptr(list, list->current) == ERR_DELETE_ELEM_PTR) {
                                *result = 0;
                                return ERR_DELETE_ELEM_IF_EXIST;
                            }
                            return ALLRIGHT;
                        } else {
                            list->current->count -= number_of_elem;
                            list->size_all -= number_of_elem;
                            *result = number_of_elem;
                        }
                        return ALLRIGHT;
                    } else if (res_cmp < 0) {
                        *result = 0;
                        return ALLRIGHT;
                    } else {
                        ; // nothing to do
                    }
                }
                if (res_cmp > 0) {
                    *result = 0;
                    return ALLRIGHT;
                }
            } else if (res_cmp < 0) {
                while (list->current->next != NULL) {
                    list->current = list->current->next;
                    if ((list->Cmp_elem)(list->current->data, data_delete, &res_cmp)) {
                        assert(0);
                        return ERR_DELETE_ELEM_IF_EXIST;
                    }
                    if ( !res_cmp) {
                        if ( !number_of_elem || number_of_elem >= list->current->count) {
                            *result = list->current->count;
                            if (List_delete_elem_ptr(list, list->current) == ERR_DELETE_ELEM_PTR) {
                                *result = 0;
                                return ERR_DELETE_ELEM_IF_EXIST;
                            }
                            return ALLRIGHT;
                        } else {
                            list->current->count -= number_of_elem;
                            list->size_all -= number_of_elem;
                            *result = number_of_elem;
                        }
                        return ALLRIGHT;
                    } else if (res_cmp > 0) {
                        *result = 0;
                        return ALLRIGHT;
                    } else {
                        ; // nothing to do
                    }
                }
                if (res_cmp < 0) {
                    *result = 0;
                    return ALLRIGHT;
                }
            }
            if ( !res_cmp) {
                if ( !number_of_elem || number_of_elem >= list->current->count) {
                    *result = list->current->count;
                    if (List_delete_elem_ptr(list, list->current) == ERR_DELETE_ELEM_PTR) {
                        *result = 0;
                        return ERR_DELETE_ELEM_IF_EXIST;
                    }
                    return ALLRIGHT;
                } else {
                    list->current->count -= number_of_elem;
                    list->size_all -= number_of_elem;
                    *result = number_of_elem;
                }
                return ALLRIGHT;
            }
        }
    } else {
        assert(0);
        return ERR_DELETE_ELEM_IF_EXIST;
    }
    return ALLRIGHT;
}

ERROR_ENUM List_delete_elem_ptr (List_t * list, List_elem_t * list_elem)
{
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    if ((list->Delete_elem)(list_elem->data)) {
        assert(0);
        return ERR_DELETE_ELEM_PTR;
    }
    if (list_elem == list->first) {
        if (list_elem->next != NULL) {
            list->current = list_elem->next;
        } else {
            list->current = NULL;
        }
        list->first = list_elem->next;
        if (list_elem->next != NULL) {
            list_elem->next->prev = NULL;
        }
    } else {
        list->current = list_elem->prev;
        list_elem->prev->next = list_elem->next;
        if (list_elem->next != NULL) {
            list_elem->next->prev = list_elem->prev;
        }
    }
    list->size_all -= list_elem->count;
    free(list_elem->data);
    free(list_elem);
    list->size_uniq--;
    return ALLRIGHT;
}

ERROR_ENUM List_do_empty (List_t * list)
{
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    while (list->first != NULL) {
        if (List_delete_elem_ptr(list, list->first) == ERR_DELETE_ELEM_PTR) {
            assert(0);
            return ERR_DO_EMPTY;
        }
    }
    list->current = NULL;
    list->size_uniq = 0;
    list->size_all = 0;
    return ALLRIGHT;
}

ERROR_ENUM List_delete (List_t * list)
{
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    if (List_do_empty(list) == ERR_DO_EMPTY) {
        assert(0);
        return ERR_DELETE_LIST;
    }
    free(list);
    return ALLRIGHT;
}

ERROR_ENUM List_size_uniq (List_t * list, size_t * size_list)
{
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    *size_list = list->size_uniq;
    return ALLRIGHT;
}

ERROR_ENUM List_size_all (List_t * list, size_t * size_list)
{
    //List_assert(list, Str_dump_1, __LINE__, __FILE__);
    *size_list = list->size_all;
    return ALLRIGHT;
}

// It was necessarily to check!
int Str_dump_1 (const void * data_del)
{
    if (data_del == NULL) {
        assert(0);
    }
    printf("%s\n", *((char**) data_del));
    return 0;
}
// Don't use this!

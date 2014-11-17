#ifndef LIST_H
#define LIST_H
#include <stddef.h>

typedef struct List_elem {
    void * data;
    size_t count;
    struct List_elem * next;
    struct List_elem * prev;
} List_elem_t;

typedef struct {
    List_elem_t * first;
    // it is the pointer to the first element of List_t

    List_elem_t * current;
    // it is the current pointer to some element of List_t

    size_t size_uniq;
    // it is a size of unique elements in List_t
    size_t size_all;
    // it is a size of all element is List_t

    size_t size_element;
    // it is a size of one element, that is in every List_elem_t

    int (*Cmp_elem) (const void *, const void *, int *);
    // user must write a function, that compares two elements of type const void *
    /* this function records the comparison result to int *
       if the first element greater than the second, int * must contain pointer to positive value
       if the first element is less than the second, int * must contain pointer to negative value
       if the first element is equal to the second, int * must contain pointer to zero
    */
    /* if the user's function is not able to compare elements, it must return nonzero value
       else the user's function must return zero
    */

    int (*Insert_elem) (void *, const void *);
    /* user must write a function, that inserts value from
       element of type const void * to element of type void *
    */
    /* if the user's function is not able to insert one element to other,
       it must return nonzero value else the user's function must return zero
    */

    int (*Delete_elem) (void *);
    /* user must write a function, that deletes element of type void *.
       for example, user's function can free memory
    */
    /* if the user's function is not able to delete element,
       it must return nonzero value else the user's function must return zero
    */

    char option_list[10];
    /* it is one of following options of List_t:
            "sort": this List_t inserts element in order,
                    which is defined by the user's function Cmp_elem(const void *, const void *, int *)
            other options maybe will be written later
    */

} List_t; // end of typedef

typedef enum ERROR_OF_LIST {
    ALLRIGHT,
    ERR_INSERT_SORT,
    ERR_DELETE_ELEM_IF_EXIST,
    ERR_DELETE_ELEM_PTR,
    ERR_DO_EMPTY,
    ERR_FIND_ELEM,
    ERR_DELETE_LIST,
    ERR_DUMP,
} ERROR_ENUM;

List_t * List_new (int (*Cmp_elem_func) (const void*, const void*, int *), \
                   int (*Insert_elem_func) (void *, const void *), \
                   int (*Delete_elem_func) (void *), \
                   size_t size_one_element, \
                   char * option);
/* if you try to create List_t with nonexistent option,
   this function returns NULL
   else returns pointer to List_t
*/

ERROR_ENUM List_insert_sort (List_t * list, void * data_new);
/* if your List_t is "sort", you can use this function for inserting element data_new to list
   else it always returns ERR_INSERT_SORT
*/
/* if the functions, that were written by user, returns nonzero value,
   this function returns ERR_INSERT_SORT
*/

ERROR_ENUM List_find_elem_contents (List_t * list, void * data_search, List_elem_t ** result, int * count_of_elem);
// you can use this function for finding element data_search
/* if the functions, that were written by user, returns nonzero value,
   this function returns ERR_FIND_ELEM
*/
/* this function records NULL to result and records 0 to count_of_element,
   if there are no elements data_search in list,
   else records pointer to found element to result
   and records number of elements data_search in list to count_of_elem
*/

ERROR_ENUM List_delete_elem_if_exist (List_t * list, void * data_delete, size_t number_of_elem, int * result);
// you can use this function for deleting element data_delete in list
/* if the functions, that were written by user, returns nonzero value,
   this function returns ERR_DELETE_ELEM_IF_EXIST
*/
/* if there are elements data_delete in list,
   this function deletes
      1) PAY ATTENTION! all elements data_delete, if number_of_elem is zero!
      2) number_of_element elements, if the number of element data_delete in list is not less than number_of_element,
         else really existing number of elements data_delete in list
*/
// this function records number of elements, which it have deleted, to result

ERROR_ENUM List_dump (List_t * list, int (*Dump_elem) (const void *));
/* if you want to use this function(it is not necessary),
   you must write a function, that writes contents of the element of type const void *
   if the user's function is not able to write contents of the element,
   it must return nonzero value else the user's function must return zero
*/
// this function prints technical information about list
/* if the function Dump_elem, that was written by user, returns nonzero value,
   this function returns ERR_DUMP
*/

ERROR_ENUM List_delete_elem_ptr (List_t * list, List_elem_t * list_elem);
// you can use this function for deleting element *list_elem in list
/* if the functions, that were written by user, returns nonzero value,
   this function returns ERR_DELETE_ELEM_PTR
*/
// WARNING! pointer list_elem must be correct!

ERROR_ENUM List_delete (List_t * list);
// you can use this function for deleting list
/* if the functions, that were written by user, returns nonzero value,
   this function returns ERR_DELETE_LIST
*/

ERROR_ENUM List_do_empty (List_t * list);
// you can use this function for doing your list empty
/* if the functions, that were written by user, returns nonzero value,
   this function returns ERR_DO_EMPTY
*/

ERROR_ENUM List_size_uniq (List_t * list, size_t * size_list);
// this function returns number of unique elements in list

ERROR_ENUM List_size_all (List_t * list, size_t * size_list);
// this function returns number of all elements in list

int List_OK (List_t * list, size_t * real_size);
// this function checks if list is right and records real size of list to real_size
/* this function is used by List_assert and it is designed to creator of List_t,
   therefore using this function is meaningless for user
*/
/* this function returns 1 if list is right
   and returns 0 if list is wrong
*/

ERROR_ENUM List_assert (List_t * list, int (*Dump_elem) (const void *),\
                                      int number_of_line, char * file);
/* if you want to use this function(it is not necessary),
   you must write a function, that writes contents of the element of type const void *
   if the user's function is not able to write contents of the element,
   it must return nonzero value else the user's function must return zero
*/
/* this function is designed to creator of List_t,
   therefore using this function is meaningless for user
*/
/* this function calls List_OK and if list is wrong, calls List_dump
   and then fails because of assert
*/

// It was necessarily to check!
int Str_dump_1 (const void * data_dump);
// Don't use this!

#endif // LIST_H

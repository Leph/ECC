#ifndef ECC_H
#define ECC_H

enum type_t {VOID_T, INT_T, FLOAT_T, ARRAY_T, REF_T};
typedef enum type_t type_t;

struct function_t
{
    char* name;
    int nb_params;
    type_t* type_params;
    type_t type_return;
};
typedef struct function_t function_t;

struct function_table_t
{
    function_t** table;
    int size;
};
typedef struct function_table_t function_table_t;

function_table_t* create_function_table();
void delete_function_table(function_table_t* t);

function_t* create_function();
void delete_function(function_t* f);

void function_set_name(function_t* f, char* name);
void function_set_return(function_t* f, type_t type);
void function_add_param(function_t* f, type_t type);

void function_table_add_function(function_table_t* t, function_t* f);

#endif


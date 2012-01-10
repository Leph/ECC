#ifndef ECC_H
#define ECC_H

/*
 * Types
 */

enum type_t {VOID_T, INT_T, FLOAT_T};
typedef enum type_t type_t;

void type_print(type_t type);

/**
 * Variables
 */

struct variable_t
{
    char* name;
    type_t type;
    int dim;
    int* size_array;
};
typedef struct variable_t variable_t;

variable_t* create_variable();
void delete_variable(variable_t* v);
void variable_print(variable_t* v);
void variable_set_name(variable_t* v, char* name);
void variable_set_type(variable_t* v, type_t type);
void variable_add_dim(variable_t* v, int size);

/*
 * Functions
 */

struct function_t
{
    char* name;
    type_t type_return;
    int nb_params;
    variable_t** params;
};
typedef struct function_t function_t;

function_t* create_function();
void delete_function(function_t* f);
void function_print(function_t* f);
void function_set_name(function_t* f, char* name);
void function_set_return(function_t* f, type_t type);
void function_add_param(function_t* f, variable_t* v);

/**
 * Functions table
 */

struct function_table_t
{
    function_t** table;
    int size;
};
typedef struct function_table_t function_table_t;

/*
 * Fonctions definitions
 */

function_table_t* create_function_table();
void delete_function_table(function_table_t* t);

void function_table_add_function(function_table_t* t, function_t* f);

#endif


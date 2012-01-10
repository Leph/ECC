#ifndef ECC_H
#define ECC_H

/*
 * Types
 */

enum type_t {VOID_T, INT_T, FLOAT_T};
typedef enum type_t type_t;

void type_print(type_t type);

/**
 * Values
 */

enum value_type_t {IDENTIFIER_T, CONST_INT_T, CONST_FLOAT_T};
typedef enum value_type_t value_type_t;

struct value_t
{
    union {
        char* identifier;
        int const_int;
        float const_float;
    } val;
    value_type_t type;
};
typedef struct value_t value_t;

value_t* create_value_identifier(char* identifier);
value_t* create_value_const_int(int const_int);
value_t* create_value_const_float(float const_float);
void delete_value(value_t* v);
void value_print(value_t* v);

/**
 * unary_operation
 */

enum unary_operation_type_t {NONE_T, INC_OP_T, DEC_OP_T, FUNCTION_CALL_T, ARRAY_CALL_T};
typedef enum unary_operation_type_t unary_operation_type_t;

struct unary_operation_t
{
    value_t* value;
    unary_operation_type_t type;
    struct arguments_list_t* arguments;
};
typedef struct unary_operation_t unary_operation_t;

unary_operation_t* create_unary_operation_nop(value_t* v);
unary_operation_t* create_unary_operation_inc(value_t* v);
unary_operation_t* create_unary_operation_dec(value_t* v);
unary_operation_t* create_unary_operation_func(value_t* v, struct arguments_list_t* l);
unary_operation_t* create_unary_operation_array(value_t* v, struct arguments_list_t* l);
void delete_unary_operation(unary_operation_t* op);
void unary_operation_print(unary_operation_t* op);

/**
 * Operations
 */

enum operation_type_t {NOP_T, ADD_T, SUB_T, MULT_T};
typedef enum operation_type_t operation_type_t;

struct operation_t
{
    unary_operation_t* op1;
    unary_operation_t* op2;
    operation_type_t type;
};
typedef struct operation_t operation_t;

operation_t* create_operation_nop(unary_operation_t* op1);
operation_t* create_operation_add(unary_operation_t* op1, unary_operation_t* op2);
operation_t* create_operation_sub(unary_operation_t* op1, unary_operation_t* op2);
operation_t* create_operation_mult(unary_operation_t* op1, unary_operation_t* op2);
void delete_operation(operation_t* op);
void operation_print(operation_t* op);

/**
 * arguments_list
 */

struct arguments_list_t
{
    operation_t** operations;
    int size;
};
typedef struct arguments_list_t arguments_list_t;

arguments_list_t* create_arguments_list();
void delete_arguments_list(arguments_list_t* l);
void arguments_list_print(arguments_list_t* l);
void arguments_list_add_arg(arguments_list_t* l, operation_t* op);

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

/**
 * variables table
 */

struct variable_table_t
{
    variable_t** table;
    int size;
};
typedef struct variable_table_t variable_table_t;

variable_table_t* create_variable_table();
void delete_variable_table(variable_table_t* t);
void variable_table_print(variable_table_t* t);
void variable_table_add_var(variable_table_t* t, variable_t* v);
void variable_table_set_all_type(variable_table_t* t, type_t type);
variable_table_t* variable_table_merge(variable_table_t* t1, variable_table_t* t2);

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

function_table_t* create_function_table();
void delete_function_table(function_table_t* t);
void function_table_add_function(function_table_t* t, function_t* f);

#endif


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
    char* identifier;
    int const_int;
    float const_float;
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

enum operation_type_t {NOP_T, MINUS_T, PLUS_T, ASSIGN_T, ADD_T, SUB_T, MULT_T};
typedef enum operation_type_t operation_type_t;

struct operation_t
{
    unary_operation_t* op1;
    unary_operation_t* op2;
    operation_type_t type;
};
typedef struct operation_t operation_t;

operation_t* create_operation_nop(unary_operation_t* op1);
operation_t* create_operation_minus(unary_operation_t* op1);
operation_t* create_operation_plus(unary_operation_t* op1);
operation_t* create_operation_assign(unary_operation_t* op1, unary_operation_t* op2);
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
 * Conditions
 */

enum condition_type_t {BOOL_T, L_T, G_T, LE_T, GE_T, EQ_T, NE_T};
typedef enum condition_type_t condition_type_t;

struct condition_t
{
    condition_type_t type;
    operation_t* op1;
    operation_t* op2;
    struct statement_t* statement;
};
typedef struct condition_t condition_t;

condition_t* create_condition_bool(operation_t* op1);
condition_t* create_condition_l(operation_t* op1, operation_t* op2);
condition_t* create_condition_g(operation_t* op1, operation_t* op2);
condition_t* create_condition_le(operation_t* op1, operation_t* op2);
condition_t* create_condition_ge(operation_t* op1, operation_t* op2);
condition_t* create_condition_eq(operation_t* op1, operation_t* op2);
condition_t* create_condition_ne(operation_t* op1, operation_t* op2);
void delete_condition(condition_t* c);
void condition_print(condition_t* c);
void condition_set_statement(condition_t* c, struct statement_t* s);

/**
 * Label
 */

typedef char label_t;

label_t* create_label(char* identifier);
void delete_label(label_t* l);
void label_print(label_t* l);

/**
 * Jump
 */

enum jump_type_t {GOTO_T, RETURN_T, RETURN_OP_T};
typedef enum jump_type_t jump_type_t;

struct jump_t
{
    jump_type_t type;
    operation_t* op;
    label_t* label;
};
typedef struct jump_t jump_t;

jump_t* create_jump_return();
jump_t* create_jump_return_op(operation_t* op);
jump_t* create_jump_goto(label_t* label);
void delete_jump(jump_t* j);
void jump_print(jump_t* j);

/**
 * Statements
 */

enum statement_type_t {LABEL_T, OP_T, COND_T, JMP_T, BLOCK_T};
typedef enum statement_type_t statement_type_t;

struct statement_t
{
    statement_type_t type;
    label_t* label;
    operation_t* operation;
    condition_t* condition;
    jump_t* jump;
    struct block_t* block;
};
typedef struct statement_t statement_t;

statement_t* create_statement_label(label_t* l);
statement_t* create_statement_op(operation_t* op);
statement_t* create_statement_cond(condition_t* cond);
statement_t* create_statement_jmp(jump_t* jmp);
statement_t* create_statement_block(struct block_t* block);
void delete_statement(statement_t* s);
void statement_print(statement_t* s);

/**
 * Statements table
 */

struct statement_table_t
{
    statement_t** table;
    int size;
};
typedef struct statement_table_t statement_table_t;

statement_table_t* create_statement_table();
void delete_statement_table(statement_table_t* t);
void statement_table_print(statement_table_t* t);
void statement_table_add(statement_table_t* t, statement_t* s);

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

/**
 * Block
 */

struct block_t
{
    statement_table_t* st;
    variable_table_t* vt;
};
typedef struct block_t block_t;

block_t* create_block(variable_table_t* vt, statement_table_t* st);
void delete_block(block_t* b);
void block_print(block_t* b);

/*
 * Functions
 */

struct function_t
{
    char* name;
    type_t type_return;
    int nb_params;
    variable_t** params;
    block_t* block;
};
typedef struct function_t function_t;

function_t* create_function();
void delete_function(function_t* f);
void function_print(function_t* f);
void function_set_name(function_t* f, char* name);
void function_set_return(function_t* f, type_t type);
void function_add_param(function_t* f, variable_t* v);
void function_set_block(function_t* f, block_t* b);

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
void function_table_print(function_table_t* t);
void function_table_add(function_table_t* t, function_t* f);

#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ecc.h"

/**
 * Types
 */

void type_print(type_t type)
{
    switch (type) {
        case VOID_T: printf("void"); break;
        case INT_T: printf("int"); break;
        case FLOAT_T: printf("float"); break;
    }
}

/**
 * Values
 */

value_t* create_value_identifier(char* identifier)
{
    value_t* v = malloc(sizeof(value_t));
    assert(v != NULL);
    v->type = IDENTIFIER_T;
    v->identifier = identifier;
    return v;
}
value_t* create_value_const_int(int const_int)
{
    value_t* v = malloc(sizeof(value_t));
    assert(v != NULL);
    v->type = CONST_INT_T;
    v->const_int = const_int;
    v->identifier = NULL;
    return v;
}
value_t* create_value_const_float(float const_float)
{
    value_t* v = malloc(sizeof(value_t));
    assert(v != NULL);
    v->type = CONST_FLOAT_T;
    v->const_float = const_float;
    v->identifier = NULL;
    return v;
}
void delete_value(value_t* v)
{
    assert(v != NULL);
    if (v->identifier != NULL) {
        free(v->identifier);
        v->identifier = NULL;
    }
    free(v);
}
void value_print(value_t* v)
{
    assert(v != NULL);
    switch (v->type) {
        case IDENTIFIER_T: printf("%s", v->identifier); break;
        case CONST_INT_T: printf("%d", v->const_int); break;
        case CONST_FLOAT_T: printf("%f", v->const_float); break;
    }
}

/**
 * unary_operation
 */

unary_operation_t* create_unary_operation_nop(value_t* v)
{
    unary_operation_t* op = malloc(sizeof(unary_operation_t));
    assert(op != NULL);
    op->type = NONE_T;
    op->value = v;
    op->arguments = NULL;
    return op;
}
unary_operation_t* create_unary_operation_inc(value_t* v)
{
    unary_operation_t* op = malloc(sizeof(unary_operation_t));
    assert(op != NULL);
    op->type = INC_OP_T;
    op->value = v;
    op->arguments = NULL;
    return op;
}
unary_operation_t* create_unary_operation_dec(value_t* v)
{
    unary_operation_t* op = malloc(sizeof(unary_operation_t));
    assert(op != NULL);
    op->type = DEC_OP_T;
    op->value = v;
    op->arguments = NULL;
    return op;
}
unary_operation_t* create_unary_operation_func(value_t* v, arguments_list_t* l)
{
    unary_operation_t* op = malloc(sizeof(unary_operation_t));
    assert(op != NULL);
    assert(l != NULL);
    op->type = FUNCTION_CALL_T;
    op->value = v;
    op->arguments = l;
    return op;
}
unary_operation_t* create_unary_operation_array(value_t* v, arguments_list_t* l)
{
    unary_operation_t* op = malloc(sizeof(unary_operation_t));
    assert(op != NULL);
    assert(l != NULL);
    op->type = ARRAY_CALL_T;
    op->value = v;
    op->arguments = l;
    return op;
}
void delete_unary_operation(unary_operation_t* op)
{
    assert(op != NULL);
    if (op->value != NULL) {
        delete_value(op->value);
        op->value = NULL;
    }
    if (op->arguments != NULL) {
        delete_arguments_list(op->arguments);
        op->arguments = NULL;
    }
    free(op);
}
void unary_operation_print(unary_operation_t* op)
{
    assert(op != NULL);
    switch (op->type) {
        case NONE_T:
            value_print(op->value);
            break;
        case INC_OP_T:
            value_print(op->value);
            printf("++");
            break;
        case DEC_OP_T:
            value_print(op->value);
            printf("--");
            break;
        case FUNCTION_CALL_T:
            value_print(op->value);
            printf("(");
            arguments_list_print(op->arguments);
            printf(")");
            break;
        case ARRAY_CALL_T:
            value_print(op->value);
            printf("[");
            arguments_list_print(op->arguments);
            printf("]");
            break;
    }
}

/**
 * Operations
 */

operation_t* create_operation_nop(unary_operation_t* op1)
{
    assert(op1 != NULL);
    operation_t* op = malloc(sizeof(operation_t));
    assert(op != NULL);
    op->type = NOP_T;
    op->op1 = op1;
    op->op2 = NULL;
    return op;
}
operation_t* create_operation_minus(unary_operation_t* op1)
{
    assert(op1 != NULL);
    operation_t* op = malloc(sizeof(operation_t));
    assert(op != NULL);
    op->type = MINUS_T;
    op->op1 = op1;
    op->op2 = NULL;
    return op;
}
operation_t* create_operation_plus(unary_operation_t* op1)
{
    assert(op1 != NULL);
    operation_t* op = malloc(sizeof(operation_t));
    assert(op != NULL);
    op->type = PLUS_T;
    op->op1 = op1;
    op->op2 = NULL;
    return op;
}
operation_t* create_operation_assign(unary_operation_t* op1, unary_operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    operation_t* op = malloc(sizeof(operation_t));
    assert(op != NULL);
    op->type = ASSIGN_T;
    op->op1 = op1;
    op->op2 = op2;
    return op;
}
operation_t* create_operation_add(unary_operation_t* op1, unary_operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    operation_t* op = malloc(sizeof(operation_t));
    assert(op != NULL);
    op->type = ADD_T;
    op->op1 = op1;
    op->op2 = op2;
    return op;
}
operation_t* create_operation_sub(unary_operation_t* op1, unary_operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    operation_t* op = malloc(sizeof(operation_t));
    assert(op != NULL);
    op->type = SUB_T;
    op->op1 = op1;
    op->op2 = op2;
    return op;
}
operation_t* create_operation_mult(unary_operation_t* op1, unary_operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    operation_t* op = malloc(sizeof(operation_t));
    assert(op != NULL);
    op->type = MULT_T;
    op->op1 = op1;
    op->op2 = op2;
    return op;
}
void delete_operation(operation_t* op)
{
    assert(op != NULL);
    if (op->op1 != NULL) {
        delete_unary_operation(op->op1);
        op->op1 = NULL;
    }
    if (op->op2 != NULL) {
        delete_unary_operation(op->op2);
        op->op2 = NULL;
    }
    free(op);
}
void operation_print(operation_t* op)
{
    assert(op != NULL);
    switch (op->type) {
        case NOP_T:
            unary_operation_print(op->op1);
            break;
        case MINUS_T:
            unary_operation_print(op->op1);
            printf(" -");
            break;
        case PLUS_T:
            unary_operation_print(op->op1);
            printf(" +");
            break;
        case ASSIGN_T:
            unary_operation_print(op->op1);
            printf(" = ");
            unary_operation_print(op->op2);
            break;
        case ADD_T:
            unary_operation_print(op->op1);
            printf(" += ");
            unary_operation_print(op->op2);
            break;
        case SUB_T:
            unary_operation_print(op->op1);
            printf(" -= ");
            unary_operation_print(op->op2);
            break;
        case MULT_T:
            unary_operation_print(op->op1);
            printf(" *= ");
            unary_operation_print(op->op2);
            break;
    }
}

/**
 * arguments_list
 */

arguments_list_t* create_arguments_list()
{
    arguments_list_t* l = malloc(sizeof(arguments_list_t));
    assert(l != NULL);
    l->operations = NULL;
    l->size = 0;
    return l;
}
void delete_arguments_list(arguments_list_t* l)
{
    assert(l != NULL);
    if (l->operations != NULL) {
        int i;
        for (i=0;i<l->size;i++) {
            delete_operation(l->operations[i]);
        }
        free(l->operations);
        l->operations = NULL;
    }
    free(l);
}
void arguments_list_print(arguments_list_t* l)
{
    assert(l != NULL);
    int i;
    for (i=0;i<l->size;i++) {
        if (i > 0) printf(", ");
        operation_print(l->operations[i]);
    }
}
void arguments_list_add_arg(arguments_list_t* l, operation_t* op)
{
    assert(l != NULL);
    assert(op != NULL);
    l->size++;
    l->operations = realloc(l->operations, sizeof(operation_t*)*l->size);
    assert(l->operations != NULL);
    l->operations[l->size-1] = op;
}

/**
 * Condition
 */

condition_t* create_condition_bool(operation_t* op1)
{
    assert(op1 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->op1 = op1;
    c->op2 = NULL;
    c->type = BOOL_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_l(operation_t* op1, operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->op1 = op1;
    c->op2 = op2;
    c->type = L_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_g(operation_t* op1, operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->op1 = op1;
    c->op2 = op2;
    c->type = G_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_le(operation_t* op1, operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->op1 = op1;
    c->op2 = op2;
    c->type = LE_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_ge(operation_t* op1, operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->op1 = op1;
    c->op2 = op2;
    c->type = GE_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_eq(operation_t* op1, operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->op1 = op1;
    c->op2 = op2;
    c->type = EQ_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_ne(operation_t* op1, operation_t* op2)
{
    assert(op1 != NULL);
    assert(op2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->op1 = op1;
    c->op2 = op2;
    c->type = NE_T;
    c->statement = NULL;
    return c;
}
void delete_condition(condition_t* c)
{
    assert(c != NULL);
    if (c->op1 != NULL) {
        delete_operation(c->op1);
        c->op1 = NULL;
    }
    if (c->op2 != NULL) {
        delete_operation(c->op2);
        c->op2 = NULL;
    }
    if (c->statement != NULL) {
        delete_statement(c->statement);
        c->statement = NULL;
    }
    free(c);
}
void condition_print(condition_t* c)
{
    assert(c != NULL);
    printf("IF (");
    switch (c->type) {
        case BOOL_T:
            operation_print(c->op1);
            break;
        case L_T:
            operation_print(c->op1);
            printf(" < ");
            operation_print(c->op2);
            break;
        case G_T:
            operation_print(c->op1);
            printf(" > ");
            operation_print(c->op2);
            break;
        case LE_T:
            operation_print(c->op1);
            printf(" <= ");
            operation_print(c->op2);
            break;
        case GE_T:
            operation_print(c->op1);
            printf(" >= ");
            operation_print(c->op2);
            break;
        case EQ_T:
            operation_print(c->op1);
            printf(" == ");
            operation_print(c->op2);
            break;
        case NE_T:
            operation_print(c->op1);
            printf(" != ");
            operation_print(c->op2);
            break;
    }
    printf(") ");
    if (c->statement != NULL) {
        statement_print(c->statement);
    }
    else {
        printf("{}");
    }
}
void condition_set_statement(condition_t* c, struct statement_t* s)
{
    assert(c != NULL);
    assert(s != NULL);
    c->statement = s;
}

/**
 * Label
 */

label_t* create_label(char* identifier)
{
    assert(identifier != NULL);
    label_t* l = identifier;
    assert(l != NULL);
    return l;
}
void delete_label(label_t* l)
{
    assert(l != NULL);
    free(l);
}
void label_print(label_t* l)
{
    assert(l != NULL);
    printf("%s:\n", l);
}

/**
 * Jump
 */

jump_t* create_jump_return()
{
    jump_t* j = malloc(sizeof(jump_t));
    assert(j != NULL);
    j->op = NULL;
    j->label = NULL;
    j->type = RETURN_T;
    return j;
}
jump_t* create_jump_return_op(operation_t* op)
{
    assert(op != NULL);
    jump_t* j = malloc(sizeof(jump_t));
    assert(j != NULL);
    j->op = op;
    j->label = NULL;
    j->type = RETURN_OP_T;
    return j;
}
jump_t* create_jump_goto(label_t* label)
{
    assert(label != NULL);
    jump_t* j = malloc(sizeof(jump_t));
    assert(j != NULL);
    j->op = NULL;
    j->label = label;
    j->type = GOTO_T;
    return j;
}
void delete_jump(jump_t* j)
{
    assert(j != NULL);
    if (j->op != NULL) {
        delete_operation(j->op);
        j->op = NULL;
    }
    if (j->label != NULL) {
        delete_label(j->label);
        j->label = NULL;
    }
    free(j);
}
void jump_print(jump_t* j)
{
    assert(j != NULL );
    switch (j->type) {
        case GOTO_T:
            printf("GOTO ");
            label_print(j->label);
            break;
        case RETURN_T:
            printf("RETURN");
            break;
        case RETURN_OP_T:
            printf("RETURN ");
            operation_print(j->op);
            break;
    }
}

/**
 * Statement
 */

statement_t* create_statement_label(label_t* l)
{
    assert(l != NULL);
    statement_t* s = malloc(sizeof(statement_t));
    assert(s != NULL);
    s->type = LABEL_T;
    s->label = l;
    s->operation = NULL;
    s->condition = NULL;
    s->jump = NULL;
    s->block = NULL;
    return s;
}
statement_t* create_statement_op(operation_t* op)
{
    assert(op != NULL);
    statement_t* s = malloc(sizeof(statement_t));
    assert(s != NULL);
    s->type = OP_T;
    s->label = NULL;
    s->operation = op;
    s->condition = NULL;
    s->jump = NULL;
    s->block = NULL;
    return s;
}
statement_t* create_statement_cond(condition_t* cond)
{
    assert(cond != NULL);
    statement_t* s = malloc(sizeof(statement_t));
    assert(s != NULL);
    s->type = COND_T;
    s->label = NULL;
    s->operation = NULL;
    s->condition = cond;
    s->jump = NULL;
    s->block = NULL;
    return s;
}
statement_t* create_statement_jmp(jump_t* jmp)
{
    assert(jmp != NULL);
    statement_t* s = malloc(sizeof(statement_t));
    assert(s != NULL);
    s->type = JMP_T;
    s->label = NULL;
    s->operation = NULL;
    s->condition = NULL;
    s->jump = jmp;
    s->block = NULL;
    return s;
}
statement_t* create_statement_block(block_t* block)
{
    assert(block != NULL);
    statement_t* s = malloc(sizeof(statement_t));
    assert(s != NULL);
    s->type = BLOCK_T;
    s->label = NULL;
    s->operation = NULL;
    s->condition = NULL;
    s->jump = NULL;
    s->block = block;
    return s;
}
void delete_statement(statement_t* s)
{
    assert(s != NULL);
    if (s->label != NULL) {
        delete_label(s->label);
        s->label = NULL;
    }
    if (s->operation != NULL) {
        delete_operation(s->operation);
        s->operation = NULL;
    }
    if (s->condition != NULL) {
        delete_condition(s->condition);
        s->condition = NULL;
    }
    if (s->jump != NULL) {
        delete_jump(s->jump);
        s->jump = NULL;
    }
    if (s->block != NULL) {
        delete_block(s->block);
        s->block = NULL;
    }
    free(s);
}
void statement_print(statement_t* s)
{
    assert(s != NULL);
    switch (s->type) {
        case LABEL_T:
            label_print(s->label);
            break;
        case OP_T:
            operation_print(s->operation);
            break;
        case COND_T:
            condition_print(s->condition);
            break;
        case JMP_T:
            jump_print(s->jump);
            break;
        case BLOCK_T:
            block_print(s->block);
            break;
    }
}

/**
 * Statement table
 */

statement_table_t* create_statement_table()
{
    statement_table_t* t = malloc(sizeof(statement_table_t));
    assert(t != NULL);
    t->table = NULL;
    t->size = 0;
    return t;
}
void delete_statement_table(statement_table_t* t)
{
    assert(t != NULL);
    if (t->table != NULL) {
        int i;
        for (i=0;i<t->size;i++) {
            delete_statement(t->table[i]);
        }
        free(t->table);
    }
    free(t);
}
void statement_table_print(statement_table_t* t)
{
    assert(t != NULL);
    int i;
    for (i=0;i<t->size;i++) {
        statement_print(t->table[i]);
        printf("\n");
    }
}
void statement_table_add(statement_table_t* t, statement_t* s)
{
    assert(t != NULL);
    assert(s != NULL);
    t->size++;
    t->table = realloc(t->table, t->size*sizeof(statement_t*));
    assert(t->table != NULL);
    t->table[t->size-1] = s;
}

/**
 * Variables
 */

variable_t* create_variable()
{
    variable_t* v = malloc(sizeof(variable_t));
    assert(v != NULL);
    v->name = NULL;
    v->type = VOID_T;
    v->dim = 0;
    v->size_array = NULL;
    return v;
}
void delete_variable(variable_t* v)
{
    assert(v != NULL);
    if (v->name != NULL) {
        free(v->name);
        v->name = NULL;
    }
    if (v->size_array != NULL) {
        free(v->size_array);
        v->size_array = NULL;
    }
    free(v);
}
void variable_print(variable_t* v)
{
    assert(v != NULL);
    type_print(v->type);
    if (v->name != NULL) {
        printf(" %s", v->name);
    }
    else {
        printf(" ?");
    }
    int i;
    for (i=0;i<v->dim;i++) {
        printf("[%d]", v->size_array[i]);
    }
}
void variable_set_name(variable_t* v, char* name)
{
    assert(v != NULL);
    assert(name != NULL);
    assert(v->name == NULL);
    v->name = name;
}
void variable_set_type(variable_t* v, type_t type)
{
    assert(v != NULL);
    v->type = type;
}
void variable_add_dim(variable_t* v, int size)
{
    assert(v != NULL);
    v->dim++;
    v->size_array = realloc(v->size_array, sizeof(int)*v->dim);
    assert(v->size_array != NULL);
    v->size_array[v->dim - 1] = size;
}

/**
 * Variable table
 */

variable_table_t* create_variable_table()
{
    variable_table_t* t = malloc(sizeof(variable_table_t));
    assert(t != NULL);
    t->table = NULL;
    t->size = 0;
    return t;
}
void delete_variable_table(variable_table_t* t)
{
    assert(t != NULL);
    if (t->table != NULL) {
        int i;
        for (i=0;i<t->size;i++) {
            delete_variable(t->table[i]);
        }
        free(t->table);
        t->table = NULL;
    }
    free(t);
}
void variable_table_print(variable_table_t* t)
{
    assert(t != NULL);
    int i;
    for (i=0;i<t->size;i++) {
        variable_print(t->table[i]);
        printf("\n");
    }
}
void variable_table_add_var(variable_table_t* t, variable_t* v)
{
    assert(t != NULL);
    assert(v != NULL);
    t->size++;
    t->table = realloc(t->table, sizeof(variable_t*)*t->size);
    assert(t->table != NULL);
    t->table[t->size-1] = v;
}
void variable_table_set_all_type(variable_table_t* t, type_t type)
{
    assert(t != NULL);
    int i;
    for (i=0;i<t->size;i++) {
        variable_set_type(t->table[i], type);
    }
}
variable_table_t* variable_table_merge(variable_table_t* t1, variable_table_t* t2)
{
    assert(t1 != NULL);
    assert(t2 != NULL);
    variable_table_t* t = create_variable_table();
    int i;
    for (i=0;i<t1->size;i++) {
        variable_table_add_var(t, t1->table[i]);
    }
    for (i=0;i<t2->size;i++) {
        variable_table_add_var(t, t2->table[i]);
    }
    if (t1->table != NULL) {
        free(t1->table);
    }
    free(t1);
    if (t2->table != NULL) {
        free(t2->table);
    }
    free(t2);
    return t;
}

/**
 * Block
 */

block_t* create_block(variable_table_t* vt, statement_table_t* st)
{
    assert(vt != NULL);
    assert(st != NULL);
    block_t* b = malloc(sizeof(block_t));
    assert(b != NULL);
    b->vt=  vt;
    b->st = st;
    return b;
}
void delete_block(block_t* b)
{
    assert(b != NULL);
    delete_variable_table(b->vt);
    delete_statement_table(b->st);
    free(b);
}
void block_print(block_t* b)
{
    assert(b != NULL);
    printf("{\n");
    variable_table_print(b->vt);
    statement_table_print(b->st);
    printf("}\n");
}

/**
 * Functions
 */

function_t* create_function()
{
    function_t* f = malloc(sizeof(function_t));
    assert(f != NULL);
    f->name = NULL;
    f->type_return = VOID_T;
    f->nb_params = 0;
    f->params = NULL;
    f->block = NULL;
    return f;
}
void delete_function(function_t* f)
{
    assert(f != NULL);
    if (f->name != NULL) {
        free(f->name);
    }
    if (f->params != NULL) {
        int i;
        for (i=0;i<f->nb_params;i++) {
            delete_variable(f->params[i]);
        }
        free(f->params);
    }
    if (f->block != NULL) {
        delete_block(f->block);
        f->block = NULL;
    }
    free(f);
}
void function_print(function_t* f)
{
    assert(f != NULL);
    type_print(f->type_return);
    if (f->name != NULL) {
        printf(" %s", f->name);
    }
    else {
        printf(" ?");
    }
    printf("(");
    int i;
    for (i=0;i<f->nb_params;i++) {
        if (i > 0) {
            printf(", ");
        }
        variable_print(f->params[i]);
    }
    printf(")");
    printf("\n");
    if (f->block != NULL) {
        block_print(f->block);
    }
}

void function_set_name(function_t* f, char* name)
{
    assert(f != NULL);
    assert(name != NULL);
    assert(f->name == NULL);
    f->name = name;
}
void function_set_return(function_t* f, type_t type)
{
    assert(f != NULL);
    f->type_return = type;
}
void function_add_param(function_t* f, variable_t* v)
{
    assert(f != NULL);
    assert(v != NULL);
    f->nb_params++;
    f->params = realloc(f->params, sizeof(type_t*)*f->nb_params);
    assert(f->params != NULL);
    f->params[f->nb_params-1] = v;
}
void function_set_block(function_t* f, block_t* b)
{
    assert(f != NULL);
    assert(b != NULL);
    f->block = b;
}

/**
 * Functions table
 */

function_table_t* create_function_table()
{
    function_table_t* t = malloc(sizeof(function_table_t));
    assert(t != NULL);
    t->table = NULL;
    t->size = 0;
    return t;
}
void delete_function_table(function_table_t* t)
{
    assert(t != NULL);
    if (t->table != NULL) {
        int i;
        for (i=0;i<t->size;i++) {
            delete_function(t->table[i]);
        }
        free(t->table);
    }
    free(t);
}
void function_table_print(function_table_t* t)
{
    assert(t != NULL);
    int i;
    for (i=0;i<t->size;i++) {
        function_print(t->table[i]);
    }
}
void function_table_add(function_table_t* t, function_t* f)
{
    assert(t != NULL);
    assert(f != NULL);
    t->size++;
    t->table = realloc(t->table, sizeof(function_t*)*t->size);
    assert(t->table != NULL);
    t->table[t->size-1] = f;
}


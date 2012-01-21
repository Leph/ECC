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
        default: break;
    }
}
int type_size(type_t type)
{
    switch (type) {
        case INT_T: return INT_BYTES_LEN;
        case FLOAT_T: return FLOAT_BYTES_LEN;
        default: return 0;
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
 * arguments_list
 */

arguments_list_t* create_arguments_list()
{
    arguments_list_t* l = malloc(sizeof(arguments_list_t));
    assert(l != NULL);
    l->values = NULL;
    l->size = 0;
    return l;
}
void delete_arguments_list(arguments_list_t* l)
{
    assert(l != NULL);
    if (l->values != NULL) {
        int i;
        for (i=0;i<l->size;i++) {
            delete_value(l->values[i]);
        }
        free(l->values);
        l->values = NULL;
    }
    free(l);
}
void arguments_list_print(arguments_list_t* l)
{
    assert(l != NULL);
    int i;
    for (i=0;i<l->size;i++) {
        if (i > 0) printf(", ");
        value_print(l->values[i]);
    }
}
void arguments_list_add_arg(arguments_list_t* l, value_t* v)
{
    assert(l != NULL);
    assert(v != NULL);
    l->size++;
    l->values = realloc(l->values, sizeof(value_t*)*l->size);
    assert(l->values != NULL);
    l->values[l->size-1] = v;
}

/**
 * unary_operation
 */
unary_expression_t* create_unary_expression_value(value_t* v)
{
    assert(v != NULL);
    unary_expression_t* e = malloc(sizeof(unary_expression_t));
    assert(e != NULL);
    e->type = VALUE_T;
    e->value = v;
    e->arguments = NULL;
    return e;
}
unary_expression_t* create_unary_expression_array(value_t* v, arguments_list_t* l)
{
    assert(v != NULL);
    assert(l != NULL);
    unary_expression_t* e = malloc(sizeof(unary_expression_t));
    assert(e != NULL);
    e->type = ARRAY_T;
    e->value = v;
    e->arguments = l;
    return e;
}
unary_expression_t* create_unary_expression_func(value_t* v, arguments_list_t* l)
{
    assert(v != NULL);
    assert(l != NULL);
    unary_expression_t* e = malloc(sizeof(unary_expression_t));
    assert(e != NULL);
    e->type = FUNCTION_T;
    e->value = v;
    e->arguments = l;
    return e;
}
void delete_unary_expression(unary_expression_t* e)
{
    assert(e != NULL);
    delete_value(e->value);
    if (e->arguments != NULL) {
        delete_arguments_list(e->arguments);
        e->arguments = NULL;
    }
    free(e);
}
void unary_expression_print(unary_expression_t* e)
{
    assert(e != NULL);
    value_print(e->value);
    switch (e->type) {
        case VALUE_T:
            break;
        case ARRAY_T:
            printf("[");
            arguments_list_print(e->arguments);
            printf("]");
            break;
        case FUNCTION_T:
            printf("(");
            arguments_list_print(e->arguments);
            printf(")");
            break;
    }
}

/**
 * Expressions
 */

expression_t* create_expression_nop(unary_expression_t* e1)
{
    assert(e1 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = NOP_T;
    exp->left = NULL;
    exp->right = e1;
    return exp;
}
expression_t* create_expression_inc(unary_expression_t* e1)
{
    assert(e1 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = INC_T;
    exp->left = e1;
    exp->right = NULL;
    return exp;
}
expression_t* create_expression_dec(unary_expression_t* e1)
{
    assert(e1 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = DEC_T;
    exp->left = e1;
    exp->right = NULL;
    return exp;
}
expression_t* create_expression_print(unary_expression_t* e1)
{
    assert(e1 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = PRINT_T;
    exp->left = NULL;
    exp->right = e1;
    return exp;
}
expression_t* create_expression_sqrt(unary_expression_t* e1)
{
    assert(e1 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = SQRT_T;
    exp->left = NULL;
    exp->right = e1;
    return exp;
}
expression_t* create_expression_assign(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = ASSIGN_T;
    exp->left = e1;
    exp->right = e2;
    return exp;
}
expression_t* create_expression_add(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = ADD_T;
    exp->left = e1;
    exp->right = e2;
    return exp;
}
expression_t* create_expression_sub(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = SUB_T;
    exp->left = e1;
    exp->right = e2;
    return exp;
}
expression_t* create_expression_mul(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = MUL_T;
    exp->left = e1;
    exp->right = e2;
    return exp;
}

expression_t* create_expression_div(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    expression_t* exp = malloc(sizeof(expression_t));
    assert(exp != NULL);
    exp->type = DIV_T;
    exp->left = e1;
    exp->right = e2;
    return exp;
}


void delete_expression(expression_t* e)
{
    assert(e != NULL);
    if (e->left != NULL) {
        delete_unary_expression(e->left);
        e->left = NULL;
    }
    if (e->right != NULL) {
        delete_unary_expression(e->right);
        e->right = NULL;
    }
    free(e);
}
void expression_print(expression_t* e)
{
    assert(e != NULL);
    switch (e->type) {
        case NOP_T:
            unary_expression_print(e->right);
            break;
        case INC_T:
            unary_expression_print(e->left);
            printf("++");
            break;
        case DEC_T:
            unary_expression_print(e->left);
            printf("--");
            break;
        case PRINT_T:
            printf("print ");
            unary_expression_print(e->right);
        case SQRT_T:
            printf("sqrt ");
            unary_expression_print(e->right);
        case ASSIGN_T:
            unary_expression_print(e->left);
            printf(" = ");
            unary_expression_print(e->right);
            break;
        case ADD_T:
            unary_expression_print(e->left);
            printf(" += ");
            unary_expression_print(e->right);
            break;
        case SUB_T:
            unary_expression_print(e->left);
            printf(" -= ");
            unary_expression_print(e->right);
            break;
        case MUL_T:
            unary_expression_print(e->left);
            printf(" *= ");
            unary_expression_print(e->right);
            break;
        case DIV_T:
            unary_expression_print(e->left);
            printf(" /= ");
            unary_expression_print(e->right);
            break;

    }
}

/**
 * Condition
 */

condition_t* create_condition_bool(unary_expression_t* e1)
{
    assert(e1 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->e1 = e1;
    c->e2 = NULL;
    c->type = BOOL_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_l(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->e1 = e1;
    c->e2 = e2;
    c->type = L_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_g(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->e1 = e1;
    c->e2 = e2;
    c->type = G_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_le(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->e1 = e1;
    c->e2 = e2;
    c->type = LE_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_ge(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->e1 = e1;
    c->e2 = e2;
    c->type = GE_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_eq(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->e1 = e1;
    c->e2 = e2;
    c->type = EQ_T;
    c->statement = NULL;
    return c;
}
condition_t* create_condition_ne(unary_expression_t* e1, unary_expression_t* e2)
{
    assert(e1 != NULL);
    assert(e2 != NULL);
    condition_t* c = malloc(sizeof(condition_t));
    assert(c != NULL);
    c->e1 = e1;
    c->e2 = e2;
    c->type = NE_T;
    c->statement = NULL;
    return c;
}
void delete_condition(condition_t* c)
{
    assert(c != NULL);
    if (c->e1 != NULL) {
        delete_unary_expression(c->e1);
        c->e1 = NULL;
    }
    if (c->e2 != NULL) {
        delete_unary_expression(c->e2);
        c->e2 = NULL;
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
            unary_expression_print(c->e1);
            break;
        case L_T:
            unary_expression_print(c->e1);
            printf(" < ");
            unary_expression_print(c->e2);
            break;
        case G_T:
            unary_expression_print(c->e1);
            printf(" > ");
            unary_expression_print(c->e2);
            break;
        case LE_T:
            unary_expression_print(c->e1);
            printf(" <= ");
            unary_expression_print(c->e2);
            break;
        case GE_T:
            unary_expression_print(c->e1);
            printf(" >= ");
            unary_expression_print(c->e2);
            break;
        case EQ_T:
            unary_expression_print(c->e1);
            printf(" == ");
            unary_expression_print(c->e2);
            break;
        case NE_T:
            unary_expression_print(c->e1);
            printf(" != ");
            unary_expression_print(c->e2);
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
    j->exp = NULL;
    j->label = NULL;
    j->type = RETURN_T;
    return j;
}
jump_t* create_jump_return_exp(unary_expression_t* exp)
{
    assert(exp != NULL);
    jump_t* j = malloc(sizeof(jump_t));
    assert(j != NULL);
    j->exp = exp;
    j->label = NULL;
    j->type = RETURN_EXP_T;
    return j;
}
jump_t* create_jump_goto(label_t* label)
{
    assert(label != NULL);
    jump_t* j = malloc(sizeof(jump_t));
    assert(j != NULL);
    j->exp = NULL;
    j->label = label;
    j->type = GOTO_T;
    return j;
}
void delete_jump(jump_t* j)
{
    assert(j != NULL);
    if (j->exp != NULL) {
        delete_unary_expression(j->exp);
        j->exp = NULL;
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
        case RETURN_EXP_T:
            printf("RETURN ");
            unary_expression_print(j->exp);
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
    s->expression = NULL;
    s->condition = NULL;
    s->jump = NULL;
    s->block = NULL;
    return s;
}
statement_t* create_statement_exp(expression_t* exp)
{
    assert(exp != NULL);
    statement_t* s = malloc(sizeof(statement_t));
    assert(s != NULL);
    s->type = EXP_T;
    s->label = NULL;
    s->expression = exp;
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
    s->expression = NULL;
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
    s->expression = NULL;
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
    s->expression = NULL;
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
    if (s->expression != NULL) {
        delete_expression(s->expression);
        s->expression = NULL;
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
        case EXP_T:
            expression_print(s->expression);
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
    v->offset = 0;
    v->is_global = 0;
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
int variable_size(variable_t* v)
{
    assert(v != NULL);
    int size;
    if (v->dim == 0) {
        size = type_size(v->type);
    }
    else {
        size = REF_BYTES_LEN;
    }
    return size;
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
    t->parent = NULL;
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
variable_t* variable_table_search_name(variable_table_t* t, char* name)
{
    assert(t != NULL);
    assert(name != NULL);
    int i;
    for (i=0;i<t->size;i++) {
        if (strcmp(name, t->table[i]->name) == 0) {
            return t->table[i];
        }
    }
    if (t->parent != NULL) {
        return variable_table_search_name(t->parent, name);
    }
    else {
        return NULL;
    }
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
    f->params = create_variable_table();
    f->block = NULL;
    f->offset = 0;
    return f;
}
void delete_function(function_t* f)
{
    assert(f != NULL);
    if (f->name != NULL) {
        free(f->name);
    }
    if (f->params != NULL) {
        delete_variable_table(f->params);
        f->params = NULL;
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
    for (i=0;i<f->params->size;i++) {
        if (i > 0) {
            printf(", ");
        }
        variable_print(f->params->table[i]);
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
    variable_table_add_var(f->params, v);
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
function_t* function_table_search_name(function_table_t* t, char* name)
{
    assert(t != NULL);
    assert(name != NULL);
    int i;
    for (i=0;i<t->size;i++) {
        if (strcmp(t->table[i]->name, name) == 0) {
            return t->table[i];
        }
    }
    return NULL;
}

/**
 * utils functions
 */

void do_variable_table_links_block(block_t* b, variable_table_t* current)
{
    assert(b != NULL);
    assert(current != NULL);
    b->vt->parent = current;
    int i;
    for (i=0;i<b->st->size;i++) {
        if (b->st->table[i]->type == BLOCK_T) {
            do_variable_table_links_block(b->st->table[i]->block, b->vt);
        }
        else if (b->st->table[i]->type == COND_T) {
            condition_t* c = b->st->table[i]->condition;
            if (c->statement->type == BLOCK_T) {
                do_variable_table_links_block(c->statement->block, b->vt);
            }
        }
    }
}
void do_variable_table_links(function_table_t* ft, variable_table_t* global_table)
{
    assert(ft != NULL);
    assert(global_table != NULL);
    int i;
    for (i=0;i<ft->size;i++) {
        ft->table[i]->params->parent = global_table;
        do_variable_table_links_block(ft->table[i]->block, ft->table[i]->params);
    }
    for (i=0;i<global_table->size;i++) {
        global_table->table[i]->is_global = 1;
    }
}

int do_variable_table_block(block_t* b, int current_offset)
{
    int offset = current_offset;
    int i;
    for (i=0;i<b->vt->size;i++) {
        b->vt->table[i]->offset = offset;
        offset -= variable_size(b->vt->table[i]);
    }
    for (i=0;i<b->st->size;i++) {
        if (b->st->table[i]->type == BLOCK_T) {
            offset = do_variable_table_block(b->st->table[i]->block, offset);
        }
        else if (b->st->table[i]->type == COND_T) {
            condition_t* c = b->st->table[i]->condition;
            if (c->statement->type == BLOCK_T) {
                offset = do_variable_table_block(c->statement->block, offset);
            }
        }
    }
    return offset;
}
void do_variable_offset(function_table_t* ft)
{
    assert(ft != NULL);
    int i,j;
    for (i=0;i<ft->size;i++) {
        function_t* f = ft->table[i];
        int offset = REF_BYTES_LEN;
        for (j=0;j<f->params->size;j++) {
            variable_t* v = f->params->table[j];
            offset += variable_size(v);
            v->offset = offset;
        }
        /** 
         * L'offset commence à -4 pour ne pas écraser %ebp sauvegarder sur la pile
         * f->offset = taille les varuableà allouer pour cette fonction
         * on corrige +4 puisque l'on commence l'offset à -4
         */
        f->offset = do_variable_table_block(f->block, -REF_BYTES_LEN) + REF_BYTES_LEN;
    }
}


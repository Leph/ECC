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
    v->name = strdup(name);
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
}

void function_set_name(function_t* f, char* name)
{
    assert(f != NULL);
    assert(name != NULL);
    assert(f->name == NULL);
    f->name = strdup(name);
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
    if (t != NULL) {
        if (t->table != NULL) {
            int i;
            for (i=0;i<t->size;i++) {
                delete_function(t->table[i]);
            }
            free(t->table);
        }
        free(t);
    }
}

void function_table_add_function(function_table_t* t, function_t* f)
{
    t->size++;
    t->table = realloc(t->table, sizeof(function_t*)*t->size);
    assert(t->table != NULL);
    t->table[t->size-1] = f;
}


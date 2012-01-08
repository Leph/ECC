#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ecc.h"

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

function_t* create_function()
{
    function_t* f = malloc(sizeof(function_t));
    assert(f != NULL);
    f->name = NULL;
    f->nb_params = 0;
    f->type_params = NULL;
    f->type_return = VOID_T;
    return f;
}
void delete_function(function_t* f)
{
    if (f != NULL) {
        if (f->name != NULL) {
            free(f->name);
        }
        if (f->type_params != NULL) {
            free(f->type_params);
        }
        free(f);
    }
}

void function_set_name(function_t* f, char* name)
{
    assert(f != NULL);
    assert(name != NULL);
    f->name = strdup(name);
}
void function_set_return(function_t* f, type_t type)
{
    assert(f != NULL);
    f->type_return = type;
}
void function_add_param(function_t* f, type_t type)
{
    assert(f != NULL);
    f->nb_params++;
    f->type_params = realloc(f->type_params, sizeof(type_t)*f->nb_params);
    assert(f->type_params != NULL);
    f->type_params[f->nb_params-1] = type;
}

void function_table_add_function(function_table_t* t, function_t* f)
{
    t->size++;
    t->table = realloc(t->table, sizeof(function_t*)*t->size);
    assert(t->table != NULL);
    t->table[t->size-1] = f;
}


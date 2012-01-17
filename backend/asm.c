#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ecc.h"
#include "asm.h"

void asm_global_table(variable_table_t* t)
{
    assert(t != NULL);
    printf(".data\n");
    int i;
    for (i=0;i<t->size;i++) {
        asm_global_variable(t->table[i]);
    }
}

void asm_global_variable(variable_t *v){
    assert(v != NULL);
    int size = variable_size(v);
    printf("\t.comm\t%s, %d\n", v->name, size);
}

void asm_function_table(function_table_t* t)
{
    assert(t != NULL);
    printf(".text\n");
    int i;
    for (i=0;i<t->size;i++) {
        asm_function(t->table[i]);
    }
}

void asm_function(function_t* f)
{
    assert(f != NULL);
    printf(".globl %s\n", f->name);
    printf("%s:\n", f->name);
    printf("\tpushl\t%%ebp\n");
    printf("\tmovl\t%%esp, %%ebp\n");

    if (f->offset < 0) {
        printf("\tsubl\t$%d, %%esp\n", -f->offset);
    }

    asm_block(f->block);

    if (f->offset < 0) {
        printf("\taddl\t$%d, %%esp\n", -f->offset);
    }

    printf("\tleave\n");
    printf("\tret\n");
}

void asm_block(block_t* b)
{
    assert(b != NULL);
    int i;
    for (i=0;i<b->st->size;i++) {
        asm_statement(b->st->table[i], b->vt);
    }
}

void asm_statement(statement_t* s, variable_table_t* t)
{
    assert(s != NULL);
    assert(t != NULL);
    switch (s->type) {
        case LABEL_T:
            break;
        case EXP_T:
            asm_expression(s->expression, t);
            break;
        case COND_T:
            break;
        case JMP_T:
            break;
        case BLOCK_T:
            asm_block(s->block);
            break;
    }
}

void asm_expression(expression_t* e, variable_table_t* t)
{
    assert(e != NULL);
    assert(t != NULL);
    switch (e->type) {
        case NOP_T:
            asm_unary_expression(e->right, t);
            break;
        case INC_T:
            printf("\taddl\t");
            printf("$1, ");
            asm_unary_expression(e->left, t);
            printf("\n");
            break;
        case DEC_T:
            printf("\tsubl\t");
            printf("$1, ");
            asm_unary_expression(e->left, t);
            printf("\n");
            break;
        case ASSIGN_T:
            printf("\tmovl\t");
            asm_unary_expression(e->left, t);
            printf(", ");
            asm_unary_expression(e->right, t);
            printf("\n");
            break;
        case ADD_T:
            break;
        case SUB_T:
            break;
        case MUL_T:
            break;
    }
}

void asm_unary_expression(unary_expression_t* e, variable_table_t* t)
{
    assert(e != NULL);
    assert(t != NULL);
    switch (e->type) {
        case VALUE_T:
             asm_value(e->value, t);
            break;
        case ARRAY_T:
            break;
        case FUNCTION_T:
            break;
    }
}

void asm_value(value_t* v, variable_table_t* t)
{
    switch (v->type) {
        case IDENTIFIER_T:
            break;
        case CONST_INT_T:
            printf("%d", v->const_int);
            break;
        case CONST_FLOAT_T:
            printf("%f", v->const_float);
            break;
    }
}


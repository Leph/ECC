#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

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

    if (f->offset != 0) {
        printf("\tsubl\t$%d, %%esp\n", -f->offset);
    }

    asm_block(f->block);

    printf("\tmovl\t%%ebp, %%esp\n");
    printf("\tpopl\t%%ebp\n");
    printf("\tret\n");
}

void asm_block(block_t* b)
{
    assert(b != NULL);
    /* TODO faire allocation des tableaux */
    int i,j;
    int size=1;
    for(i=0;i<b->vt->size;i++){
      if(b->vt->table[i]->dim > 0) {
	for(j=0;j<b->vt->table[i]->dim;j++)
	  size*=b->vt->table[i]->size_array[j];
	
	printf("\tmovl\t$%d, (%%esp)\n", size);
      }
    }
    
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
            asm_condition(s->condition, t);
            break;
        case JMP_T:
            asm_jump(s->jump, t);
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
    char left[1024];
    char right[1024];
    switch (e->type) {
        case NOP_T:
            strcpy(right, asm_unary_expression(e->right, t));
            break;
        case INC_T:
            strcpy(left, asm_unary_expression(e->left, t));
            printf("\taddl\t$1, %s\n", left);
            break;
        case DEC_T:
            strcpy(left, asm_unary_expression(e->left, t));
            printf("\tsubl\t$1, %s\n", left);
            break;
        case ASSIGN_T:
            strcpy(left, asm_unary_expression(e->left, t));
            strcpy(right, asm_unary_expression(e->right, t));
            printf("\tmovl\t%s, %%ecx\n", right);
            printf("\tmovl\t%%ecx, %s\n", left);
            break;
        case ADD_T:
            strcpy(left, asm_unary_expression(e->left, t));
            strcpy(right, asm_unary_expression(e->right, t));
            printf("\tmovl\t%s, %%ecx\n", right);
            printf("\taddl\t%%ecx, %s\n", left);
            break;
        case SUB_T:
            strcpy(left, asm_unary_expression(e->left, t));
            strcpy(right, asm_unary_expression(e->right, t));
            printf("\tmovl\t%s, %%ecx\n", right);
            printf("\tsubl\t%%ecx, %s\n", left);
            break;
        case MUL_T:
            strcpy(left, asm_unary_expression(e->left, t));
            strcpy(right, asm_unary_expression(e->right, t));
            printf("\tmovl\t%s, %%ecx\n", right);
            printf("\timul\t%%ecx, %s\n", left);
            break;
    }
}

char* asm_unary_expression(unary_expression_t* e, variable_table_t* t)
{
    assert(e != NULL);
    assert(t != NULL);
    static char code[1024];
    char value[1024];
    int i;
    switch (e->type) {
        case VALUE_T:
             strcpy(code, asm_value(e->value, t));
            break;
        case ARRAY_T:
	  

            break;
        case FUNCTION_T:
            for (i=e->arguments->size-1;i>=0;i--) {
                strcpy(value, asm_value(e->arguments->values[i], t));
                printf("\tpushl\t%s\n", value);
            }
            printf("\tcall\t%s\n", e->value->identifier);
            printf("\taddl\t$%d, %%esp\n", e->arguments->size*4);
            sprintf(code, "%%eax");
            break;
    }
    return code;
}

char* asm_value(value_t* v, variable_table_t* t)
{
    assert(v != NULL);
    assert(t != NULL);
    static char code[1024];
    variable_t* tmp;
    switch (v->type) {
        case IDENTIFIER_T:
            tmp = variable_table_search_name(t, v->identifier);
            assert(tmp != NULL);
            if (tmp->is_global) {
                sprintf(code, "%s", tmp->name);
            }
            else {
                if (tmp->offset != 0) {
                    sprintf(code, "%d(%%ebp)", tmp->offset);
                }
                else {
                    sprintf(code, "(%%ebp)");
                }
            }
            break;
        case CONST_INT_T:
            sprintf(code, "$%d", v->const_int);
            break;
        case CONST_FLOAT_T:
            sprintf(code, "$%f", v->const_float);
            break;
    }
    return code;
}

void asm_jump(jump_t* j, variable_table_t* t)
{
    assert(j != NULL);
    assert(t != NULL);
    char value[1024];
    switch (j->type) {
        case GOTO_T:
            printf("\tjmp\t%s\n", j->label);
            break;
        case RETURN_T:
            printf("\tmovl\t%%ebp, %%esp\n");
            printf("\tpopl\t%%ebp\n");
            printf("\tret\n");
            break;
        case RETURN_EXP_T:
            strcpy(value, asm_unary_expression(j->exp, t));
            printf("\tmovl\t%s, %%eax\n", value);
            printf("\tmovl\t%%ebp, %%esp\n");
            printf("\tpopl\t%%ebp\n");
            printf("\tret\n");
            break;
    }
}

void asm_condition(condition_t* c, variable_table_t* t)
{
    assert(c != NULL);
    assert(t != NULL);
    switch (c->type) {
        case BOOL_T:
            break;
        case L_T:
            break;
        case G_T:
            break;
        case LE_T:
            break;
        case GE_T:
            break;
        case EQ_T:
            break;
        case NE_T:
            break;
    }
}


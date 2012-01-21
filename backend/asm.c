#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "ecc.h"
#include "asm.h"
extern function_table_t* function_table;

int label_number = 0;

void asm_global_table(variable_table_t* t)
{
    assert(t != NULL);
    printf(".data\n");
    printf("PRINT_INT:\n");
    printf("\t.string\t\"%%d\\n\"\n");
    printf("PRINT_FLOAT:\n");
    printf("\t.string\t\"%%f\\n\"\n");
    printf("PRINT_FLOAT_VECT:\n");
    printf("\t.string\t\"%%f \"\n");
    printf("PRINT_INT_VECT:\n");
    printf("\t.string\t\"%%d \"\n");
    printf("PRINT_ENDL:\n");
    printf("\t.string\t\"\\n\"\n");
    printf(".align 16\n");
    printf("SSE_CONST_ONE:\n");
    printf("\t.long\t1065353216\n");
    printf("\t.long\t1065353216\n");
    printf("\t.long\t1065353216\n");
    printf("\t.long\t1065353216\n");
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
    int i;
    //Malloc array
    for(i=0;i<b->vt->size;i++) {
        if(b->vt->table[i]->dim > 0) {
            asm_array_alloc(b->vt->table[i]);
        }
    }
    //Generate asm
    for (i=0;i<b->st->size;i++) {
        asm_statement(b->st->table[i], b->vt);
    }
    //Free array
    asm_variable_table_array_free(b->vt);
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
    char right[1024];
    type_t type_left;
    type_t type_right;
    if (e->left != NULL) {
        type_left = asm_get_type(e->left, t);
    }
    if (e->right != NULL) {
        type_right = asm_get_type(e->right, t);
    }
    switch (e->type) {
        case NOP_T:
            strcpy(right, asm_unary_expression(e->right, t));
            break;
        case INC_T:
            if (type_left == INT_T) asm_op_inc_int(e->left, t);
            else if (type_left == FLOAT_T) asm_op_inc_float(e->left, t);
            else if (type_right == FLOAT_VECTOR_T) asm_op_inc_fvect(e->left, t);
            else assert(0);
            break;
        case DEC_T:
            if (type_left == INT_T) asm_op_dec_int(e->left, t);
            else if (type_left == FLOAT_T) asm_op_dec_float(e->left, t);
            else if (type_right == FLOAT_VECTOR_T) asm_op_dec_fvect(e->left, t);
            else assert(0);
            break;
        case PRINT_T:
            if (type_right == INT_T) asm_op_print_int(e->right, t);
            else if (type_right == FLOAT_T) asm_op_print_float(e->right, t);
            else if (type_right == FLOAT_VECTOR_T) asm_op_print_fvect(e->right, t);
            else assert(0);
            break;
        case ASSIGN_T:
            if (type_left == INT_T && type_right == INT_T) asm_op_assign_int_int(e->left, e->right, t);
            else if (type_left == FLOAT_T && type_right == FLOAT_T) asm_op_assign_float_float(e->left, e->right, t);
            else if (type_left == FLOAT_VECTOR_T && type_right == FLOAT_VECTOR_T) asm_op_assign_fvect_fvect(e->left, e->right, t);
            else assert(0);
            break;
        case ADD_T:
            if (type_left == INT_T && type_right == INT_T) asm_op_add_int_int(e->left, e->right, t);
            else if (type_left == FLOAT_T && type_right == FLOAT_T) asm_op_add_float_float(e->left, e->right, t);
            else if (type_left == FLOAT_VECTOR_T && type_right == FLOAT_VECTOR_T) asm_op_add_fvect_fvect(e->left, e->right, t);
            else assert(0);
            break;
        case SUB_T:
            if (type_left == INT_T && type_right == INT_T) asm_op_sub_int_int(e->left, e->right, t);
            else if (type_left == FLOAT_T && type_right == FLOAT_T) asm_op_sub_float_float(e->left, e->right, t);
            else if (type_left == FLOAT_VECTOR_T && type_right == FLOAT_VECTOR_T) asm_op_sub_fvect_fvect(e->left, e->right, t);
            else assert(0);
            break;
        case MUL_T:
            if (type_left == INT_T && type_right == INT_T) asm_op_mul_int_int(e->left, e->right, t);
            else if (type_left == FLOAT_T && type_right == FLOAT_T) asm_op_mul_float_float(e->left, e->right, t);
            else if (type_left == FLOAT_VECTOR_T && type_right == FLOAT_VECTOR_T) asm_op_mul_fvect_fvect(e->left, e->right, t);
            else assert(0);
            break;
    }
}

char* asm_unary_expression(unary_expression_t* e, variable_table_t* t)
{
    assert(e != NULL);
    assert(t != NULL);
    static char code[1024];
    char value[1024];
    variable_t* v;
    int i;
    switch (e->type) {
        case VALUE_T:
             strcpy(code, asm_value(e->value, t));
            break;
        case ARRAY_T:
            strcpy(value, asm_value(e->value, t));
            v = variable_table_search_name(t, e->value->identifier);
            assert(v != NULL);
			assert(v->dim >= e->arguments->size);
			for (i=0;i<e->arguments->size;i++) {
				assert(e->arguments->values[i]->const_int<=v->size_array[i]);
			}
            int offset = 0;
            for (i=0;i<e->arguments->size;i++) {
                int j;
                int size = type_size(v->type);
                for (j=v->dim-1;j>i;j--) {
                    size *= v->size_array[j];
                    if (j == v->dim-1) {
                        size = 16*(size/16) + 16;
                    }
                }
                offset += size*e->arguments->values[i]->const_int;
            }
            printf("\tmovl\t%s, %%ebx\n", value);
			printf("\taddl\t$%d, %%ebx\n", offset);
            type_t type = asm_get_type(e, t);
            if (type == INT_T || type == FLOAT_T) {
                sprintf(code, "(%%ebx)");
            }
            else {
                sprintf(code, "%%ebx");
            }
            break;
        case FUNCTION_T:
            for (i=e->arguments->size-1;i>=0;i--) {
                strcpy(value, asm_value(e->arguments->values[i], t));
                printf("\tpushl\t%s\n", value);
            }
            printf("\tcall\t%s\n", e->value->identifier);
            if (e->arguments->size > 0) {
                printf("\taddl\t$%d, %%esp\n", e->arguments->size*4);
            }
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
            sprintf(code, "$0x%08x", *(int*)&v->const_float);
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
            asm_variable_table_array_free(t);
            printf("\tmovl\t%%ebp, %%esp\n");
            printf("\tpopl\t%%ebp\n");
            printf("\tret\n");
            break;
        case RETURN_EXP_T:
            strcpy(value, asm_unary_expression(j->exp, t));
            asm_variable_table_array_free(t);
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
    char e1[1024];
    char e2[1024];
    int number = label_number;
    label_number++;
    switch (c->type) {
        case BOOL_T:
            strcpy(e1, asm_unary_expression(c->e1, t));
            printf("\tmovl\t%s, %%ecx\n", e1);
            printf("\tcmp\t%%ecx, $0\n");
            printf("\tje\tL_ECC_%d\n", number);
            break;
        case L_T:
            strcpy(e1, asm_unary_expression(c->e1, t));
            strcpy(e2, asm_unary_expression(c->e2, t));
            printf("\tmovl\t%s, %%ecx\n", e1);
            printf("\tmovl\t%s, %%edx\n", e2);
            printf("\tcmp\t%%ecx, %%edx\n");
            printf("\tjge\tL_ECC_%d\n", number);
            break;
        case G_T:
            strcpy(e1, asm_unary_expression(c->e1, t));
            strcpy(e2, asm_unary_expression(c->e2, t));
            printf("\tmovl\t%s, %%ecx\n", e1);
            printf("\tmovl\t%s, %%edx\n", e2);
            printf("\tcmp\t%%ecx, %%edx\n");
            printf("\tjle\tL_ECC_%d\n", number);
            break;
        case LE_T:
            strcpy(e1, asm_unary_expression(c->e1, t));
            strcpy(e2, asm_unary_expression(c->e2, t));
            printf("\tmovl\t%s, %%ecx\n", e1);
            printf("\tmovl\t%s, %%edx\n", e2);
            printf("\tcmp\t%%ecx, %%edx\n");
            printf("\tjg\tL_ECC_%d\n", number);
            break;
        case GE_T:
            strcpy(e1, asm_unary_expression(c->e1, t));
            strcpy(e2, asm_unary_expression(c->e2, t));
            printf("\tmovl\t%s, %%ecx\n", e1);
            printf("\tmovl\t%s, %%edx\n", e2);
            printf("\tcmp\t%%ecx, %%edx\n");
            printf("\tjl\tL_ECC_%d\n", number);
            break;
        case EQ_T:
            strcpy(e1, asm_unary_expression(c->e1, t));
            strcpy(e2, asm_unary_expression(c->e2, t));
            printf("\tmovl\t%s, %%ecx\n", e1);
            printf("\tmovl\t%s, %%edx\n", e2);
            printf("\tcmp\t%%ecx, %%edx\n");
            printf("\tjne\tL_ECC_%d\n", number);
            break;
        case NE_T:
            strcpy(e1, asm_unary_expression(c->e1, t));
            strcpy(e2, asm_unary_expression(c->e2, t));
            printf("\tmovl\t%s, %%ecx\n", e1);
            printf("\tmovl\t%s, %%edx\n", e2);
            printf("\tcmp\t%%ecx, %%edx\n");
            printf("\tje\tL_ECC_%d\n", number);
            break;
    }
    asm_statement(c->statement, t);
    printf("L_ECC_%d:\n", number);
}

void asm_array_alloc(variable_t* v)
{
    assert(v != NULL);
    assert(v->dim > 0);
    int j;
    int size = type_size(v->type);
    for(j=v->dim-1;j>=0;j--) {
        size *= v->size_array[j];
        if (j == v->dim-1) {
            size = 16*(size/16) + 16;
        }
    }

    printf("\tpushl\t$%d\n", size);
    printf("\tpushl\t$16\n");
    printf("\tsubl\t$4, %%esp\n");
    printf("\tmovl\t%%esp, (%%esp)\n");
    printf("\tcall\tposix_memalign\n");
    printf("\tmovl\t(%%esp), %%ecx\n");
    printf("\tmovl\t%%ecx, %d(%%ebp)\n", v->offset);
    printf("\taddl\t$12, %%esp\n");
}

void asm_array_free(variable_t* v)
{
    assert(v != NULL);
    assert(v->dim > 0);
    printf("\tpushl\t%d(%%ebp)\n", v->offset);
    printf("\tcall\tfree\n");
    printf("\taddl\t$4, %%esp\n");
}

void asm_variable_table_array_free(variable_table_t* t)
{
    assert(t != NULL);
    int nb_table = 0;
    variable_table_t* pt = t;
    while (pt != NULL) {
        nb_table++;
        pt = pt->parent;
    }
    int i, j;
    pt = t;
    for (j=0;j<nb_table-2;j++) {
        for (i=0;i<pt->size;i++) {
            if (pt->table[i]->dim > 0) {
                asm_array_free(pt->table[i]);
            }
        }
        pt = pt->parent;
    }
}

type_t asm_get_type(unary_expression_t* e, variable_table_t* t)
{
    assert(e != NULL);
    assert(t != NULL);
    variable_t* v;
    function_t* f;
    switch (e->type) {
        case VALUE_T:
            switch (e->value->type) {
                case IDENTIFIER_T:
                    v = variable_table_search_name(t, e->value->identifier);
                    assert(v != NULL);
                    if (v->dim == 0) {
                        return v->type;
                    }
                    else if (v->dim == 1) {
                        if (v->type == INT_T) {
                            return INT_VECTOR_T;
                        }
                        else if (v->type == FLOAT_T) {
                            return FLOAT_VECTOR_T;
                        }
                        else {
                            assert(0);
                            return 0;
                        }
                    }
                    else {
                        assert(0);
                        return 0;
                    }
                    break;
                case CONST_INT_T:
                    return INT_T;
                    break;
                case CONST_FLOAT_T:
                    return FLOAT_T;
                    break;
                default: 
                    return 0; 
                    break;
            }
            break;
        case ARRAY_T:
            v = variable_table_search_name(t, e->value->identifier);
            assert(v != NULL);
            if (v->dim == e->arguments->size) {
                return v->type;
            }
            else if (e->arguments->size == v->dim -1) {
                if (v->type == INT_T) {
                    return INT_VECTOR_T;
                }
                else if (v->type == FLOAT_T) {
                    return FLOAT_VECTOR_T;
                }
                else {
                    assert(0);
                    return 0;
                }
            }
            else {
                assert(0);
                return 0;
            }
            break;
        case FUNCTION_T:
            f = function_table_search_name(function_table, e->value->identifier);
            assert(f != NULL);
            return f->type_return;
            break;
        default:
            return 0;
            break;
    }
}

void asm_op_inc_int(unary_expression_t* e_left, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(t != NULL);
    char left[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tadd\t$1, %s\n", left);
}
void asm_op_dec_int(unary_expression_t* e_left, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(t != NULL);
    char left[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tsub\t$1, %s\n", left);
}
void asm_op_print_int(unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_right != NULL);
    assert(t != NULL);
    char right[1024];
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tpushl\t%s\n", right);
    printf("\tpushl\t$PRINT_INT\n");
    printf("\tcall\tprintf\n");
    printf("\taddl\t$8, %%esp\n");
}
void asm_op_assign_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ecx\n", right);
    printf("\tmovl\t%%ecx, %s\n", left);
}
void asm_op_add_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ecx\n", right);
    printf("\taddl\t%%ecx, %s\n", left);
}
void asm_op_sub_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ecx\n", right);
    printf("\tsubl\t%%ecx, %s\n", left);
}
void asm_op_mul_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ecx\n", right);
    printf("\timull\t%%ecx, %s\n", left);
}

void asm_op_inc_float(unary_expression_t* e_left, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(t != NULL);
    char left[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tflds\t%s\n", left);
    printf("\tfld1\n");
    printf("\tfaddp\n");
    printf("\tfstps\t%s\n", left);
}
void asm_op_dec_float(unary_expression_t* e_left, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(t != NULL);
    char left[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tfld1\n");
    printf("\tflds\t%s\n", left);
    printf("\tfsubp\n");
    printf("\tfstps\t%s\n", left);
}
void asm_op_print_float(unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_right != NULL);
    assert(t != NULL);
    char right[1024];
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tsubl\t$8, %%esp\n");
    printf("\tflds\t%s\n", right);
    printf("\tfstpl\t(%%esp)\n");
    printf("\tpushl\t$PRINT_FLOAT\n");
    printf("\tcall\tprintf\n");
    printf("\taddl\t$12, %%esp\n");
}
void asm_op_assign_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ecx\n", right);
    printf("\tmovl\t%%ecx, %s\n", left);
}
void asm_op_add_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tflds\t%s\n", left);
    printf("\tflds\t%s\n", right);
    printf("\tfaddp\n");
    printf("\tfstps\t%s\n", left);
}
void asm_op_sub_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tflds\t%s\n", right);
    printf("\tflds\t%s\n", left);
    printf("\tfsubp\n");
    printf("\tfstps\t%s\n", left);
}
void asm_op_mul_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    assert(t != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tflds\t%s\n", left);
    printf("\tflds\t%s\n", right);
    printf("\tfmulp\n");
    printf("\tfstps\t%s\n", left);
}

void asm_op_inc_fvect(unary_expression_t* e_left, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(t != NULL);
    char left[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tmovl\t%s, %%ebx\n", left);
    variable_t *v_left = variable_table_search_name(t, e_left->value->identifier);
    assert(v_left != NULL);
    int size = v_left->size_array[v_left->dim-1];
    printf("\tmovl\t$%d, %%eax\n", size);
    int number = label_number;
    label_number++;
    printf("L_ECC_%d:\n", number);
    printf("\tmovaps\t(%%ebx), %%xmm0\n");
    printf("\tmovaps\tSSE_CONST_ONE, %%xmm1\n");
    printf("\taddps\t%%xmm1, %%xmm0\n");
    printf("\tmovaps\t%%xmm0, (%%ebx)\n");
    printf("\taddl\t$16, %%ebx\n");
    printf("\tsubl\t$4, %%eax\n");
    printf("\tcmp\t$0, %%eax\n");
    printf("\tjg\tL_ECC_%d\n", number);
}
void asm_op_dec_fvect(unary_expression_t* e_left, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(t != NULL);
    char left[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tmovl\t%s, %%ebx\n", left);
    variable_t *v_left = variable_table_search_name(t, e_left->value->identifier);
    assert(v_left != NULL);
    int size = v_left->size_array[v_left->dim-1];
    printf("\tmovl\t$%d, %%eax\n", size);
    int number = label_number;
    label_number++;
    printf("L_ECC_%d:\n", number);
    printf("\tmovaps\t(%%ebx), %%xmm0\n");
    printf("\tmovaps\tSSE_CONST_ONE, %%xmm1\n");
    printf("\tsubps\t%%xmm1, %%xmm0\n");
    printf("\tmovaps\t%%xmm0, (%%ebx)\n");
    printf("\taddl\t$16, %%ebx\n");
    printf("\tsubl\t$4, %%eax\n");
    printf("\tcmp\t$0, %%eax\n");
    printf("\tjg\tL_ECC_%d\n", number);
}
void asm_op_print_fvect(unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_right != NULL);
    assert(t != NULL);
    char right[1024];
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ebx\n", right);
    variable_t *v_right = variable_table_search_name(t, e_right->value->identifier);
    assert(v_right != NULL);
    int size = v_right->size_array[v_right->dim-1];
    printf("\tmovl\t$%d, %%eax\n", size);
    int number = label_number;
    label_number++;
    printf("L_ECC_%d:\n", number);
    printf("\tpushl\t%%eax\n");
    printf("\tsubl\t$8, %%esp\n");
    printf("\tflds\t(%%ebx)\n");
    printf("\tfstpl\t(%%esp)\n");
    printf("\tpushl\t$PRINT_FLOAT_VECT\n");
    printf("\tcall\tprintf\n");
    printf("\taddl\t$12, %%esp\n");
    printf("\tpopl\t%%eax\n");
    printf("\taddl\t$4, %%ebx\n");
    printf("\tsubl\t$1, %%eax\n");
    printf("\tcmp\t$0, %%eax\n");
    printf("\tjg\tL_ECC_%d\n", number);
    printf("\tpushl\t$PRINT_ENDL\n");
    printf("\tcall\tprintf\n");
    printf("\taddl\t$4, %%esp\n");
}
void asm_op_assign_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tmovl\t%s, %%ecx\n", left);
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ebx\n", right);
    variable_t *v_left = variable_table_search_name(t, e_left->value->identifier);
    variable_t *v_right = variable_table_search_name(t, e_right->value->identifier);
    assert(v_left != NULL);
    assert(v_right != NULL);
    int size_left = v_left->size_array[v_left->dim-1];
    int size_right = v_right->size_array[v_right->dim-1];
    int size = (size_left < size_right) ? size_left : size_right;
    printf("\tmovl\t$%d, %%eax\n", size);
    int number = label_number;
    label_number++;
    printf("L_ECC_%d:\n", number);
    printf("\tmovaps\t(%%ebx), %%xmm0\n");
    printf("\tmovaps\t%%xmm0, (%%ecx)\n");
    printf("\taddl\t$16, %%ebx\n");
    printf("\taddl\t$16, %%ecx\n");
    printf("\tsubl\t$4, %%eax\n");
    printf("\tcmp\t$0, %%eax\n");
    printf("\tjg\tL_ECC_%d\n", number);
}
void asm_op_add_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tmovl\t%s, %%ecx\n", left);
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ebx\n", right);
    variable_t *v_left = variable_table_search_name(t, e_left->value->identifier);
    variable_t *v_right = variable_table_search_name(t, e_right->value->identifier);
    assert(v_left != NULL);
    assert(v_right != NULL);
    int size_left = v_left->size_array[v_left->dim-1];
    int size_right = v_right->size_array[v_right->dim-1];
    int size = (size_left < size_right) ? size_left : size_right;
    printf("\tmovl\t$%d, %%eax\n", size);
    int number = label_number;
    label_number++;
    printf("L_ECC_%d:\n", number);
    printf("\tmovaps\t(%%ebx), %%xmm0\n");
    printf("\tmovaps\t(%%ecx), %%xmm1\n");
    printf("\taddps\t%%xmm0, %%xmm1\n");
    printf("\tmovaps\t%%xmm1, (%%ecx)\n");
    printf("\taddl\t$16, %%ebx\n");
    printf("\taddl\t$16, %%ecx\n");
    printf("\tsubl\t$4, %%eax\n");
    printf("\tcmp\t$0, %%eax\n");
    printf("\tjg\tL_ECC_%d\n", number);
}
void asm_op_sub_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tmovl\t%s, %%ecx\n", left);
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ebx\n", right);
    variable_t *v_left = variable_table_search_name(t, e_left->value->identifier);
    variable_t *v_right = variable_table_search_name(t, e_right->value->identifier);
    assert(v_left != NULL);
    assert(v_right != NULL);
    int size_left = v_left->size_array[v_left->dim-1];
    int size_right = v_right->size_array[v_right->dim-1];
    int size = (size_left < size_right) ? size_left : size_right;
    printf("\tmovl\t$%d, %%eax\n", size);
    int number = label_number;
    label_number++;
    printf("L_ECC_%d:\n", number);
    printf("\tmovaps\t(%%ebx), %%xmm0\n");
    printf("\tmovaps\t(%%ecx), %%xmm1\n");
    printf("\tsubps\t%%xmm0, %%xmm1\n");
    printf("\tmovaps\t%%xmm1, (%%ecx)\n");
    printf("\taddl\t$16, %%ebx\n");
    printf("\taddl\t$16, %%ecx\n");
    printf("\tsubl\t$4, %%eax\n");
    printf("\tcmp\t$0, %%eax\n");
    printf("\tjg\tL_ECC_%d\n", number);
}
void asm_op_mul_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t)
{
    assert(e_left != NULL);
    assert(e_right != NULL);
    char left[1024];
    char right[1024];
    strcpy(left, asm_unary_expression(e_left, t));
    printf("\tmovl\t%s, %%ecx\n", left);
    strcpy(right, asm_unary_expression(e_right, t));
    printf("\tmovl\t%s, %%ebx\n", right);
    variable_t *v_left = variable_table_search_name(t, e_left->value->identifier);
    variable_t *v_right = variable_table_search_name(t, e_right->value->identifier);
    assert(v_left != NULL);
    assert(v_right != NULL);
    int size_left = v_left->size_array[v_left->dim-1];
    int size_right = v_right->size_array[v_right->dim-1];
    int size = (size_left < size_right) ? size_left : size_right;
    printf("\tmovl\t$%d, %%eax\n", size);
    int number = label_number;
    label_number++;
    printf("L_ECC_%d:\n", number);
    printf("\tmovaps\t(%%ebx), %%xmm0\n");
    printf("\tmovaps\t(%%ecx), %%xmm1\n");
    printf("\tmulps\t%%xmm0, %%xmm1\n");
    printf("\tmovaps\t%%xmm1, (%%ecx)\n");
    printf("\taddl\t$16, %%ebx\n");
    printf("\taddl\t$16, %%ecx\n");
    printf("\tsubl\t$4, %%eax\n");
    printf("\tcmp\t$0, %%eax\n");
    printf("\tjg\tL_ECC_%d\n", number);
}

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ecc.h"
#include "asm.h"

void asm_global_table(variable_table_t* t)
{
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
        printf("\taddl\t$%d, %%esp\n", f->offset);
    }

    printf("\tleave\n");
    printf("\tret\n");
}

void asm_statement(statement_t* s)
{
}


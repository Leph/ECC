#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ecc.h"
#include "asm.h"

#define INT_BYTE_SIZE 4
#define FLOAT_BYTE_SIZE 4

void asm_global_table(variable_table_t* t)
{
    printf(".data\n");
    int i;
    for (i=0;i<t->size;i++) {
        asm_global_variable(t->table[i]);
    }
}

void asm_global_variable(variable_t *var){
  assert(var != NULL);
  size_t size=1;
  size_t type_size;
  if(var->type==INT_T)
    type_size=INT_BYTE_SIZE;
  else if(var->type==FLOAT_T)
    type_size=FLOAT_BYTE_SIZE;
  
  if(var->dim==0)
    size=type_size;
  else{
    int i;
    for(i=0;i<var->dim;i++)
      size*=var->size_array[i];
  }
  printf("\t.comm \t %s, %d\n",var->name,size);
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
    printf("\tpushl \t %%ebp\n");
    printf("\tmovl \t %%esp, %%ebp\n");

    int size_locals = f->block->st->size;
    printf("");

    printf("\tleave\n");
    printf("\tret\n");
}


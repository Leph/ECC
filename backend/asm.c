#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "ecc.h"

#define INT_BYTE_SIZE 4
#define FLOAT_BYTE_SIZE 4

void global_var(variable_t *var){
  assert(var!=NULL);
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
  printf("\t.comm %s,%d\n",var->name,size);
}

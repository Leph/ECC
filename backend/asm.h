#ifndef ASM_H
#define ASM_H

void asm_global_table(variable_table_t* t);
void asm_global_variable(variable_t *var);
void asm_function_table(function_table_t* t);
void asm_function(function_t* f);

#endif

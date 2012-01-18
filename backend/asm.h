#ifndef ASM_H
#define ASM_H

void asm_global_table(variable_table_t* t);
void asm_global_variable(variable_t *var);
void asm_function_table(function_table_t* t);
void asm_function(function_t* f);
void asm_block(block_t* b);
void asm_statement(statement_t* s, variable_table_t* t);
void asm_expression(expression_t* e, variable_table_t* t);
char* asm_unary_expression(unary_expression_t* e, variable_table_t* t);
char* asm_value(value_t* v, variable_table_t* t);
void asm_jump(jump_t* j, variable_table_t* t);
void asm_condition(condition_t* c, variable_table_t* t);
void asm_array_alloc(variable_t* v);
void asm_array_free(variable_t* v);
void asm_variable_table_array_free(variable_table_t* t);

#endif

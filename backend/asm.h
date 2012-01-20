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
type_t asm_get_type(unary_expression_t* e, variable_table_t* t);

void asm_op_inc_int(unary_expression_t* e_left, variable_table_t* t);
void asm_op_dec_int(unary_expression_t* e_left, variable_table_t* t);
void asm_op_print_int(unary_expression_t* e_right, variable_table_t* t);
void asm_op_assign_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_add_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_sub_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);

void asm_op_inc_float(unary_expression_t* e_left, variable_table_t* t);
void asm_op_dec_float(unary_expression_t* e_left, variable_table_t* t);
void asm_op_print_float(unary_expression_t* e_right, variable_table_t* t);
void asm_op_assign_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_add_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_sub_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
#endif


void asm_op_assign_fvec_fvec(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_sub_fvec_fvec(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_add_fvec_fvec(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_fvec_fvec(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);



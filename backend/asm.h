#ifndef ASM_H
#define ASM_H

void asm_global_table(variable_table_t* t);
void asm_global_variable(variable_t *var);
void asm_function_table(function_table_t* t);
void asm_function(function_t* f);
void asm_block(block_t* b);
void asm_statement(statement_t* s, variable_table_t* t);
void asm_label(label_t* l);
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
void asm_op_div_int_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);

void asm_op_inc_float(unary_expression_t* e_left, variable_table_t* t);
void asm_op_dec_float(unary_expression_t* e_left, variable_table_t* t);
void asm_op_print_float(unary_expression_t* e_right, variable_table_t* t);
void asm_op_sqrt_float(unary_expression_t* e_right, variable_table_t* t);
void asm_op_assign_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_add_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_sub_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_div_float_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);

void asm_op_inc_fvect(unary_expression_t* e_left, variable_table_t* t);
void asm_op_dec_fvect(unary_expression_t* e_left, variable_table_t* t);
void asm_op_print_fvect(unary_expression_t* e_right, variable_table_t* t);
void asm_op_assign_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_add_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_sub_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_div_fvect_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);

void asm_op_inc_ivect(unary_expression_t* e_left, variable_table_t* t);
void asm_op_dec_ivect(unary_expression_t* e_left, variable_table_t* t);
void asm_op_print_ivect(unary_expression_t* e_right, variable_table_t* t);
void asm_op_assign_ivect_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_add_ivect_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_sub_ivect_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_ivect_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_div_ivect_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);

void asm_int_to_float(char* in, char* out);
void asm_float_to_int(char* in, char* out);

void asm_op_assign_int_float(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_assign_float_int(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);

void asm_op_add_int_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_max_int_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_min_int_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_add_float_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_norm_float_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_max_float_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_min_float_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_int_ivect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);
void asm_op_mul_float_fvect(unary_expression_t* e_left, unary_expression_t* e_right, variable_table_t* t);

//void asm_op_print_imat(unary_expression_t* e_right, variable_table_t* t);

#endif


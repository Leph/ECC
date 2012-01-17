%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "ecc.h"
    #include "asm.h"

    int yylex();
    int yyerror();

    variable_table_t* global_table = NULL;
    function_table_t* function_table = NULL;
%}

%union {
    char* identifier;
    int constant_int;
    float constant_float;
    type_t type;
    variable_t* variable;
    function_t* function;
    variable_table_t* variable_table;
    value_t* value;
    unary_expression_t* unary_expression;
    expression_t* expression;
    arguments_list_t* arguments_list;
    condition_t* condition;
    label_t* label;
    jump_t* jump;
    statement_t* statement;
    statement_table_t* statement_table;
    block_t* block;
};

%token<identifier> IDENTIFIER
%token<constant_int> CONSTANT_INT
%token<constant_float> CONSTANT_FLOAT
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token ASSIGN SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN
%token INT FLOAT VOID
%token IF ELSE GOTO RETURN

%type<type> type_name
%type<variable> array_declarator
%type<variable> variable_declarator
%type<variable> parameter_declaration
%type<function> parameter_declaration_list
%type<function> function_prototype
%type<function> function_definition
%type<variable_table> variable_declarator_list
%type<variable_table> declaration
%type<variable_table> declaration_list
%type<value> value_expression
%type<unary_expression> unary_expression
%type<unary_expression> left_expression
%type<unary_expression> right_expression
%type<expression> expression
%type<expression> expression_statement
%type<arguments_list> argument_expression_list
%type<arguments_list> array_expression_list
%type<condition> comparison_expression
%type<condition> selection_statement
%type<jump> jump_statement
%type<label> labeled_statement
%type<statement> statement
%type<statement_table> statement_list
%type<block> compound_statement

%start program
%%

value_expression
: IDENTIFIER { $$ = create_value_identifier($1); }
| CONSTANT_INT { $$ = create_value_const_int($1); }
| CONSTANT_FLOAT { $$ = create_value_const_float($1); }
;

array_expression_list
: '[' value_expression ']' { $$ = create_arguments_list(); arguments_list_add_arg($$, $2); }
| array_expression_list '[' value_expression ']' { $$ = $1; arguments_list_add_arg($$, $3); }
;

argument_expression_list
: value_expression { $$ = create_arguments_list(); arguments_list_add_arg($$, $1); }
| argument_expression_list ',' value_expression { $$ = $1; arguments_list_add_arg($$, $3); }
;

unary_expression
: value_expression { $$ = create_unary_expression_value($1); }
| value_expression array_expression_list { $$ = create_unary_expression_array($1, $2); }
;

left_expression
: unary_expression { $$ = $1; }
;

right_expression
: unary_expression { $$ = $1; }
| IDENTIFIER '(' ')' { value_t* v = create_value_identifier($1); arguments_list_t* l = create_arguments_list(); $$ = create_unary_expression_func(v, l); }
| IDENTIFIER '(' argument_expression_list ')' { value_t* v = create_value_identifier($1); $$ = create_unary_expression_func(v, $3); }
;

expression
: right_expression { $$ = create_expression_nop($1); }
| left_expression INC_OP { $$ = create_expression_inc($1); }
| left_expression DEC_OP { $$ = create_expression_dec($1); }
| left_expression ASSIGN right_expression { $$ = create_expression_assign($1, $3); }
| left_expression ADD_ASSIGN right_expression { $$ = create_expression_add($1, $3); }
| left_expression SUB_ASSIGN right_expression { $$ = create_expression_sub($1, $3); }
| left_expression MUL_ASSIGN right_expression { $$ = create_expression_mul($1, $3); }
;

comparison_expression
: unary_expression { $$ = create_condition_bool($1); }
| unary_expression '<' unary_expression { $$ = create_condition_l($1, $3); }
| unary_expression '>' unary_expression { $$ = create_condition_g($1, $3); }
| unary_expression LE_OP unary_expression { $$ = create_condition_le($1, $3); }
| unary_expression GE_OP unary_expression { $$ = create_condition_ge($1, $3); }
| unary_expression EQ_OP unary_expression { $$ = create_condition_eq($1, $3); }
| unary_expression NE_OP unary_expression { $$ = create_condition_ne($1, $3); }
;

selection_statement
: IF '(' comparison_expression ')' statement { $$ = $3; condition_set_statement($$, $5); }
;

jump_statement
: GOTO IDENTIFIER ';' { label_t* l = create_label($2); $$ = create_jump_goto(l); }
| RETURN left_expression ';' { $$ = create_jump_return_exp($2); }
| RETURN ';' { $$ = create_jump_return(); }
;

expression_statement
: expression ';' { $$ = $1; }
;

labeled_statement
: IDENTIFIER ':' { $$ = create_label($1); }
;

statement
: labeled_statement { $$ = create_statement_label($1); }
| expression_statement { $$ = create_statement_exp($1); }
| selection_statement { $$ = create_statement_cond($1); }
| jump_statement { $$ = create_statement_jmp($1); }
| compound_statement { $$ = create_statement_block($1); }
;

statement_list
: statement { $$ = create_statement_table(); statement_table_add($$, $1); }
| statement_list statement { $$ = $1; statement_table_add($$, $2); }
;

declaration
: type_name variable_declarator_list ';' { $$ = $2; variable_table_set_all_type($$, $1); }
;

declaration_list
: declaration { $$ = $1; }
| declaration_list declaration { $$ = variable_table_merge($1, $2); }
;

compound_statement
: '{' '}' { statement_table_t* st = create_statement_table(); variable_table_t* vt = create_variable_table(); $$ = create_block(vt, st); }
| '{' statement_list '}' { variable_table_t* vt = create_variable_table(); $$ = create_block(vt, $2); }
| '{' declaration_list statement_list '}' { $$ = create_block($2, $3); }
;

type_name
: INT { $$ = INT_T; }
| VOID { $$ = VOID_T; }
| FLOAT { $$ = FLOAT_T; }
;

array_declarator
: '[' CONSTANT_INT ']' { $$ = create_variable(); variable_add_dim($$, $2); }
| array_declarator '[' CONSTANT_INT ']' { $$ = $1; variable_add_dim($$, $3); }
;

variable_declarator
: IDENTIFIER { $$ = create_variable(); variable_set_name($$, $1); }
| IDENTIFIER array_declarator { $$ = $2; variable_set_name($$, $1);}
;

variable_declarator_list
: variable_declarator { $$ = create_variable_table(); variable_table_add_var($$, $1); }
| variable_declarator_list ',' variable_declarator { $$ = $1; variable_table_add_var($$, $3); }
;

parameter_declaration
: type_name variable_declarator { $$ = $2; variable_set_type($$, $1); }
;

parameter_declaration_list
: parameter_declaration { $$ = create_function(); function_add_param($$, $1); }
| parameter_declaration_list ',' parameter_declaration { $$ = $1; function_add_param($$, $3);}
;

function_prototype
: type_name IDENTIFIER '(' ')' { $$ = create_function(); function_set_name($$, $2); function_set_return($$, $1); }
| type_name IDENTIFIER '(' parameter_declaration_list ')' { $$ = $4; function_set_name($$, $2); function_set_return($$, $1); }
;

function_definition
: function_prototype compound_statement { $$ = $1; function_set_block($$, $2); }
;

external_declaration
: function_definition { function_table_add(function_table, $1); }
| type_name variable_declarator_list ';' { variable_table_set_all_type($2, $1); global_table = variable_table_merge(global_table, $2); }
| function_prototype ';' { delete_function($1); }
;

program
: external_declaration
| external_declaration program
;

%%

extern char yytext[];
extern int column;
extern int line;
extern FILE* yyin;

char* file_name = NULL;

int yyerror (char *s) {
    fflush (stdout);
    fprintf (stderr, "%s:%d:%d: %s\n", file_name, line, column, s);
    return EXIT_FAILURE;
}

int main (int argc, char *argv[]) {
    FILE *input = NULL;
    if (argc == 2) {
        input = fopen (argv[1], "r");
        file_name = strdup(argv[1]);
        if (input) {
            yyin = input;
        }
        else {
            fprintf (stderr, "Could not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }
    else {
        fprintf (stderr, "%s: error: no input file\n", *argv);
        return EXIT_FAILURE;
    }

    global_table = create_variable_table();
    function_table = create_function_table();

    yyparse();

    do_variable_table_links(function_table, global_table);
    do_variable_offset(function_table);

    //variable_table_print(global_table);
    //function_table_print(function_table);
    //printf("\n\n---------------------\n\n");
    asm_global_table(global_table);
    asm_function_table(function_table);

    delete_variable_table(global_table);
    delete_function_table(function_table);

    fclose(input);
    free(file_name);
    return EXIT_SUCCESS;
}


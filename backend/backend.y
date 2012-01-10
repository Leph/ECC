%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "ecc.h"

    int yylex();
    int yyerror();

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
        unary_operation_t* unary_operation;
        operation_t* operation;
        arguments_list_t* arguments_list;
};

%token<identifier> IDENTIFIER
%token<constant_int> CONSTANT_INT
%token<constant_float> CONSTANT_FLOAT
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN
%token INT FLOAT VOID
%token IF ELSE GOTO RETURN

%type<type> type_name
%type<variable> array_declarator
%type<variable> variable_declarator
%type<variable> parameter_declaration
%type<function> parameter_declaration_list
%type<function> function_prototype
%type<variable_table> variable_declarator_list
%type<variable_table> declaration
%type<variable_table> declaration_list
%type<unary_operation> primary_expression
%type<unary_operation> unary_expression
%type<operation> expression
%type<arguments_list> argument_expression_list
%type<arguments_list> array_expression_list



%start program
%%

primary_expression
: IDENTIFIER { value_t* v = create_value_identifier($1); $$ = create_unary_operation_nop(v); }
| CONSTANT_INT { value_t* v = create_value_const_int($1); $$ = create_unary_operation_nop(v); }
| CONSTANT_FLOAT { value_t* v = create_value_const_float($1); $$ = create_unary_operation_nop(v); }
| IDENTIFIER INC_OP { value_t* v = create_value_identifier($1); $$ = create_unary_operation_inc(v); }
| IDENTIFIER DEC_OP { value_t* v = create_value_identifier($1); $$ = create_unary_operation_dec(v); }
| IDENTIFIER '(' ')' { value_t* v = create_value_identifier($1); arguments_list_t* l = create_arguments_list(); $$ = create_unary_operation_func(v, l); }
| IDENTIFIER '(' argument_expression_list ')' { value_t* v = create_value_identifier($1); $$ = create_unary_operation_func(v, $3); }
| IDENTIFIER array_expression_list { value_t* v = create_value_identifier($1); $$ = create_unary_operation_array(v, $2); }
;

array_expression_list
: expression { $$ = create_arguments_list(); arguments_list_add_arg($$, $1); }
| array_expression_list '[' expression ']' { $$ = $1; arguments_list_add_arg($$, $3); }
;

argument_expression_list
: expression { $$ = create_arguments_list(); arguments_list_add_arg($$, $1); }
| argument_expression_list ',' expression { $$ = $1; arguments_list_add_arg($$, $3); }
;

unary_expression
: primary_expression
;

expression
: '+' unary_expression
| '-' unary_expression
| unary_expression '=' unary_expression
| unary_expression MUL_ASSIGN unary_expression
| unary_expression ADD_ASSIGN unary_expression
| unary_expression SUB_ASSIGN unary_expression
| unary_expression
;

comparison_expression
: unary_expression
| primary_expression '<' primary_expression
| primary_expression '>' primary_expression
| primary_expression LE_OP primary_expression
| primary_expression GE_OP primary_expression
| primary_expression EQ_OP primary_expression
| primary_expression NE_OP primary_expression
;

selection_statement
: IF '(' comparison_expression ')' statement
;

jump_statement
: GOTO IDENTIFIER ';'
| RETURN ';'
| RETURN expression ';'
;

expression_statement
: ';'
| expression ';'
;

labeled_statement
: IDENTIFIER ':'
;

statement
: labeled_statement
| expression_statement
| selection_statement
| jump_statement
| compound_statement
;

statement_list
: statement
| statement_list statement
;

declaration
: type_name variable_declarator_list ';' { $$ = $2; variable_table_set_all_type($$, $1); }
;

declaration_list
: declaration { $$ = $1; }
| declaration_list declaration { $$ = variable_table_merge($1, $2); }
;

compound_statement
: '{' '}'
| '{' statement_list '}'
| '{' declaration_list statement_list '}' { variable_table_print($2); }
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
| parameter_declaration ',' parameter_declaration_list { $$ = $3; function_add_param($$, $1);}
;

function_prototype
: type_name IDENTIFIER '(' ')' { $$ = create_function(); function_set_name($$, $2); function_set_return($$, $1); }
| type_name IDENTIFIER '(' parameter_declaration_list ')' { $$ = $4; function_set_name($$, $2); function_set_return($$, $1); }
;

function_definition
: function_prototype compound_statement { function_print($1); }
;

external_declaration
: function_definition
| type_name variable_declarator_list ';'
| function_prototype ';'
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
        free(file_name);
    }
    else {
        fprintf (stderr, "%s: error: no input file\n", *argv);
        return EXIT_FAILURE;
    }

    yyparse();

    return EXIT_SUCCESS;
}


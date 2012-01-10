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
        int constant;
        type_t type;
        variable_t* variable;
        function_t* function;
};

%token<identifier> IDENTIFIER
%token<constant> CONSTANT
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

%start program
%%

primary_expression
: IDENTIFIER
| CONSTANT
| IDENTIFIER '(' ')'
| IDENTIFIER '(' argument_expression_list ')'
| IDENTIFIER INC_OP
| IDENTIFIER DEC_OP
;

postfix_expression
: primary_expression
| postfix_expression '[' expression ']'
;

argument_expression_list
: primary_expression
| argument_expression_list ',' primary_expression
;

unary_operator
: '+'
| '-'
;

unary_expression
: postfix_expression
| INC_OP unary_expression
| DEC_OP unary_expression
| unary_operator unary_expression
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

assignment_operator
: '='
| MUL_ASSIGN
| ADD_ASSIGN
| SUB_ASSIGN
;

selection_statement
: IF '(' comparison_expression ')' statement
;

jump_statement
: GOTO IDENTIFIER ';'
| RETURN ';'
| RETURN expression ';'
;

expression
: unary_expression assignment_operator unary_expression
| unary_expression
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
: type_name variable_declarator_list ';'
;

declaration_list
: declaration
| declaration_list declaration
;

compound_statement
: '{' '}'
| '{' statement_list '}'
| '{' declaration_list statement_list '}'
;

type_name
: INT { $$ = INT_T; }
| VOID { $$ = VOID_T; }
| FLOAT { $$ = FLOAT_T; }
;

array_declarator
: '[' CONSTANT ']' { $$ = create_variable(); variable_add_dim($$, $2); }
| array_declarator '[' CONSTANT ']' { $$ = $1; variable_add_dim($$, $3); }
;

variable_declarator
: IDENTIFIER { $$ = create_variable(); variable_set_name($$, $1); }
| IDENTIFIER array_declarator { $$ = $2; variable_set_name($$, $1);}
;

variable_declarator_list
: variable_declarator
| variable_declarator ',' variable_declarator_list
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
: function_prototype compound_statement {function_print($1); printf("\n");}
;

external_declaration
: function_definition
| type_name variable_declarator_list ';'
| function_prototype ';'
;

program
: external_declaration
| program external_declaration
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


%{
    #include <stdio.h>
    #include <string.h>
    #include "ecc.h"

    int yylex ();
    int yyerror ();

    function_table_t function_table;
%}

%token<str> IDENTIFIER CONSTANT
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP
%token SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN
%token TYPE_NAME
%union {
	char *str;
}
%token INT FLOAT VOID

%token  IF ELSE GOTO RETURN

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

unary_expression
: postfix_expression
| INC_OP unary_expression
| DEC_OP unary_expression
| unary_operator unary_expression
;

unary_operator
: '+'
| '-'
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

expression
: unary_expression assignment_operator unary_expression
| unary_expression
;

assignment_operator
: '='
| MUL_ASSIGN
| ADD_ASSIGN
| SUB_ASSIGN
;

declaration
: type_name declarator_list ';'
;

declarator_list
: declarator
| declarator_list ',' declarator
;

type_name
: INT
| VOID
| FLOAT
;

declarator
: IDENTIFIER
| '(' declarator ')'
| declarator '[' CONSTANT ']'
| declarator '[' ']'
| declarator '(' parameter_list ')'
| declarator '(' ')'
;


parameter_list
: parameter_declaration
| parameter_list ',' parameter_declaration
;

parameter_declaration
: type_name declarator
;

statement
: labeled_statement
| compound_statement
| expression_statement
| selection_statement
| jump_statement
;

labeled_statement
: IDENTIFIER ':' statement
;

compound_statement
: '{' '}'
| '{' statement_list '}'
| '{' declaration_list statement_list '}'
;

declaration_list
: declaration
| declaration_list declaration
;

statement_list
: statement
| statement_list statement
;

expression_statement
: ';'
| expression ';'
;

selection_statement
: IF '(' comparison_expression ')' statement
;

jump_statement
: GOTO IDENTIFIER ';'
| RETURN ';'
| RETURN expression ';'
;

program
: external_declaration
| program external_declaration
;

external_declaration
: function_definition
| declaration
;

function_definition
: type_name declarator compound_statement
;

%%
#include <stdio.h>

extern char yytext[];
extern int column;
extern int yylineno;
extern FILE *yyin;

char *file_name = NULL;

int yyerror (char *s) {
    fflush (stdout);
    fprintf (stderr, "%s:%d:%d: %s\n", file_name, yylineno, column, s);
    return 0;
}


int main (int argc, char *argv[]) {
    FILE *input = NULL;
    if (argc==2) {
	input = fopen (argv[1], "r");
	file_name = strdup (argv[1]);
	if (input) {
	    yyin = input;
	}
	else {
	    fprintf (stderr, "Could not open %s\n", argv[1]);
	    return 1;
	}
    }
    else {
	fprintf (stderr, "%s: error: no input file\n", *argv);
	return 1;
    }

    function_table.size = 0;

    yyparse ();
    free (file_name);

    long i;
    for (i=0;i<function_table.size;i++) {
        printf("%s\n", function_table.table[i].name);
    }

    return 0;
}

D[0-9]
L[a-zA-Z_]
BLANK[ \r\t\n\t\f]

%{
    #include <stdio.h>
    #include <string.h>
    #include "ecc.h"

    #include "backend.tab.h"

    void count();
%}
%option noinput 
%option nounput
%%

"/*"([^*]|"*"[^*/])*"*/"		{ count(); }
"//".*          { count(); }
 "float "	{ count(); return(FLOAT); }
 "goto "	{ count(); return(GOTO); }
 "if "		{ count(); return(IF); }
 "int "		{ count(); return(INT); }
 "return"	{ count(); return(RETURN); }
 "print"	{ count(); return(PRINT); }
 "sqrt"	        { count(); return(SQRT); }
 "void "	{ count(); return(VOID); }

{L}({L}|{D})*	{ count(); yylval.identifier = strdup(yytext); return(IDENTIFIER); }
{D}+"."{D}*    	{ count(); yylval.constant_float = atof(yytext); return(CONSTANT_FLOAT); }
{D}+    	{ count(); yylval.constant_int = atoi(yytext); return(CONSTANT_INT); }

"norm="	{ count(); return(NORM_ASSIGN); }
"max="	{ count(); return(MAX_ASSIGN); }
"min="	{ count(); return(MIN_ASSIGN); }
"+="	{ count(); return(ADD_ASSIGN); }
"-="	{ count(); return(SUB_ASSIGN); }
"*="	{ count(); return(MUL_ASSIGN); }
"/="	{ count(); return(DIV_ASSIGN); }
"++"	{ count(); return(INC_OP); }
"--"	{ count(); return(DEC_OP); }
"<="	{ count(); return(LE_OP); }
">="	{ count(); return(GE_OP); }
"=="	{ count(); return(EQ_OP); }
"!="	{ count(); return(NE_OP); }
";"	{ count(); return(';'); }
"{"	{ count(); return('{'); }
"}"	{ count(); return('}'); }
","	{ count(); return(','); }
":"   { count(); return(':'); }
"="	{ count(); return(ASSIGN); }
"("	{ count(); return('('); }
")"	{ count(); return(')'); }
"["	{ count(); return('['); }
"]"	{ count(); return(']'); }
"."	{ count(); return('.'); }
"-"	{ count(); return('-'); }
"+"	{ count(); return('+'); }
"*"	{ count(); return('*'); }
"<"	{ count(); return('<'); }
">"	{ count(); return('>'); }
"^"	{ count(); return('^'); }
"|"	{ count(); return('|'); }

{BLANK}	{ count(); }
.	{ /* ignore bad characters */ }

%%

int yywrap() {
    return 1;
}

int column = 1;
int line = 1;

void count() {
    int i;
    for (i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            column = 0;
            line++;
        }
        else if (yytext[i] == '\t')
            column += 8 - (column % 8);
        else
            column++;
    }
}


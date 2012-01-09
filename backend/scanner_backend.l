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
 "return "	{ count(); return(RETURN); }
 "void "	{ count(); return(VOID); }

{L}({L}|{D})*	{ count(); yylval.str = strdup(yytext); return(IDENTIFIER); }
{D}+    	{ count(); yylval.str = strdup(yytext); return(CONSTANT); }

"+="	{ count(); return(ADD_ASSIGN); }
"-="	{ count(); return(SUB_ASSIGN); }
"*="	{ count(); return(MUL_ASSIGN); }
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
"="	{ count(); return('='); }
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


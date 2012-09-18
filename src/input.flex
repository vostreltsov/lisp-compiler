%{
#include <stdio.h>
#include "lexemtypes.h"
%}

%option noyywrap
%option never-interactive

DIGIT [0-9]
WORDCHAR [a-zA-Z0-9_]

%%

"("     printf("Opening paren:  %s\n", yytext);
")"     printf("Closing paren:  %s\n", yytext);
"+"     printf("Maths add:      %s\n", yytext);
"-"     printf("Maths sub:      %s\n", yytext);
"*"     printf("Maths mult:     %s\n", yytext);
"/"     printf("Maths div:      %s\n", yytext);

"(?i)defun" printf("Key word defun: %s\n", yytext);

%%

int main(int argc, char * argv[])
{
    yyin = fopen("test.txt", "r");
    if (yyin == 0)
        printf("error opening text.txt\n");
    else
    {
        yylex();
        fclose(yyin);
    }
    return 0;
}

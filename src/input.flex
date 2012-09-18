%{
#include <stdio.h>
//#include "lexemtypes.h"
// ATTENTION! LISP is case-insensitive so don't forget to use (?i: ... ) when creating rules.
%}

%option noyywrap
%option never-interactive

DIGIT           [0-9]
WORDCHAR        [a-zA-Z0-9_]
WHITESPACE      [ \t\n\r]
NOTWHITESPACE   [ \t\n\r]

%x STRING

%%

{WHITESPACE} {
    // Whitespaces.
    printf("Skipping whitespace\n");
    //printf("Whitespace:  %s\n", yytext);
}
"(" {
    // Opening parenthesis.
    printf("Opening paren:  %s\n", yytext);
}
")" {
    // Closing parenthesis.
    printf("Closing paren:  %s\n", yytext);
}
"+" {
    // Maths: addition.
    printf("Maths add:      %s\n", yytext);
}
"-" {
    // Maths: subtraction.
    printf("Maths sub:      %s\n", yytext);
}
"*" {
    // Maths: multiplication.
    printf("Maths mult:     %s\n", yytext);
}
"/" {
    // Maths: division.
    printf("Maths div:      %s\n", yytext);
}
("+"|"-")?{DIGIT}+ {
    // Numeric constant.
    printf("Numeric constant:   %s\n", yytext);
}
(?i:"#\\"{NOTWHITESPACE}) {
    // Character constant.
    printf("Character constant:   %s\n", yytext);
}
(?i:"#\\"("SPACE"|"TAB"|"NEWLINE"|"PAGE"|"RUBOUT"|"LINEFEED"|"RETURN"|"BACKSPACE")) {
    // Character constant - whitespace.
    printf("Character constant:   %s\n", yytext);
}

(?i:"defun") {
    printf("Key word defun: %s\n", yytext);
}

%%

int main(int argc, char * argv[])
{
    yyin = fopen("text.txt", "r");
    if (yyin == 0)
        printf("error opening text.txt\n");
    else
    {
        yylex();
        fclose(yyin);
    }
    return 0;
}

%{

//#include "lexemtypes.h"
// ATTENTION! LISP is case-insensitive so don't forget to use (?i: ... ) when creating rules.

char            buffer[8 * 1024];   // For storing strings, comments etc.
unsigned int    buffer_length = 0;  // Length of the buffer.

void buffer_output(char * title, char * buf, unsigned int len)
{
    printf("%s", title);
    for (unsigned int i = 0; i < len; i++)
        printf("%c", buf[i]);
    printf("\n");
}

%}

%option noyywrap
%option never-interactive

DIGIT           [0-9]
WORDCHAR        [a-zA-Z0-9_]
WHITESPACE      [ \t\n\r]
NOTWHITESPACE   [^ \t\n\r]

%x COMMENT_ML
%x STRING

%%

"#|" {
    // Multiline comment beginning.
    buffer_length = 0;
    BEGIN(COMMENT_ML);
}
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
{DIGIT}+"."? {
    // Numeric constant.
    int value;
    sscanf(yytext, "%d", &value);
    printf("Numeric constant:   %d\n", value);
}
(?i:"#\\"{NOTWHITESPACE}) {
    // Character constant.
    printf("Character constant:   %s\n", yytext);
}
(?i:"#\\"("SPACE"|"TAB"|"NEWLINE"|"PAGE"|"RUBOUT"|"LINEFEED"|"RETURN"|"BACKSPACE")) {
    // Character constant - whitespace.
    printf("Character constant:   %s\n", yytext);
}
"\"" {
    // String constant beginning.
    buffer_length = 0;
    BEGIN(STRING);
}
(?i:"defun") {
    printf("Key word defun: %s\n", yytext);
}
<COMMENT_ML>[^|] {
    // Multiline comment body.
    buffer[buffer_length++] = yytext[0];
}
<COMMENT_ML>"|" {
    // Multiline comment body.
    buffer[buffer_length++] = yytext[0];
}
<COMMENT_ML>"|#" {
    // Multiline comment ending.
    buffer_output("Comment: ", buffer, buffer_length);
    buffer_length = 0;
    BEGIN(INITIAL);
}
<STRING>[^"\""] {
    // String constant body: non-quote character.
    buffer[buffer_length++] = yytext[0];
}
<STRING>"\\\"" {
    // String constant body: escaped quote character.
    buffer[buffer_length++] = '"';
}
<STRING>"\\\\" {
    // String constant body: escaped slash character.
    buffer[buffer_length++] = '\\';
}
<STRING>"\"" {
    // String constant ending.
    buffer_output("String constant: ", buffer, buffer_length);
    buffer_length = 0;
    BEGIN(INITIAL);
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

%{

#include <string.h>
#include "parser.tab.h"

int my_powah(int base, int n)
{
    int result = 1;
    for (int i = 0; i < n; i++)
        result *= base;
    return result;
}

int nondec2dec(char * string, int base)
{
    int result = 0;
    size_t length = strlen(string);
    for (size_t i = 0; i < length; i++)
    {
        char cur = string[length - i - 1];
        int tmp = 0;
        switch (cur)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            tmp = (int)cur - 0x30;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            tmp = (int)cur - ('A' - 0xA);
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            tmp = (int)cur - ('a' - 0xA);
            break;
        default:
            tmp = 0;
        }
        result += tmp * my_powah(base, i);
    }
    return result;
}

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
%option case-insensitive
%option outfile="lexer.c"

DIGIT_BIN       [0-1]
DIGIT_OCT       [0-7]
DIGIT_DEC       [0-9]
DIGIT_HEX       [0-9a-fA-F]
WHITESPACE      [ \t\n\r]
NOTWHITESPACE   [^ \t\n\r]
SYMBOLID        [a-zA-Z0-9+\-*/@$%^&_=<>~.]

%x COMMENT_ML
%x STRING

%%

%{
char            buffer[8 * 1024];   // For storing strings, comments etc.
unsigned int    buffer_length = 0;  // Length of the buffer.
%}

"#|" {
    // Multiline comment beginning.
    buffer_length = 0;
    BEGIN(COMMENT_ML);
}
"\"" {
    // String constant beginning.
    buffer_length = 0;
    BEGIN(STRING);
}
";".* {
    // Singleline comment.
    strcpy(buffer, yytext + 1);
    buffer_length = strlen(yytext + 1);
    buffer_output("Comment:                   ", buffer, buffer_length);
}
{WHITESPACE}+ {
    // Whitespaces.
}
"(" {
    // Opening parenthesis.
    return (unsigned char)'(';
}
")" {
    // Closing parenthesis.
    return (unsigned char)')';
}
"+" {
    // Operator: addition.
    return (unsigned char)'+';
}
"-" {
    // Operator: subtraction.
    return (unsigned char)'-';
}
"*" {
    // Operator: multiplication.
    return (unsigned char)'*';
}
"/" {
    // Operator: division.
    return (unsigned char)'/';
}
">" {
    // Operator: greater.
    return (unsigned char)'>';
}
">=" {
    // Operator: greater or equal.
    return GRTR_EQ;
}
"<" {
    // Operator: less.
    return (unsigned char)'<';
}
"<=" {
    // Operator: less or equal.
    return LESS_EQ;
}
"=" {
    // Operator: equal.
    return (unsigned char)'=';
}
"and" {
    // Operator: and.
    return AND;
}
"or" {
    // Operator: or.
    return OR;
}
"not" {
    // Operator: not.
    return NOT;
}
"t" {
    // Boolean constant - true.
    printf("Boolean constant - true:   %s\n", yytext);
}
"nil" {
    // Boolean constant - false.
    printf("Boolean constant - false:  %s\n", yytext);
}
"#b"{DIGIT_BIN}+ {
    // Numeric constant - binary.
    int value = nondec2dec(yytext + 2, 2);
    printf("Numeric constant - bin:    %d\n", value);
}
"#o"{DIGIT_OCT}+ {
    // Numeric constant - octal.
    int value = nondec2dec(yytext + 2, 8);
    printf("Numeric constant - oct:    %d\n", value);
}
{DIGIT_DEC}+"."? {
    // Numeric constant.
    if (yytext[yyleng - 1] == '.')
        yytext[yyleng - 1] = '\0';
    int value = nondec2dec(yytext, 10);
    yylval.semantic_int = value;
    return INT;
    //printf("Numeric constant - dec:    %d\n", value);
}
"#x"{DIGIT_HEX}+ {
    // Numeric constant - hexadecimal.
    int value = nondec2dec(yytext + 2, 16);
    printf("Numeric constant - hex:    %d\n", value);
}
"#\\"{NOTWHITESPACE} {
    // Character constant.
    printf("Character constant:        %s\n", yytext + 2);
}
"#\\"("SPACE"|"TAB"|"NEWLINE"|"PAGE"|"RUBOUT"|"LINEFEED"|"RETURN"|"BACKSPACE") {
    // Character constant - whitespace.
    // TODO: convert to a real character.
    printf("Character constant:        %s\n", yytext + 2);
}
"loop" {
    printf("Key word:                  %s\n", yytext);
}
"for" {
    printf("Key word:                  %s\n", yytext);
}
"in" {
    printf("Key word:                  %s\n", yytext);
}
"do" {
    printf("Key word:                  %s\n", yytext);
}
"from" {
    printf("Key word:                  %s\n", yytext);
}
"to" {
    printf("Key word:                  %s\n", yytext);
}
"repeat" {
    printf("Key word:                  %s\n", yytext);
}
"while" {
    printf("Key word:                  %s\n", yytext);
}
"until" {
    printf("Key word:                  %s\n", yytext);
}
"progn" {
    printf("Key word:                  %s\n", yytext);
}
"defparameter" {
    printf("Key word:                  %s\n", yytext);
}
"setf" {
    printf("Key word:                  %s\n", yytext);
}
"let" {
    printf("Key word:                  %s\n", yytext);
}
"vector" {
    printf("Key word:                  %s\n", yytext);
}
"vector-push" {
    printf("Key word:                  %s\n", yytext);
}
"vector-pop" {
    printf("Key word:                  %s\n", yytext);
}
"elt" {
    printf("Key word:                  %s\n", yytext);
}
"list" {
    printf("Key word:                  %s\n", yytext);
}
"list-length" {
    printf("Key word:                  %s\n", yytext);
}
"length" {
    printf("Key word:                  %s\n", yytext);
}
"find" {
    printf("Key word:                  %s\n", yytext);
}
"position" {
    printf("Key word:                  %s\n", yytext);
}
"remove" {
    printf("Key word:                  %s\n", yytext);
}
"substitute" {
    printf("Key word:                  %s\n", yytext);
}
"concatenate" {
    printf("Key word:                  %s\n", yytext);
}
"if" {
    printf("Key word:                  %s\n", yytext);
}
"dotimes" {
    printf("Key word:                  %s\n", yytext);
}
"defun" {
    printf("Key word:                  %s\n", yytext);
}
"slot-value" {
    printf("Key word:                  %s\n", yytext);
}
"open" {
    printf("Key word:                  %s\n", yytext);
}
"close" {
    printf("Key word:                  %s\n", yytext);
}
"with-open-file" {
    printf("Key word:                  %s\n", yytext);
}
"format" {
    printf("Key word:                  %s\n", yytext);
}
"funcall" {
    printf("Key word:                  %s\n", yytext);
}
"defclass" {
    printf("Key word:                  %s\n", yytext);
}
"'" {
    printf("Key word:                  %s\n", yytext);
}
"print" {
    printf("Key word:                  %s\n", yytext);
}
":initform" {
    printf("Key word:                  %s\n", yytext);
}
":reader" {
    printf("Key word:                  %s\n", yytext);
}
":writer" {
    printf("Key word:                  %s\n", yytext);
}
":accessor" {
    printf("Key word:                  %s\n", yytext);
}
":initarg" {
    printf("Key word:                  %s\n", yytext);
}
":allocation" {
    printf("Key word:                  %s\n", yytext);
}
":type" {
    printf("Key word:                  %s\n", yytext);
}
":documentation" {
    printf("Key word:                  %s\n", yytext);
}
":"{SYMBOLID}+ {
    printf("Key word:                  %s\n", yytext);
}
{SYMBOLID}+ {
    printf("Symbol:                    %s\n", yytext);
}
. {
    printf("UNEXPECTED CHARACTER:      %s\n", yytext);
}
<COMMENT_ML>"|" {
    // Multiline comment body: any character.
    buffer[buffer_length++] = yytext[0];
}
<COMMENT_ML>"#" {
    // Multiline comment body: any character.
    buffer[buffer_length++] = yytext[0];
}
<COMMENT_ML>[^|#]+ {
    // Multiline comment body: any character.
    for (int i = 0; i < yyleng; i++)
        buffer[buffer_length++] = yytext[i];
}
<COMMENT_ML>"|#" {
    // Multiline comment ending.
    buffer_output("Comment:                   ", buffer, buffer_length);
    buffer_length = 0;
    BEGIN(INITIAL);
}
<COMMENT_ML><<EOF>> {
    puts("ERROR: UNCLOSED COMMENT");
    yyterminate();
}
<STRING>"\\\"" {
    // String constant body: escaped quote character.
    buffer[buffer_length++] = '"';
}
<STRING>"\\\\" {
    // String constant body: escaped slash character.
    buffer[buffer_length++] = '\\';
}
<STRING>[^"\"\\"]+ {
    // String constant body: non-quote, non-slash characters.
    for (int i = 0; i < yyleng; i++)
        buffer[buffer_length++] = yytext[i];
}
<STRING>"\\" {
    // String constant body: unrecognized slash character.
    buffer[buffer_length++] = '\\';
}
<STRING>"\"" {
    // String constant ending.
    buffer_output("String constant:           ", buffer, buffer_length);
    buffer_length = 0;
    BEGIN(INITIAL);
}
<STRING><<EOF>> {
    puts("ERROR: UNCLOSED STRING CONSTANT");
    yyterminate();
}

%%

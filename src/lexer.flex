%{

#include <string.h>
#include <ctype.h>
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

void store_function_id() {
    yylval.semantic_id = (char *)malloc(sizeof(char) * (yyleng + 1));
    strcpy(yylval.semantic_id, yytext);
    for (size_t i = 0; i < yyleng; i++)
        yylval.semantic_id[i] = tolower(yylval.semantic_id[i]);
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

%x COMMENT_ML_ST
%x STRING_ST

%%

%{
char            buffer[8 * 1024];   // For storing strings, comments etc.
unsigned int    buffer_length = 0;  // Length of the buffer.
%}

"#|" {
    // Multiline comment beginning.
    BEGIN(COMMENT_ML_ST);
}
"\"" {
    // String constant beginning.
    buffer_length = 0;
    BEGIN(STRING_ST);
}
";".* {
    // Singleline comment, do nothing.
}
{WHITESPACE}+ {
    // Whitespaces, do nothing.
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
    store_function_id();
    return ID;
}
"-" {
    // Operator: subtraction.
    store_function_id();
    return ID;
}
"*" {
    // Operator: multiplication.
     store_function_id();
    return ID;
}
"/" {
    // Operator: division.
    store_function_id();
    return ID;
}
">" {
    // Operator: greater.
    store_function_id();
    return ID;
}
">=" {
    // Operator: greater or equal.
    store_function_id();
    return ID;
}
"<" {
    // Operator: less.
    store_function_id();
    return ID;
}
"<=" {
    // Operator: less or equal.
    store_function_id();
    return ID;
}
"=" {
    // Operator: equal.
    store_function_id();
    return ID;
}
"and" {
    // Operator: and.
    store_function_id();
    return ID;
}
"or" {
    // Operator: or.
    store_function_id();
    return ID;
}
"not" {
    // Operator: not.
    store_function_id();
    return ID;
}
"t" {
    // Boolean constant - true.
    yylval.semantic_bool = 1;
    return BOOL;
}
"nil" {
    // Boolean constant - false.
    yylval.semantic_bool = 0;
    return BOOL;
}
"#b"{DIGIT_BIN}+ {
    // Numeric constant - binary.
    yylval.semantic_int = nondec2dec(yytext + 2, 2);
    return INT;
}
"#o"{DIGIT_OCT}+ {
    // Numeric constant - octal.
    yylval.semantic_int = nondec2dec(yytext + 2, 8);
    return INT;
}
{DIGIT_DEC}+"."? {
    // Numeric constant.
    if (yytext[yyleng - 1] == '.')
        yytext[yyleng - 1] = '\0';
    yylval.semantic_int = nondec2dec(yytext, 10);
    return INT;
}
"#x"{DIGIT_HEX}+ {
    // Numeric constant - hexadecimal.
    yylval.semantic_int = nondec2dec(yytext + 2, 16);
    return INT;
}
"#\\"{NOTWHITESPACE} {
    // Character constant.
    yylval.semantic_char = yytext[2];
    return CHAR;
}
"#\\"("SPACE"|"TAB"|"NEWLINE"|"PAGE"|"RUBOUT"|"LINEFEED"|"RETURN"|"BACKSPACE") {
    // Character constant - whitespace.
    yylval.semantic_char = 'q'; // TODO: convert to a real character.
    return CHAR;
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
    return (unsigned char)'\'';
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
    // User-defined symbol.
    store_function_id();
    return ID;
}
. {
    printf("UNEXPECTED CHARACTER:      %s\n", yytext);
}
<COMMENT_ML_ST>"|" {
    // Multiline comment body: any character.
}
<COMMENT_ML_ST>"#" {
    // Multiline comment body: any character.
}
<COMMENT_ML_ST>[^|#]+ {
    // Multiline comment body: any character.
}
<COMMENT_ML_ST>"|#" {
    // Multiline comment ending.
    BEGIN(INITIAL);
}
<COMMENT_ML_ST><<EOF>> {
    puts("ERROR: UNCLOSED COMMENT");
    yyterminate();
}
<STRING_ST>"\\\"" {
    // String constant body: escaped quote character.
    buffer[buffer_length++] = '"';
}
<STRING_ST>"\\\\" {
    // String constant body: escaped slash character.
    buffer[buffer_length++] = '\\';
}
<STRING_ST>[^"\"\\"]+ {
    // String constant body: non-quote, non-slash characters.
    for (int i = 0; i < yyleng; i++)
        buffer[buffer_length++] = yytext[i];
}
<STRING_ST>"\\" {
    // String constant body: unrecognized slash character.
    buffer[buffer_length++] = '\\';
}
<STRING_ST>"\"" {
    // String constant ending.
    yylval.semantic_string = (char *)malloc(sizeof(char) * (buffer_length + 1));
    strcpy(yylval.semantic_string, buffer);
    buffer_length = 0;
    BEGIN(INITIAL);
    return STRING;
}
<STRING_ST><<EOF>> {
    puts("ERROR: UNCLOSED STRING CONSTANT");
    yyterminate();
}

%%

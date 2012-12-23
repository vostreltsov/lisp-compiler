%{

#include <string.h>
#include <ctype.h>
#include "parser.hpp"
#include "errors.h"

int mah_powah(int base, int n)
{
    int result = 1;
    for (int i = 0; i < n; i++)
        result *= base;
    return result;
}

int nondec2dec(const char * string, int base)
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
        result += tmp * mah_powah(base, i);
    }
    return result;
}

float str2float(const char * string) {
    char left[256];
    char right[256];
    const char * cur = string;
    int i = 0;
    while ('0' <= *cur && *cur <= '9' && *cur != '\0') {
        left[i++] = *cur;
        cur++;
    }
    left[i] = '\0';
    cur++;  // skip '.'
    i = 0;
    while ('0' <= *cur && *cur <= '9' && *cur != '\0') {
        right[i++] = *cur;
        cur++;
    }
    right[i] = '\0';
    return (float)nondec2dec(left, 10) + (float)nondec2dec(right, 10) / (float)mah_powah(10, strlen(right));
}

void store_function_id(const char * id) {
    size_t length = strlen(id);
    yylval.semantic_id = (char *)malloc(sizeof(char) * (length + 1));
    strcpy(yylval.semantic_id, id);
    for (size_t i = 0; i < length; i++)
        yylval.semantic_id[i] = tolower(yylval.semantic_id[i]);
}

%}

%option noyywrap
%option never-interactive
%option case-insensitive
%option outfile="lexer.cpp"

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
"'" {
    return (unsigned char)'\'';
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
    store_function_id("plus");  // Convert to the RTL method name, same for the below rules.
    return ID;
}
"-" {
    // Operator: subtraction.
    store_function_id("minus");
    return ID;
}
"*" {
    // Operator: multiplication.
     store_function_id("mult");
    return ID;
}
"/" {
    // Operator: division.
    store_function_id("div");
    return ID;
}
">" {
    // Operator: greater.
    store_function_id("greater");
    return ID;
}
">=" {
    // Operator: greater or equal.
    store_function_id("greater_eq");
    return ID;
}
"<" {
    // Operator: less.
    store_function_id("less");
    return ID;
}
"<=" {
    // Operator: less or equal.
    store_function_id("less_eq");
    return ID;
}
"=" {
    // Operator: equal.
    store_function_id("eq");
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
"-"?"#b"{DIGIT_BIN}+ {
    // Numeric constant - binary.
    int shift = (yytext[0] == '-') ? 1 : 0;
    int sign = shift ? -1 : 1;
    yylval.semantic_int = sign * nondec2dec(yytext + 2 + shift, 2);
    return INT;
}
"-"?"#o"{DIGIT_OCT}+ {
    // Numeric constant - octal.
    int shift = (yytext[0] == '-') ? 1 : 0;
    int sign = shift ? -1 : 1;
    yylval.semantic_int = sign * nondec2dec(yytext + 2 + shift, 8);
    return INT;
}
"-"?{DIGIT_DEC}+ {
    // Numeric constant.
    int shift = (yytext[0] == '-') ? 1 : 0;
    int sign = shift ? -1 : 1;
    yylval.semantic_int = sign * nondec2dec(yytext + shift, 10);
    return INT;
}
"-"?"#x"{DIGIT_HEX}+ {
    // Numeric constant - hexadecimal.
    int shift = (yytext[0] == '-') ? 1 : 0;
    int sign = shift ? -1 : 1;
    yylval.semantic_int = sign * nondec2dec(yytext + 2 + shift, 16);
    return INT;
}
"-"?{DIGIT_DEC}*"."{DIGIT_DEC}* {
    // Floating-point constant.
    int shift = (yytext[0] == '-') ? 1 : 0;
    int sign = shift ? -1 : 1;
    yylval.semantic_float = sign * str2float(yytext + shift);
    return FLOAT;
}
"#\\"{NOTWHITESPACE} {
    // Character constant.
    yylval.semantic_char = yytext[2];
    return CHAR;
}
"#\\"("SPACE"|"TAB"|"NEWLINE"|"PAGE"|"RUBOUT"|"LINEFEED"|"RETURN"|"BACKSPACE") {
    // Character constant - whitespace.
    char upper[32];
    strcpy(upper, yytext + 2);
    for (size_t i = 0; i < yyleng - 2; i++) {
        upper[i] = toupper(upper[i]);
    }
    if (strcmp(upper, "SPACE") == 0) {
        yylval.semantic_char = 0x20;
    } else if (strcmp(upper, "TAB") == 0) {
        yylval.semantic_char = 0x09;
    } else if (strcmp(upper, "NEWLINE") == 0) {
        yylval.semantic_char = 0x0A;
    } else if (strcmp(upper, "PAGE") == 0) {
        yylval.semantic_char = 0x0C;
    } else if (strcmp(upper, "RUBOUT") == 0) {
        yylval.semantic_char = 0x7F;
    } else if (strcmp(upper, "LINEFEED") == 0) {
        yylval.semantic_char = 0x0A;
    } else if (strcmp(upper, "RETURN") == 0) {
        yylval.semantic_char = 0x0D;
    } else if (strcmp(upper, "BACKSPACE") == 0) {
        yylval.semantic_char = 0x08;
    } else {
        yylval.semantic_char = 0x00;
    }
    return CHAR;
}
"loop" {
    return LOOP;
}
"for" {
    return FOR;
}
"in" {
    return IN;
}
"do" {
    return DO;
}
"from" {
    return FROM;
}
"to" {
    return TO;
}
"downto" {
    return DOWNTO;
}
"while" {
    return WHILE;
}
"progn" {
    return PROGN;
}
"if" {
    return IF;
}
"defclass" {
    return DEFCLASS;
}
"defun" {
    return DEFUN;
}
":initform" {
    return INITFORM;
}
":reader" {
    return READER;
}
":writer" {
    return WRITER;
}
":accessor" {
    return ACCESSOR;
}
":allocation" {
    return ALLOCATION;
}
":instance" {
    return ALLOCINSTANCE;
}
":class" {
    return ALLOCCLASS;
}
"make-instance" {
    return MAKEINSTANCE;
}
"slot-value" {
    return SLOTVALUE;
}
{SYMBOLID}+ {
    store_function_id(yytext);
    return ID;
}
"|".+"|" {
    // User-defined symbol.
    store_function_id(yytext);
    return ID;
}
. {
    return ERROR_UNEXPECTED_CHARACTER;
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
    return ERROR_UNCLOSED_COMMENT;
}
<STRING_ST>"\\\"" {
    // String constant body: escaped quote character.
    buffer[buffer_length++] = '"';
}
<STRING_ST>"\\\\" {
    // String constant body: escaped slash character.
    buffer[buffer_length++] = '\\';
}
<STRING_ST>"\\n" {
    // String constant body: newline.
    buffer[buffer_length++] = '\n';
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
    buffer[buffer_length] = '\0';
    yylval.semantic_string = (char *)malloc(sizeof(char) * (buffer_length + 1));
    strcpy(yylval.semantic_string, buffer);
    buffer_length = 0;
    BEGIN(INITIAL);
    return STRING;
}
<STRING_ST><<EOF>> {
    return ERROR_UNCLOSED_STRING;
}

%%

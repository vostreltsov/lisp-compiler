%{

//#include "lexemtypes.h"
// ATTENTION! LISP is case-insensitive so don't forget to use (?i: ... ) when creating rules.

char            buffer[8 * 1024];   // For storing strings, comments etc.
unsigned int    buffer_length = 0;  // Length of the buffer.

int my_powah(int base, int n)
{
    int result = 1;
    for (int i = 0; i < n; i++)
        result *= base;
    return result;
}

int nondec2dec(char * string, int base) {
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

DIGIT_BIN       [0-1]
DIGIT_OCT       [0-7]
DIGIT_DEC       [0-9]
DIGIT_HEX       [0-9a-fA-F]
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
(?i:"#"b{DIGIT_BIN}+) {
    // Numeric constant - binary.
    int value = nondec2dec(yytext + 2, 2);
    printf("Numeric constant - binary:   %d\n", value);
}
(?i:"#"o{DIGIT_OCT}+) {
    // Numeric constant - octal.
    int value = nondec2dec(yytext + 2, 8);
    printf("Numeric constant - octal:   %d\n", value);
}
{DIGIT_DEC}+"."? {
    // Numeric constant.
    if (yytext[yyleng - 1] == '.')
        yytext[yyleng - 1] = '\0';
    int value = nondec2dec(yytext, 10);
    printf("Numeric constant:   %d\n", value);
}
(?i:"#"x{DIGIT_HEX}+) {
    // Numeric constant - hexadecimal.
    int value = nondec2dec(yytext + 2, 16);
    printf("Numeric constant - hexadecimal:   %d\n", value);
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
<COMMENT_ML>"|#" {
    // Multiline comment ending.
    buffer_output("Comment: ", buffer, buffer_length);
    buffer_length = 0;
    BEGIN(INITIAL);
}
<COMMENT_ML>[^|] {
    // Multiline comment body.
    buffer[buffer_length++] = yytext[0];
}
<COMMENT_ML>"|" {
    // Multiline comment body.
    buffer[buffer_length++] = yytext[0];
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

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
WHITESPACE      [ \t\n\r]
NOTWHITESPACE   [^ \t\n\r]
ALPHA           [a-zA-Z_]

%x COMMENT_ML
%x STRING

%%

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
{WHITESPACE} {
    // Whitespaces.
    printf("Skipping whitespace\n");
    //printf("Whitespace:  %s\n", yytext);
}
"(" {
    // Opening parenthesis.
    printf("Opening paren:             %s\n", yytext);
}
")" {
    // Closing parenthesis.
    printf("Closing paren:             %s\n", yytext);
}
"+" {
    // Operator: addition.
    printf("Operator add:              %s\n", yytext);
}
"-" {
    // Operator: subtraction.
    printf("Operator sub:              %s\n", yytext);
}
"*" {
    // Operator: multiplication.
    printf("Operator mult:             %s\n", yytext);
}
"/" {
    // Operator: division.
    printf("Operator div:              %s\n", yytext);
}
">" {
    // Operator: greater.
    printf("Operator greater:          %s\n", yytext);
}
">=" {
    // Operator: greater or equal.
    printf("Operator greater or equal: %s\n", yytext);
}
"<" {
    // Operator: less.
    printf("Operator less:             %s\n", yytext);
}
"<=" {
    // Operator: less or equal.
    printf("Operator less or equal:    %s\n", yytext);
}
"=" {
    // Operator: equal.
    printf("Operator equal:            %s\n", yytext);
}
"and" {
    // Operator: and.
    printf("Operator and:              %s\n", yytext);
}
"or" {
    // Operator: or.
    printf("Operator or:               %s\n", yytext);
}
"not" {
    // Operator: not.
    printf("Operator not:              %s\n", yytext);
}
(?i:"#b"{DIGIT_BIN}+) {
    // Numeric constant - binary.
    int value = nondec2dec(yytext + 2, 2);
    printf("Numeric constant - bin:    %d\n", value);
}
(?i:"#o"{DIGIT_OCT}+) {
    // Numeric constant - octal.
    int value = nondec2dec(yytext + 2, 8);
    printf("Numeric constant - oct:    %d\n", value);
}
{DIGIT_DEC}+"."? {
    // Numeric constant.
    if (yytext[yyleng - 1] == '.')
        yytext[yyleng - 1] = '\0';
    int value = nondec2dec(yytext, 10);
    printf("Numeric constant - dec:    %d\n", value);
}
(?i:"#x"{DIGIT_HEX}+) {
    // Numeric constant - hexadecimal.
    int value = nondec2dec(yytext + 2, 16);
    printf("Numeric constant - hex:    %d\n", value);
}
(?i:"#\\"{NOTWHITESPACE}) {
    // Character constant.
    printf("Character constant:        %s\n", yytext + 2);
}
(?i:"#\\"("SPACE"|"TAB"|"NEWLINE"|"PAGE"|"RUBOUT"|"LINEFEED"|"RETURN"|"BACKSPACE")) {
    // Character constant - whitespace.
    // TODO: convert to a real character.
    printf("Character constant:        %s\n", yytext + 2);
}
(?i:"loop") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"for") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"in") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"do") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"from") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"to") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"repeat") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"while") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"until") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"progn") {
    printf("Key word:                  %s\n", yytext);
}
(?i:"defun") {
    printf("Key word:                  %s\n", yytext);
}
{ALPHA}({ALPHA}|{DIGIT_DEC})* {
    printf("Variable:                  %s\n", yytext);
}
<COMMENT_ML>"|#" {
    // Multiline comment ending.
    buffer_output("Comment:                   ", buffer, buffer_length);
    buffer_length = 0;
    BEGIN(INITIAL);
}
<COMMENT_ML>[^"|"]|"|" {
    // Multiline comment body: any character.
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
    buffer_output("String constant:           ", buffer, buffer_length);
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

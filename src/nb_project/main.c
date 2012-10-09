#include <stdio.h>
#include <stdlib.h>
#include "../parser.tab.h"
#include "../lexer.c"
#include "../parser.tab.c"

int main(int argc, char * argv[]) {
    yyin = fopen("../../misc/text.txt", "r");
    if (yyin == 0) {
        printf("error opening text.txt\n");
    } else {
        yylex();
        fclose(yyin);
    }
    return 0;
}

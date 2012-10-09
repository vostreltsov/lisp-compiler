#include <stdio.h>
#include <stdlib.h>
#include "../parser_structs.h"

extern int yyparse();
extern FILE * yyin;
extern struct program_struct * root;

int main(int argc, char * argv[]) {
    yyin = fopen("../../misc/text.txt", "r");
    if (yyin == 0) {
        printf("error opening text.txt\n");
    } else {
        yyparse();
        struct program_struct * qwe = root;
        printf("%d", qwe->s_expr->const_int);
        fclose(yyin);
    }
    return 0;
}

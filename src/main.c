#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser_structs.h"
#include "dotcode.h"

extern int yyparse();
extern FILE * yyin;
extern struct program_struct * root;

void run_dot(struct program_struct * program) {
    char * dotFN = "tmp.dot";
    char * pngFN = "res.png";

    FILE * dot = fopen(dotFN, "w");
    if (dot == 0) {
        printf("error opening %s\n", pngFN);
    } else {
        dot_for_program(dot, program);
        fclose(dot);
        exec_dot(dotFN, pngFN);
        char cmd[256];
        sprintf(cmd, "ristretto %s", pngFN);
        system(cmd);    // run image viewer automatically.
        unlink(dotFN);
    }
}

int main(int argc, char * argv[]) {
    yyin = fopen("../misc/text.txt", "r");
    if (yyin == 0) {
        printf("error opening text.txt\n");
    } else {
        yyparse();
        fclose(yyin);
        run_dot(root);
    }
    return 0;
}

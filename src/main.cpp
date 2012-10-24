#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <stdio.h>
#include <stdlib.h>
#include "parser_structs.h"
#include "parser_funcs.h"
#include "dotcode.h"

extern int yyparse();
extern FILE * yyin;
extern struct program_struct * root;

void run_dot(struct program_struct * program) {
    char * dotFN = "tmp.dot";
    char * pngFN = "res.png";

    QFile dot(dotFN);
    if (dot.open(QFile::WriteOnly)) {
        QTextStream out(&dot);
        DotCode::dot_for_program(out, program);
        dot.close();
        DotCode::exec_dot("dot", dotFN, pngFN);
        char cmd[256];
        sprintf(cmd, "ristretto %s", pngFN);
        system(cmd);    // run image viewer automatically.
        QFile::remove(dotFN);
    } else {
        printf("error opening %s\n", pngFN);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    yyin = fopen("../misc/text.txt", "r");
    if (yyin == 0) {
        printf("error opening text.txt\n");
    } else {
        yyparse();
        fclose(yyin);
        run_dot(root);
        free_program(root);
    }
    return /*a.exec()*/0;
}

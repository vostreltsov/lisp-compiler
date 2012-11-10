#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <stdio.h>
#include <stdlib.h>
#include "parser_structs.h"
#include "parser_funcs.h"
#include "errors.h"
#include "semanticanalyzer.h"

extern int yyparse();
extern int errorCode;
extern FILE * yyin;
extern struct program_struct * root;

void exec_dot(const QString & dotBinFileName, const QString & dotFileName, const QString & pngFileName) {
    QStringList args;
    args << QString("-Tpng") << QString("-o") + pngFileName << dotFileName;
    QProcess::execute(dotBinFileName, args);
}

void run_don_on_attr_node(ProgramNode * program, QString fileName) {
    if (program == NULL) {
        return;
    }
    QString dotFN = "tmp.dot";
    QFile dot(dotFN);
    if (dot.open(QFile::WriteOnly)) {
        QTextStream out(&dot);
        out << program->dotCode("", "");
        dot.close();
        exec_dot("dot", dotFN, fileName);
        // Run image viewer automatically.
        QStringList args;
        args << fileName;
        QProcess::execute("ristretto", args);
        QFile::remove(dotFN);
    } else {
        // Error occured, don't tell anybody about it!
    }
}

void drawProgram(const char * program, const char * image)
{
    yyin = fopen(program, "r");
    if (yyin == 0) {
        printf("error opening %s\n", program);
    } else {
        yyparse();
        if (errorCode == ERROR_NO_ERROR) {
            SemanticAnalyzer * sem = new SemanticAnalyzer(root);
            sem->doTransform();
            run_don_on_attr_node(sem->getRoot(), image);
            delete sem;
        }
        fclose(yyin);
        free_program(root);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "--draw") == 0) {
            if (argc >= 4) {
                drawProgram(argv[2], argv[3]);
            } else {
                puts("too few parameters");
            }
        } else {
            puts("use --draw program.cl out.png for drawing");
        }

    } else if (argc == 1) {
        drawProgram("../misc/example.cl", "attr.png");
    }
    return 0;
}

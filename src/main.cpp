#include <QCoreApplication>
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    yyin = fopen("../misc/example.cl", "r");
    if (yyin == 0) {
        printf("error opening example.cl\n");
    } else {
        yyparse();
        switch (errorCode) {
        case ERROR_NO_ERROR: {
            SemanticAnalyzer * sem = new SemanticAnalyzer(root);
            // Transform the tree.
            sem->doTransform();
            // Perform semantic checks, generate tables, etc.
            if (sem->doSemantics()) {
                // Semantics passed, generate java bytecode.
                // TODO
            } else {
                // Display semantic errors.
                foreach (QString str, sem->getErrors()) {
                    QTextStream(stdout) << str << "\n";
                }
            }
            run_don_on_attr_node(sem->getRoot(), "attr.res");
            delete sem;
            break;
        }
        case ERROR_LEXICAL_UNCLOSED_COMMENT: {
            puts("lexical error: unclosed comment");
            break;
        }
        case ERROR_LEXICAL_UNCLOSED_STRING: {
            puts("lexical error: unclosed string constant");
            break;
        }
        case ERROR_LEXICAL_UNEXPECTED_CHARACTER: {
            puts("lexical error: unexpected character");
            break;
        }
        case ERROR_SYNTAX: {
            puts("syntax");
            break;
        }
        default: {
            break;
        }
        }
        fclose(yyin);
        free_program(root);
    }
    return /*a.exec()*/0;
}

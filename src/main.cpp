#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <stdio.h>
#include <stdlib.h>
#include "parser_structs.h"
#include "parser_funcs.h"
#include "syntaxdotcode.h"
#include "attrnodes.h"

extern int yyparse();
extern FILE * yyin;
extern struct program_struct * root;

void exec_dot(const QString & dotBinFileName, const QString & dotFileName, const QString & pngFileName) {
    QStringList args;
    args << QString("-Tpng") << QString("-o") + pngFileName << dotFileName;
    QProcess::execute(dotBinFileName, args);
}

void run_dot_on_syntax_node(struct program_struct * program, QString fileName) {
    QString dotFN = "tmp.dot";
    QFile dot(dotFN);
    if (dot.open(QFile::WriteOnly)) {
        QTextStream out(&dot);
        SyntaxDotCode::dot_for_program(out, program);
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

void run_don_on_attr_node(ProgramNode * program, QString fileName) {
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
    yyin = fopen("../misc/text.txt", "r");
    if (yyin == 0) {
        printf("error opening text.txt\n");
    } else {
        yyparse();
        fclose(yyin);
        ProgramNode * program = ProgramNode::fromSyntaxNode(root);
        //run_dot_on_syntax_node(root, "syntax.png");
        run_don_on_attr_node(program, "attr.res");
        free_program(root);
        delete program;
    }
    return /*a.exec()*/0;
}

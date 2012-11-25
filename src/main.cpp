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
#include "compiler.h"

extern int yyparse();
extern int errorCode;
extern char errorMessage[256];
extern FILE * yyin;
extern struct program_struct * root;

void exec_dot(const QString & dotBinFileName, const QString & dotFileName, const QString & pngFileName) {
    QStringList args;
    args << QString("-Tpng") << QString("-o") + pngFileName << dotFileName;
    QProcess::execute(dotBinFileName, args);
}

void run_don_on_program(SemanticProgram & program, QString fileName, bool showResult) {
    QString dotFN = "tmp.dot";
    QFile dot(dotFN);
    if (dot.open(QFile::WriteOnly)) {
        QTextStream out(&dot);
        QString dotCode = "strict digraph {\n" + program.dotCode() + "}\n";
        out << dotCode;
        dot.close();
        exec_dot("dot", dotFN, fileName);
        // Run image viewer automatically.
        QStringList args;
        args << fileName;
        if (showResult) {
            QProcess::execute("ristretto", args);
        }
        QFile::remove(dotFN);
    } else {
        // Error occured, don't tell anybody about it!
    }
}

void compile(const char * program, const char * image, bool showTree)
{
    // Open the input file.
    yyin = fopen(program, "r");
    if (yyin == 0) {
        printf("error opening %s\n", program);
        return;
    }

    // Parse the sources.
    yyparse();
    fclose(yyin);

    // There could be a lexical or syntax error.
    if (errorCode != ERROR_NO_ERROR) {
        QTextStream(stdout) << errorMessage << "\n";
        return;
    }

    // Do semantic analysis, generate bytecode.
    SemanticProgram sem(root);
    sem.doTransform();
    sem.doSemantics();
    QStringList errors = sem.errors();
    if (errors.isEmpty()) {
        QString binDirName = "bin-" + QFileInfo(program).fileName();
        sem.doGenerateCode(binDirName);
    } else {
        foreach (QString error, sem.errors()) {
            QTextStream(stdout) << error << "\n";
        }
    }

    // Draw the sytax tree if requested.
    if (image != NULL) {
        run_don_on_program(sem, image, showTree);
    }

    // Delete the syntax tree.
    free_program(root);
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "--draw") == 0) {
            if (argc >= 4) {
                compile(argv[2], argv[3], false);
            } else {
                puts("too few parameters");
            }
        } else {
            puts("use --draw program.cl out.png for drawing");
        }

    } else if (argc == 1) {
        compile("../examples/correct/maths.cl", "../examples/correct/images/maths.png", false);
    }
    return 0;
}

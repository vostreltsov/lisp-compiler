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

void compile(QString sourceFileName, QString destFolder, QString imageFileName, QString tablesFileName)
{
    QTextStream stream(stdout);

    // Open the input file.
    yyin = fopen(sourceFileName.toStdString().c_str(), "r");
    if (yyin == 0) {
        stream << "error opening " << sourceFileName << "\n";
        return;
    }

    // Parse the sources.
    yyparse();
    fclose(yyin);

    // There could be a lexical or syntax error.
    if (errorCode != ERROR_NO_ERROR) {
        stream << errorMessage << ".\n";
        return;
    }

    // Do semantic analysis, generate bytecode.
    SemanticProgram sem(root);
    sem.doTransform();
    sem.doSemantics();
    QStringList errors = sem.errors();
    if (!errors.isEmpty()) {
        foreach (QString error, sem.errors()) {
            stream << error << "\n";
        }
        return;
    }

    sem.doGenerateCode(destFolder);

    // Draw the sytax tree if requested.
    if (!imageFileName.isEmpty()) {
        run_don_on_program(sem, imageFileName, false);
    }

    // Write tables if requested.
    if (!tablesFileName.isEmpty()) {
        QFile file(tablesFileName);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream tablesStream(&file);
            tablesStream << sem.tablesToString();
            file.close();
        } else {
            stream << "Could not open file " + tablesFileName + "\n";
        }
    }

    // Delete the syntax tree.
    free_program(root);
}

void help()
{
    QTextStream stream(stdout);
    QString thisFileName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    stream << "Usage: " << thisFileName << " [options]\n";
    stream << "Required options:\n";
    stream << "-i file    Specifies the input file.\n";
    stream << "\n";
    stream << "Additional options:\n";
    stream << "-o folder  Specifies the output folder to store the MainClass.class.\n";
    stream << "-s file    Specifies the image file name to store the syntax tree (graphviz required).\n";
    stream << "-t file    Specifies the html file name to store generated tables.\n";
    stream << "-h         Prints this message.\n";
}

int main(int argc, char *argv[])
{

#ifdef QT_DEBUG
    compile("../examples/correct/quicksort.cl", "../examples/correct/bin", "", "");
    return 0;
#else
    QCoreApplication a(argc, argv);
    QTextStream stream(stdout);

    if (argc == 1) {
        help();
        return 0;
    }

    // Parse command line arguments.
    QString src;
    QString dst;
    QString img;
    QString tab;
    QStringList unknown;

    int index = 1;
    while (index < argc) {
        QString cur = argv[index];
        if (cur == "-h") {
            if (index == 1) {
                help();
                return 0;
            }
        } else if (cur == "-i") {
            if (index == argc - 1) {
                stream << "Too few arguments for -i.\n";
                help();
                return 0;
            }
            src = argv[++index];
        } else if (cur == "-o") {
            if (index == argc - 1) {
                stream << "Too few arguments for -o.\n";
                help();
                return 0;
            }
            dst = argv[++index];
        } else if (cur == "-s") {
            if (index == argc - 1) {
                stream << "Too few arguments for -s.\n";
                help();
                return 0;
            }
            img = argv[++index];
        } else if (cur == "-t") {
            if (index == argc - 1) {
                stream << "Too few arguments for -t.\n";
                help();
                return 0;
            }
            tab = argv[++index];
        } else {
            unknown << argv[index];
        }
        index++;
    }

    if (!unknown.isEmpty()) {
        stream << "Unknown parameter(s):\n";
        foreach (QString err, unknown) {
            stream << err << "\n";
        }
        stream << "Use -h to get some help.\n";
        return 0;
    }

    if (src.isEmpty()) {
        stream << "You should specify the source file.\n";
        return 0;
    }

    // Calculate output folder if not specified.
    if (dst.isEmpty()) {
        QFileInfo srcInfo(src);
        dst = srcInfo.dir().absolutePath() + "/bin-" + srcInfo.fileName();
    }

    compile(src, dst, img, tab);

    return 0;
#endif
}

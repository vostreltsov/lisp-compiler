#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "nodetypes.h"
#include "attrnodes.h"

enum JavaConstantsTypes
{
    CONSTANT_Utf8 = 1,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_Class = 7 ,
    CONSTANT_String = 8,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_NameAndType = 12
};

class SemanticAnalyzer
{
public:
    SemanticAnalyzer();
    SemanticAnalyzer(const program_struct * root);
    ~SemanticAnalyzer();
    ProgramNode * getRoot() const;
    QLinkedList<QString> getErrors() const;
    bool doCheck();
    void doTransform();
private:
    ProgramNode * program;
    QLinkedList<QString> errors;
};

#endif // SEMANTICANALYZER_H

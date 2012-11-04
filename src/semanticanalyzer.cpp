#include "semanticanalyzer.h"

SemanticAnalyzer::SemanticAnalyzer()
{
    program = NULL;
}

SemanticAnalyzer::SemanticAnalyzer(const program_struct * root)
{
    program = ProgramNode::fromSyntaxNode(root);
}

SemanticAnalyzer::~SemanticAnalyzer()
{
    if (program != NULL) {
        delete program;
    }
}

ProgramNode * SemanticAnalyzer::getRoot() const
{
    return program;
}

QLinkedList<QString> SemanticAnalyzer::getErrors() const
{
    return errors;
}

bool SemanticAnalyzer::doCheck()
{
    errors.clear();
    if (program != NULL) {
        program->check(&errors);
    }
    return errors.empty();
}

void SemanticAnalyzer::doTransform()
{
    if (program != NULL) {
        program->transform();
    }
}

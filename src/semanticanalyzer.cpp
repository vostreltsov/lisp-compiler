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

#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "nodetypes.h"
#include "attrnodes.h"

class SemanticAnalyzer
{
public:
    SemanticAnalyzer();
    SemanticAnalyzer(const program_struct * root);
    ~SemanticAnalyzer();
    ProgramNode * getRoot() const;
private:
    ProgramNode * program;
};

#endif // SEMANTICANALYZER_H

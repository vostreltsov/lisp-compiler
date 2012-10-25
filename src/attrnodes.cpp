#include "attrnodes.h"

AttributedNode::AttributedNode()
{
}

AttributedNode::~AttributedNode()
{
}

ProgramNode::ProgramNode() : AttributedNode()
{
    fType = ATTR_TYPE_PROGRAM;
}

ProgramNode::~ProgramNode()
{
    foreach(AttributedNode * expr, fExpressions) {
        delete expr;
    }
}

QString ProgramNode::dotCode() const
{
    return "";
}

ProgramNode * ProgramNode::fromSyntaxNode(program_struct * syntaxNode)
{
    if (syntaxNode != NULL) {
        ProgramNode * result = new ProgramNode();
        s_expr_struct * expr = syntaxNode->s_expr_seq->first;
        while (expr != NULL) {
            result->fExpressions.append(SExpressionNode::fromSyntaxNode(expr));
            expr = expr->next;
        }
        return result;
    } else {
        return NULL;
    }
}

SExpressionNode::SExpressionNode() : AttributedNode()
{
    fType = ATTR_TYPE_S_EXPR;
}

SExpressionNode::~SExpressionNode()
{
}

QString SExpressionNode::dotCode() const
{
    return "";
}

SExpressionNode * SExpressionNode::fromSyntaxNode(s_expr_struct * syntaxNode)
{
    if (syntaxNode != NULL) {
        SExpressionNode * result = new SExpressionNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fSubType = syntaxNode->type;
        result->fInteger = syntaxNode->integer;
        result->fCharacter = syntaxNode->character;
        result->fString = syntaxNode->string;
        result->fBoolean = syntaxNode->boolean;
        result->fId = syntaxNode->id;
        result->fList = ListNode::fromSyntaxNode(syntaxNode->list);
        return result;
    } else {
        return NULL;
    }
}

SlotDefinitionNode::SlotDefinitionNode() : AttributedNode()
{
    fType = ATTR_TYPE_SLOT_DEF;
}

SlotDefinitionNode::~SlotDefinitionNode()
{
}

QString SlotDefinitionNode::dotCode() const
{
    return "";
}

SlotDefinitionNode * SlotDefinitionNode::fromSyntaxNode(slot_def_struct * syntaxNode)
{
    if (syntaxNode != NULL) {

    } else {
        return NULL;
    }
}

ListNode::ListNode() : AttributedNode()
{
    fType = ATTR_TYPE_LIST;
}

ListNode::~ListNode()
{
}

QString ListNode::dotCode() const
{
    return "";
}

ListNode * ListNode::fromSyntaxNode(list_struct * syntaxNode)
{
    if (syntaxNode != NULL) {

    } else {
        return NULL;
    }
}

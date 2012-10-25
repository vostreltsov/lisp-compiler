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
    foreach(SExpressionNode * expr, fExpressions) {
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
        s_expr_struct * expr = (syntaxNode->s_expr_seq != NULL ? syntaxNode->s_expr_seq->first : NULL);
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
    if (fList != NULL) {
        delete fList;
    }
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
    if (fInitform != NULL) {
        delete fInitform;
    }
}

QString SlotDefinitionNode::dotCode() const
{
    return "";
}

SlotDefinitionNode * SlotDefinitionNode::fromSyntaxNode(slot_def_struct * syntaxNode)
{
    if (syntaxNode != NULL) {
        SlotDefinitionNode * result = new SlotDefinitionNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fSubType = syntaxNode->type;
        result->fInitform = SExpressionNode::fromSyntaxNode(syntaxNode->initform);
        result->fId = syntaxNode->id;
        result->fAllocType = syntaxNode->alloc;
        return result;
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
    if (fCondition != NULL) {
        delete fCondition;
    }
    if (fContainer != NULL) {
        delete fContainer;
    }
    if (fFrom != NULL) {
        delete fFrom;
    }
    if (fTo != NULL) {
        delete fTo;
    }
    if (fBody1 != NULL) {
        delete fBody1;
    }
    if (fBody2 != NULL) {
        delete fBody2;
    }
    foreach(SlotDefinitionNode * slotdef, fSlotDefs) {
        delete slotdef;
    }
}

QString ListNode::dotCode() const
{
    return "";
}

ListNode * ListNode::fromSyntaxNode(list_struct * syntaxNode)
{
    if (syntaxNode != NULL) {
        ListNode * result = new ListNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fId = syntaxNode->id;
        s_expr_struct * expr = (syntaxNode->ops != NULL ? syntaxNode->ops->first : NULL);
        while (expr != NULL) {
            result->fOperands.append(SExpressionNode::fromSyntaxNode(expr));
            expr = expr->next;
        }
        result->fCondition = SExpressionNode::fromSyntaxNode(syntaxNode->cond);
        result->fContainer = SExpressionNode::fromSyntaxNode(syntaxNode->container);
        result->fFrom = SExpressionNode::fromSyntaxNode(syntaxNode->from);
        result->fTo = SExpressionNode::fromSyntaxNode(syntaxNode->to);
        result->fBody1 = SExpressionNode::fromSyntaxNode(syntaxNode->body1);
        result->fBody2 = SExpressionNode::fromSyntaxNode(syntaxNode->body2);
        slot_def_struct * slotdef = (syntaxNode->slotdefs != NULL ? syntaxNode->slotdefs->first : NULL);
        while (slotdef != NULL) {
            result->fSlotDefs.append(SlotDefinitionNode::fromSyntaxNode(slotdef));
            slotdef = slotdef->next;
        }
        result->fParent = syntaxNode->parent;
        return result;
    } else {
        return NULL;
    }
}

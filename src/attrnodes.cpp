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
    foreach (SExpressionNode * expr, fExpressions) {
        delete expr;
    }
}

QString ProgramNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\n program\"";
    QString result = "digraph {\n";
    foreach (SExpressionNode * expr, fExpressions) {
        result += expr->dotCode(tmp, "");
    }
    result += "}\n";
    return result;
}

void ProgramNode::doCheck(QLinkedList<QString> * errorList) const
{
    // Just check every single operand.
    foreach (SExpressionNode * node, fExpressions) {
        node->doCheck(errorList);
    }
}

ProgramNode * ProgramNode::fromSyntaxNode(program_struct * syntaxNode)
{
    if (syntaxNode != NULL) {
        ProgramNode * result = new ProgramNode();
        result->fNodeId = syntaxNode->nodeId;
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

QString SExpressionNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\ns_expr:";
    switch (fSubType) {
    case S_EXPR_TYPE_INT:
        tmp += "int\\n" + QString::number(fInteger) + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    case S_EXPR_TYPE_CHAR:
        tmp += "char\\n" + QString(fCharacter) + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    case S_EXPR_TYPE_STRING:
        tmp += "string\\n" + fString + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    case S_EXPR_TYPE_BOOL:
        tmp += "bool\\n" + QString(fBoolean ? "TRUE" : "FALSE") + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    case S_EXPR_TYPE_ID:
        tmp += "id\\n" + fId + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    case S_EXPR_TYPE_LIST:
        tmp += "list\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        return result + fList->dotCode(tmp, "");
    }
}

void SExpressionNode::doCheck(QLinkedList<QString> * errorList) const
{
    switch (fSubType) {
    case S_EXPR_TYPE_INT:
    case S_EXPR_TYPE_CHAR:
    case S_EXPR_TYPE_STRING:
    case S_EXPR_TYPE_BOOL:
    case S_EXPR_TYPE_ID:
        break;
    case S_EXPR_TYPE_LIST:
        fList->doCheck(errorList);
        break;
    default:
        errorList->append("Unknown s-expression subtype: " + QString::number(fSubType));
        break;
    }
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

QString SlotDefinitionNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\n:";
    if (fSubType == SLOT_DEF_INITFORM) {
        tmp += "initform\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        return result + fInitform->dotCode(tmp, "");
    } else if (fSubType == SLOT_DEF_READER) {
        tmp += "reader " + fId + "\"";
    } else if (fSubType == SLOT_DEF_WRITER) {
        tmp += "writer " + fId + "\"";
    } else if (fSubType == SLOT_DEF_ACCESSOR) {
        tmp += "accessor " + fId + "\"";
    } else if (fSubType == SLOT_DEF_ALLOCATION && fAllocType == SLOT_ALLOC_INSTANCE) {
        tmp += "alloc instance\"";
    } else if (fSubType == SLOT_DEF_ALLOCATION && fAllocType == SLOT_ALLOC_CLASS) {
        tmp += "alloc class\"";
    }
    return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
}

void SlotDefinitionNode::doCheck(QLinkedList<QString> * errorList) const
{
    // TODO.
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
    foreach (SlotDefinitionNode * slotdef, fSlotDefs) {
        delete slotdef;
    }
}

QString ListNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\nlist:";
    switch (fSubType) {
    case LIST_TYPE_FCALL: {
        tmp += "call " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * op, fOperands) {
            result += op->dotCode(tmp, "operand " + QString::number(cnt++));
        }
        return result;
    }
    case LIST_TYPE_LOOP_IN: {
        tmp += "call " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fContainer->dotCode(tmp, "container");
        result += fBody1->dotCode(tmp, "body");
        return result;
    }
    case LIST_TYPE_LOOP_FROM_TO: {
        tmp += "call " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fFrom->dotCode(tmp, "from");
        result += fTo->dotCode(tmp, "to");
        result += fBody1->dotCode(tmp, "body");
        return result;
    }
    case LIST_TYPE_PROGN: {
        tmp += "progn\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        foreach (SExpressionNode * op, fOperands) {
            result += op->dotCode(tmp, "");
        }
        return result;
    }
    case LIST_TYPE_IF: {
        tmp += "if\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fCondition->dotCode(tmp, "condition");
        result += fBody1->dotCode(tmp, "positive");
        if (fBody2 != NULL) {
            result += fBody2->dotCode(tmp, "negative");
        }
        return result;
    }
    case LIST_TYPE_SLOTDEF: {
        tmp += "slot " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        foreach (SlotDefinitionNode * slotdef, fSlotDefs) {
            result += slotdef->dotCode(tmp, "");
        }
        return result;
    }
    case LIST_TYPE_DEFUN: {
        tmp += "defun " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * op, fOperands) {
            result += op->dotCode(tmp, "argument " + QString::number(cnt++));
        }
        result += fBody1->dotCode(tmp, "body");
        return result;
    }
    case LIST_TYPE_DEFCLASS: {
        if (fParent.length() == 0) {
            tmp += "defclass " + fId + "\"";
        } else {
            tmp += "defclass " + fId + "\\nparent:" + fParent + "\"";
        }
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * op, fOperands) {
            result += op->dotCode(tmp, "slot " + QString::number(cnt++));
        }
        return result;
    }
    default:
        break;
    }
}

void ListNode::doCheck(QLinkedList<QString> * errorList) const
{
    // TODO
}

ListNode * ListNode::fromSyntaxNode(list_struct * syntaxNode)
{
    if (syntaxNode != NULL) {
        ListNode * result = new ListNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fSubType = syntaxNode->type;
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

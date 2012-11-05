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

void ProgramNode::check(QLinkedList<QString> * errorList) const
{
    // Check every single operand.
    foreach (SExpressionNode * node, fExpressions) {
        node->check(errorList);
    }
}

bool ProgramNode::isCalculable() const
{
    return false;
}

void ProgramNode::transform()
{
    // Transform every single operand.
    foreach (SExpressionNode * node, fExpressions) {
        node->transform();
    }
}

ProgramNode * ProgramNode::fromSyntaxNode(const program_struct * syntaxNode)
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

void SExpressionNode::check(QLinkedList<QString> * errorList) const
{
    // Only lists need to be checked.
    switch (fSubType) {
    case S_EXPR_TYPE_INT:
    case S_EXPR_TYPE_CHAR:
    case S_EXPR_TYPE_STRING:
    case S_EXPR_TYPE_BOOL:
    case S_EXPR_TYPE_ID:
        break;
    case S_EXPR_TYPE_LIST:
        fList->check(errorList);
        break;
    default:
        errorList->append("Unknown s-expression subtype: " + QString::number(fSubType));
        break;
    }
}

bool SExpressionNode::isCalculable() const
{
    switch (fSubType) {
    case S_EXPR_TYPE_INT:
    case S_EXPR_TYPE_CHAR:
    case S_EXPR_TYPE_STRING:
    case S_EXPR_TYPE_BOOL:
    case S_EXPR_TYPE_ID:
        return true;
    case S_EXPR_TYPE_LIST:
        return fList->isCalculable();
    default:
        return false;
    }
}

void SExpressionNode::transform()
{
    if (fList != NULL) {
        fList->transform();
    }
}

SExpressionNode * SExpressionNode::fromSyntaxNode(const s_expr_struct * syntaxNode)
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

void SlotDefinitionNode::check(QLinkedList<QString> * errorList) const
{
    // The only thing to check is calculability of the initform.
    if (fSubType == SLOT_DEF_INITFORM) {
        if (!fInitform->isCalculable()) {
            errorList->append("Only calculable expressions can be used for :initform.");
        }
    }
}

bool SlotDefinitionNode::isCalculable() const
{
    return false;
}

void SlotDefinitionNode::transform()
{
    if (fInitform != NULL) {
        fInitform->transform();
    }
}

SlotDefinitionNode * SlotDefinitionNode::fromSyntaxNode(const slot_def_struct * syntaxNode)
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
    foreach (SExpressionNode * op, fOperands) {
        delete op;
    }
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
    case LIST_TYPE_ASSIGN_ELT: {
        tmp += "[]= \"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * op, fOperands) {
            result += op->dotCode(tmp, "operand " + QString::number(cnt++));
        }
        return result;
    }
    default: {
        break;
    }
    }
}

void ListNode::check(QLinkedList<QString> * errorList) const
{
    // First, check the subtree.
    foreach (SExpressionNode * op, fOperands) {
        op->check(errorList);
    }
    if (fCondition != NULL) {
        fCondition->check(errorList);
    }
    if (fContainer != NULL) {
        fContainer->check(errorList);
    }
    if (fFrom != NULL) {
        fFrom->check(errorList);
    }
    if (fTo != NULL) {
        fTo->check(errorList);
    }
    if (fBody1 != NULL) {
        fBody1->check(errorList);
    }
    if (fBody2 != NULL) {
        fBody2->check(errorList);
    }
    foreach (SlotDefinitionNode * slotdef, fSlotDefs) {
        slotdef->check(errorList);
    }

    // Now check this node.
    switch (fSubType) {
    case LIST_TYPE_FCALL: {
        // Operands should be calculable.
        foreach (SExpressionNode * op, fOperands) {
            if (!op->isCalculable()) {
                errorList->append("Only calculable expressions can be passed as arguments.");
            }
        }

        break;
    }
    case LIST_TYPE_LOOP_IN: {
        // TODO: check container
        break;
    }
    case LIST_TYPE_LOOP_FROM_TO: {
        if (!fFrom->isCalculable() || !fTo->isCalculable()) {
            errorList->append("Only calculable expressions can be used as loop conditions.");
        }
        break;
    }
    case LIST_TYPE_PROGN: {
        break;
    }
    case LIST_TYPE_IF: {
        break;
    }
    case LIST_TYPE_SLOTDEF: {
        break;
    }
    case LIST_TYPE_DEFUN: {
        break;
    }
    case LIST_TYPE_DEFCLASS: {
        break;
    }
    case LIST_TYPE_ASSIGN_ELT: {
        break;
    }
    default: {
        break;
    }
    }
}

bool ListNode::isCalculable() const
{
    // Only function calls are calculable.
    bool result = (fSubType == LIST_TYPE_FCALL);
    foreach (SExpressionNode * op, fOperands) {
        result &= op->isCalculable();
    }
    return result;
}

void ListNode::transform()
{
    // First, transform the subtree.
    foreach (SExpressionNode * op, fOperands) {
        op->transform();
    }
    if (fCondition != NULL) {
        fCondition->transform();
    }
    if (fContainer != NULL) {
        fContainer->transform();
    }
    if (fFrom != NULL) {
        fFrom->transform();
    }
    if (fTo != NULL) {
        fTo->transform();
    }
    if (fBody1 != NULL) {
        fBody1->transform();
    }
    if (fBody2 != NULL) {
        fBody2->transform();
    }
    foreach (SlotDefinitionNode * slotdef, fSlotDefs) {
        slotdef->transform();
    }

    // Now transform this node.
    switch (fSubType) {
    case LIST_TYPE_FCALL: {
        SExpressionNode * op1 = fOperands.first();
        if (fId == FUNC_NAME_SETF) {
            // Convert to ternary operator.
            if (op1->fSubType == S_EXPR_TYPE_LIST && op1->fList->fSubType == LIST_TYPE_FCALL && op1->fList->fId == FUNC_NAME_ELT) {
                fSubType = LIST_TYPE_ASSIGN_ELT;
                // Remove the first element and concatenate the rest to the ELT's operands list.
                fOperands.removeFirst();
                op1->fList->fOperands << fOperands;
                fOperands = op1->fList->fOperands;
                // Clear the ELT's operands list and delete it.
                op1->fList->fOperands.clear();
                delete op1;
            }
        }
        break;
    }
    case LIST_TYPE_LOOP_IN: {
        break;
    }
    case LIST_TYPE_LOOP_FROM_TO: {
        break;
    }
    case LIST_TYPE_PROGN: {
        break;
    }
    case LIST_TYPE_IF: {
        break;
    }
    case LIST_TYPE_SLOTDEF: {
        break;
    }
    case LIST_TYPE_DEFUN: {
        break;
    }
    case LIST_TYPE_DEFCLASS: {
        break;
    }
    case LIST_TYPE_ASSIGN_ELT: {
        break;
    }
    default: {
        break;
    }
    }
}

ListNode * ListNode::fromSyntaxNode(const list_struct * syntaxNode)
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

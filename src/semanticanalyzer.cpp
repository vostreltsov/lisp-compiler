#include "semanticanalyzer.h"

SemanticConstant::SemanticConstant()
{
    fId = -1;
    fType = CONSTANT_Utf8;
    fUtf8 = "";
    fInteger = 0;
    fRef1 = NULL;
    fRef2 = NULL;
}

SemanticProgram::SemanticProgram()
{
}

SemanticClass::SemanticClass()
{
}

SemanticField::SemanticField()
{
}

SemanticMethod::SemanticMethod()
{
}

SemanticLocalVariable::SemanticLocalVariable()
{
}

SemanticAnalyzer::SemanticAnalyzer()
{
    root = NULL;
}

SemanticAnalyzer::SemanticAnalyzer(const program_struct * root)
{
    this->root = ProgramNode::fromSyntaxNode(root);
}

SemanticAnalyzer::~SemanticAnalyzer()
{
    if (root != NULL) {
        delete root;
    }
}

ProgramNode * SemanticAnalyzer::getRoot() const
{
    return root;
}

QLinkedList<QString> SemanticAnalyzer::getErrors() const
{
    return errors;
}

bool SemanticAnalyzer::doSemantics()
{
    errors.clear();
    if (root != NULL) {
        root->semantics(&errors);
    }
    return errors.empty();
}

void SemanticAnalyzer::doTransform()
{
    if (root != NULL) {
        root->transform();
    }
}

AttributedNode::AttributedNode()
{
}

AttributedNode::~AttributedNode()
{
    foreach (AttributedNode * child, childNodes()) {
        delete child;
    }
}

QLinkedList<AttributedNode *> AttributedNode::childNodes() const
{
    return QLinkedList<AttributedNode *>();
}

ProgramNode::ProgramNode() : AttributedNode()
{
    fType = ATTR_TYPE_PROGRAM;
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

bool ProgramNode::isCalculable() const
{
    return false;
}

QLinkedList<AttributedNode *> ProgramNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    foreach (SExpressionNode * expr, fExpressions) {
        result << expr;
    }
    return result;
}

void ProgramNode::transform()
{
    // Transform all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->transform();
    }
}

void ProgramNode::semantics(QLinkedList<QString> * errorList) const
{
    // Check every single operand.
    foreach (SExpressionNode * node, fExpressions) {
        node->semantics(errorList);
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

QLinkedList<AttributedNode *> SExpressionNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    if (fList != NULL) {
        result << fList;
    }
    return result;
}

void SExpressionNode::transform()
{
    // Transform all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->transform();
    }
}

void SExpressionNode::semantics(QLinkedList<QString> * errorList) const
{
    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(errorList);
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

bool SlotDefinitionNode::isCalculable() const
{
    return false;
}

QLinkedList<AttributedNode *> SlotDefinitionNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    if (fInitform != NULL) {
        result << fInitform;
    }
    return result;
}

void SlotDefinitionNode::transform()
{
    // Transform all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->transform();
    }
}

void SlotDefinitionNode::semantics(QLinkedList<QString> * errorList) const
{
    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(errorList);
    }
    // The only thing to check is calculability of the initform.
    if (fSubType == SLOT_DEF_INITFORM) {
        if (!fInitform->isCalculable()) {
            errorList->append("Only calculable expressions can be used for :initform.");
        }
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
        tmp += "loop (in) " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fContainer->dotCode(tmp, "container");
        result += fBody1->dotCode(tmp, "body");
        return result;
    }
    case LIST_TYPE_LOOP_FROM_TO: {
        tmp += "loop (from to) " + fId + "\"";
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

bool ListNode::isCalculable() const
{
    // Only function calls are calculable.
    bool result = (fSubType == LIST_TYPE_FCALL);
    foreach (SExpressionNode * op, fOperands) {
        result &= op->isCalculable();
    }
    return result;
}

QLinkedList<AttributedNode *> ListNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    foreach (SExpressionNode * op, fOperands) {
        result << op;
    }
    if (fCondition != NULL) {
        result << fCondition;
    }
    if (fContainer != NULL) {
        result << fContainer;
    }
    if (fFrom != NULL) {
        result << fFrom;
    }
    if (fTo != NULL) {
        result << fTo;
    }
    if (fBody1 != NULL) {
        result << fBody1;
    }
    if (fBody2 != NULL) {
        result << fBody2;
    }
    foreach (SlotDefinitionNode * slotdef, fSlotDefs) {
        result << slotdef;
    }
    return result;
}

void ListNode::transform()
{
    // Transform all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->transform();
    }

    // Now transform this node.
    switch (fSubType) {
    case LIST_TYPE_FCALL: {
        SExpressionNode * op1 = fOperands.first();
        if (fId == NAME_FUNC_SETF) {
            // Convert to ternary operator.
            if (op1->fSubType == S_EXPR_TYPE_LIST && op1->fList->fSubType == LIST_TYPE_FCALL && op1->fList->fId == NAME_FUNC_ELT) {
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

void ListNode::semantics(QLinkedList<QString> * errorList) const
{
    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(errorList);
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
        bool result = (fContainer->fSubType == S_EXPR_TYPE_ID) ||
                      (fContainer->fSubType == S_EXPR_TYPE_LIST && (fContainer->fList->fId == NAME_FUNC_VECTOR || fContainer->fList->fId == NAME_FUNC_LIST));
        if (!result) {
            errorList->append("Wrong container specified for the loop.");
        }
        break;
    }
    case LIST_TYPE_LOOP_FROM_TO: {
        // Check if conditions are calculable.
        if (!fFrom->isCalculable() || !fTo->isCalculable()) {
            errorList->append("Only calculable expressions can be used as loop conditions.");
        }
        break;
    }
    case LIST_TYPE_PROGN: {
        // Checked before this switch-case.
        break;
    }
    case LIST_TYPE_IF: {
        // Check if the condition is calculable.
        if (!fCondition->isCalculable()) {
            errorList->append("Only calculable expressions can be used as conditions.");
        }
        break;
    }
    case LIST_TYPE_SLOTDEF: {
        // Checked before this switch-case.
        break;
    }
    case LIST_TYPE_DEFUN: {
        // Function parameters are identifiers.
        foreach (SExpressionNode * op, fOperands) {
            if (op->fSubType != S_EXPR_TYPE_ID) {
                errorList->append("Invalid function parameter name.");
            }
        }

        break;
    }
    case LIST_TYPE_DEFCLASS: {
        // Checked before this switch-case.
        break;
    }
    case LIST_TYPE_ASSIGN_ELT: {
        // Check if the assigned expression is calculable.
        if (!fOperands.last()->isCalculable()) {
            errorList->append("Only calculable expressions can be assigned.");
        }
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

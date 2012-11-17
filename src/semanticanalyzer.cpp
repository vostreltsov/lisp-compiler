#include "semanticanalyzer.h"

SemanticConstant::SemanticConstant(int id, JavaConstantsTypes type, QString utf8, int integer, SemanticConstant * ref1, SemanticConstant * ref2)
{
    fId = id;
    fType = type;
    fUtf8 = utf8;
    fInteger = integer;
    fRef1 = ref1;
    fRef2 = ref2;
}

SemanticClass::SemanticClass()
{
    fConstClass = NULL;
    fConstParent = NULL;
}

SemanticConstant * SemanticClass::addUtf8Constant(QString value)
{
    // Does it already exist?
    foreach (SemanticConstant * existed, fConstantsTable) {
        if (existed->fType == CONSTANT_Utf8 && existed->fUtf8 == value) {
            return existed;
        }
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size(), CONSTANT_Utf8, value);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addIntegerConstant(int value)
{
    // Does it already exist?
    foreach (SemanticConstant * existed, fConstantsTable) {
        if (existed->fType == CONSTANT_Integer && existed->fInteger == value) {
            return existed;
        }
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size(), CONSTANT_Integer, "", value);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addClassConstant(QString name)
{
    SemanticConstant * utf8const = addUtf8Constant(name);
    // Does it already exist? Check by the pointer to the utf8 constant.
    foreach (SemanticConstant * existed, fConstantsTable) {
        if (existed->fType == CONSTANT_Class && existed->fRef1 == utf8const) {
            return existed;
        }
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size(), CONSTANT_Class, "", 0, utf8const);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addStringConstant(QString value)
{
    SemanticConstant * utf8const = addUtf8Constant(value);
    // Does it already exist? Check by the pointer to utf8 constant.
    foreach (SemanticConstant * existed, fConstantsTable) {
        if (existed->fType == CONSTANT_String && existed->fRef1 == utf8const) {
            return existed;
        }
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size(), CONSTANT_String, "", 0, utf8const);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addFieldrefConstant(QString className, QString fieldName, QString descriptor)
{
   SemanticConstant * classConst = addClassConstant(className);
   SemanticConstant * nameAndTypeConst = addNameAndTypeConstant(fieldName, descriptor);
   // Does it already exist? Check by the pointers to the class and nameAndType constants.
   foreach (SemanticConstant * existed, fConstantsTable) {
       if (existed->fType == CONSTANT_Fieldref && existed->fRef1 == classConst && existed->fRef2 == nameAndTypeConst) {
           return existed;
       }
   }
   // Create the new constant.
   SemanticConstant * result = new SemanticConstant(fConstantsTable.size(), CONSTANT_Fieldref, "", 0, classConst, nameAndTypeConst);
   fConstantsTable << result;
   return result;
}

SemanticConstant * SemanticClass::addMethodrefConstant(QString className, QString methodName, QString descriptor)
{
    SemanticConstant * classConst = addClassConstant(className);
    SemanticConstant * nameAndTypeConst = addNameAndTypeConstant(methodName, descriptor);
    // Does it already exist? Check by the pointers to the class and nameAndType constants.
    foreach (SemanticConstant * existed, fConstantsTable) {
        if (existed->fType == CONSTANT_Methodref && existed->fRef1 == classConst && existed->fRef2 == nameAndTypeConst) {
            return existed;
        }
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size(), CONSTANT_Methodref, "", 0, classConst, nameAndTypeConst);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addNameAndTypeConstant(QString name, QString type)
{
    SemanticConstant * nameConst = addUtf8Constant(name);
    SemanticConstant * typeConst = addUtf8Constant(type);
    // Does it already exist? Check by the pointers to the utf8 constants.
    foreach (SemanticConstant * existed, fConstantsTable) {
        if (existed->fType == CONSTANT_NameAndType && existed->fRef1 == nameConst && existed->fRef2 == typeConst) {
            return existed;
        }
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size(), CONSTANT_NameAndType, "", 0, nameConst, typeConst);
    fConstantsTable << result;
    return result;
}

void SemanticClass::addDefaultAndParentConstructor()
{
    // Add constructor name and descriptor.
    addUtf8Constant(NAME_JAVA_CONSTRUCTOR);
    addUtf8Constant(DESC_JAVA_CONSTRUCTOR_VOID);

    // Add constructor to the methods table.
    SemanticMethod * constructorThis = new SemanticMethod();
    //SemanticMethod * constructorParent = new SemanticMethod();
    constructorThis->fConstMethodref = addMethodrefConstant(fConstClass->fRef1->fUtf8, NAME_JAVA_CONSTRUCTOR, DESC_JAVA_CONSTRUCTOR_VOID);
    /*constructorParent->fConstMethodref =*/ addMethodrefConstant(fConstParent->fUtf8, NAME_JAVA_CONSTRUCTOR, DESC_JAVA_CONSTRUCTOR_VOID);

    // Add methods to the table.
    fMethodsTable.insert(NAME_JAVA_CONSTRUCTOR, constructorThis);
}

SemanticField::SemanticField()
{
    fConstFieldref = NULL;
}

SemanticMethod::SemanticMethod()
{
    fConstMethodref = NULL;
}

SemanticLocalVar * SemanticMethod::addLocalVarConstant(QString name)
{
    // Does it already exist?
    if (fLocalVarsTable.contains(name)) {
        return fLocalVarsTable[name];
    }
    // Create the new variable.
    SemanticLocalVar * result = new SemanticLocalVar(fLocalVarsTable.size(), name);
    fLocalVarsTable.insert(name, result);
    return result;
}

SemanticLocalVar::SemanticLocalVar(int id, QString name)
{
    fId = id;
    fName = name;
}

SemanticAnalyzer::SemanticAnalyzer()
{
    fRoot = NULL;
}

SemanticAnalyzer::SemanticAnalyzer(const program_struct * root)
{
    fRoot = ProgramNode::fromSyntaxNode(root);
}

SemanticAnalyzer::~SemanticAnalyzer()
{
    if (fRoot != NULL) {
        delete fRoot;
    }
}

ProgramNode * SemanticAnalyzer::getRoot() const
{
    return fRoot;
}

QLinkedList<QString> SemanticAnalyzer::getErrors() const
{
    return fErrors;
}

bool SemanticAnalyzer::doSemantics()
{
    fClassTable.clear();
    fErrors.clear();
    if (fRoot != NULL) {
        fRoot->semantics(&fClassTable,  &fErrors, NULL, NULL);
    }
    return fErrors.empty();
}

void SemanticAnalyzer::doTransform()
{
    if (fRoot != NULL) {
        fRoot->transform();
    }
}

SemanticClass * SemanticAnalyzer::addClass(DefinitionNode * nodeDefclass)
{
    QString className = nodeDefclass->fId;

    if (fClassTable.contains(className)) {
        SemanticClass * result = new SemanticClass();
        // TODO: add constants, check inheritance, etc.

        fClassTable.insert(className, result);
        return result;
    } else {
        // Error: the class already exists.
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
    foreach (AttributedNode * node, childNodes()) {
        result += node->dotCode(tmp);
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
    foreach (ProgramPartNode * node, fParts) {
        result << node;
    }
    return result;
}

void ProgramNode::transform()
{
    // Transform all parts.
    foreach (AttributedNode * node, childNodes()) {
        node->transform();
    }

    // Create main method definition node,
    DefinitionNode * mainMethod = new DefinitionNode();
    mainMethod->fNodeId = 100500;
    mainMethod->fSubType = DEF_TYPE_FUNC;
    mainMethod->fId = NAME_JAVA_METHOD_MAIN;
    // No need to add arguments I hope, handle it on the code generation stage.
    // Move all global operators to the main method.
    QLinkedList<ProgramPartNode *>::iterator iter = fParts.begin();
    while (iter != fParts.end()) {
        ProgramPartNode * curNode = *iter;
        if (curNode->fSubType == PROGRAM_PART_TYPE_S_EXPR) {
            mainMethod->fBody << curNode->fSExpression;
            curNode->fSExpression = NULL;
            iter = fParts.erase(iter);
        } else {
            ++iter;
        }

    }

    // Main method is done, create main class definition node, add the main method definition node.
    DefinitionNode * mainClass = new DefinitionNode();
    mainClass->fNodeId = 100501;
    mainClass->fSubType = DEF_TYPE_CLASS;
    mainClass->fId = NAME_JAVA_CLASS_MAINCLASS;
    mainClass->fParent = NAME_JAVA_CLASS_OBJECT;
    mainClass->fClassMethods << mainMethod;

    // Finally, create a new program part node which contains main class.
    ProgramPartNode * mainPart = new ProgramPartNode();
    mainPart->fNodeId = 100502;
    mainPart->fSubType = PROGRAM_PART_TYPE_DEF;
    mainPart->fDefinition = mainClass;

    fParts << mainPart;
}

void ProgramNode::semantics(QMap<QString, SemanticClass *> * classTable, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const
{
    // Create tables and check all child nodes.
    foreach (AttributedNode * node, childNodes()) {
        node->semantics(classTable, errorList, curClass, curMethod);
    }

    // Main class is added in the above call since it's added as a node during transformation. Add the base class to constants.
    SemanticClass * baseClass = new SemanticClass();
    baseClass->fConstClass = baseClass->addClassConstant(NAME_JAVA_CLASS_BASECLASS);
    baseClass->fConstParent = baseClass->addClassConstant(NAME_JAVA_CLASS_OBJECT);
    baseClass->addDefaultAndParentConstructor();
}

ProgramNode * ProgramNode::fromSyntaxNode(const program_struct * syntaxNode)
{
    ProgramNode * result = NULL;
    if (syntaxNode != NULL) {
        result = new ProgramNode();
        result->fNodeId = syntaxNode->nodeId;
        program_part_struct * part = (syntaxNode->parts != NULL ? syntaxNode->parts->first : NULL);
        while (part != NULL) {
            result->fParts << ProgramPartNode::fromSyntaxNode(part);
            part = part->next;
        }
    }
    return result;
}

ProgramPartNode::ProgramPartNode() : AttributedNode()
{
    fType = ATTR_TYPE_PROGRAM_PART;
    fSExpression = NULL;
    fDefinition = NULL;
}

QString ProgramPartNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\n program part\"";
    QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    switch (fSubType) {
    case PROGRAM_PART_TYPE_S_EXPR: {
        result += fSExpression->dotCode(tmp);
        break;
    }
    case PROGRAM_PART_TYPE_DEF: {
        result += fDefinition->dotCode(tmp);
        break;
    }
    default: {
        break;
    }
    }
    return result;
}

bool ProgramPartNode::isCalculable() const
{
    return (fSubType == PROGRAM_PART_TYPE_S_EXPR && fSExpression->isCalculable());
}

QLinkedList<AttributedNode *> ProgramPartNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    if (fSExpression != NULL) {
        result << fSExpression;
    }
    if (fDefinition != NULL) {
        result << fDefinition;
    }
    return result;
}

void ProgramPartNode::transform()
{
    foreach (AttributedNode * node, childNodes()) {
        node->transform();
    }
}

void ProgramPartNode::semantics(QMap<QString, SemanticClass *> * classTable, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const
{
    // TODO
}

ProgramPartNode * ProgramPartNode::fromSyntaxNode(const program_part_struct * syntaxNode)
{
    ProgramPartNode * result = NULL;
    if (syntaxNode != NULL) {
        result = new ProgramPartNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fSubType = syntaxNode->type;
        result->fSExpression = SExpressionNode::fromSyntaxNode(syntaxNode->s_expr);
        result->fDefinition = DefinitionNode::fromSyntaxNode(syntaxNode->def);
    }
    return result;
}

SExpressionNode::SExpressionNode() : AttributedNode()
{
    fType = ATTR_TYPE_S_EXPR;
}

QString SExpressionNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\ns_expr:";
    switch (fSubType) {
    case S_EXPR_TYPE_INT: {
        tmp += "int\\n" + QString::number(fInteger) + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_CHAR: {
        tmp += "char\\n" + QString(fCharacter) + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_STRING: {
        tmp += "string\\n" + fString + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_BOOL: {
        tmp += "bool\\n" + QString(fBoolean ? "TRUE" : "FALSE") + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_ID: {
        tmp += "id\\n" + fId + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_FCALL: {
        tmp += "call " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * node, fArguments) {
            result += node->dotCode(tmp, "arg " + QString::number(cnt++));
        }
        return result;
    }
    case S_EXPR_TYPE_LOOP_IN: {
        tmp += "loop (in) " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fContainer->dotCode(tmp, "container");
        result += fBody1->dotCode(tmp, "body");
        return result;
    }
    case S_EXPR_TYPE_LOOP_FROM_TO: {
        tmp += "loop (from to) " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fFrom->dotCode(tmp, "from");
        result += fTo->dotCode(tmp, "to");
        result += fBody1->dotCode(tmp, "body");
        return result;
    }
    case S_EXPR_TYPE_PROGN: {
        tmp += "progn\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        foreach (SExpressionNode * op, fArguments) {
            result += op->dotCode(tmp, "");
        }
        return result;
    }
    case S_EXPR_TYPE_IF: {
        tmp += "if\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fCondition->dotCode(tmp, "condition");
        result += fBody1->dotCode(tmp, "positive");
        if (fBody2 != NULL) {
            result += fBody2->dotCode(tmp, "negative");
        }
        return result;
    }
    case S_EXPR_TYPE_MAKEINSTANCE: {
        tmp += "makeinstance '" + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        return result;
    }
    case S_EXPR_TYPE_SLOTVALUE: {
        tmp += "slotvalue " + fSlotValueObject + " '" + fSlotValueSlot + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        return result;
    }
    case S_EXPR_TYPE_ASSIGN_ELT: {
        tmp += "[]= \"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * node, fArguments) {
            result += node->dotCode(tmp, "arg " + QString::number(cnt++));
        }
        return result;
    }
    case S_EXPR_TYPE_ASSIGN_FIELD: {
        tmp += fSlotValueObject + "." + fSlotValueSlot + " = \"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * node, fArguments) {
            result += node->dotCode(tmp, "value " + QString::number(cnt++));
        }
        return result;
    }
    default: {
        return "";
    }
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
    case S_EXPR_TYPE_FCALL:
        return true;
    default:
        return false;
    }
}

QLinkedList<AttributedNode *> SExpressionNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    foreach (SExpressionNode * node, fArguments) {
        result << node;
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
    return result;
}

void SExpressionNode::transform()
{
    // Transform child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->transform();
    }

    // Transform this node.
    switch (fSubType) {
    case S_EXPR_TYPE_FCALL: {
        SExpressionNode * op1 = fArguments.isEmpty() ? NULL : fArguments.first();
        // Convert to ternary operators.
        if (op1 != NULL && fId == NAME_FUNC_SETF) {
            if (op1->fSubType == S_EXPR_TYPE_FCALL && op1->fId == NAME_FUNC_ELT) {
                // Convert "arr[i] = value" to ternary operator.
                fSubType = S_EXPR_TYPE_ASSIGN_ELT;
                // Remove the first element and concatenate the rest to the ELT's operands list.
                fArguments.removeFirst();
                op1->fArguments << fArguments;
                fArguments = op1->fArguments;
                // Clear the ELT's operands list and delete it.
                op1->fArguments.clear();
                delete op1;
            } else if (op1->fSubType == S_EXPR_TYPE_SLOTVALUE) {
                // Convert "obj.field = value" to ternary operator.
                fSubType = S_EXPR_TYPE_ASSIGN_FIELD;
                // Remove the first element and concatenate the rest to the ELT's operands list.
                fArguments.removeFirst();
                fSlotValueObject = op1->fSlotValueObject;
                fSlotValueSlot = op1->fSlotValueSlot;
                // Clear the SLOTVALUE's operands list and delete it.
                op1->fArguments.clear();
                delete op1;
            }
        }
        break;
    }
    default: {
        break;
    }
    }
}

void SExpressionNode::semantics(QMap<QString, SemanticClass *> * classTable, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const
{
    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(classTable, errorList, curClass, curMethod);
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
        result->fSlotValueObject = syntaxNode->slvalobj;
        result->fSlotValueSlot = syntaxNode->slvalslot;
        s_expr_struct * cur = (syntaxNode->args != NULL ? syntaxNode->args->first : NULL);
        while (cur != NULL) {
            result->fArguments << SExpressionNode::fromSyntaxNode(cur);
            cur = cur->next;
        }
        result->fCondition = SExpressionNode::fromSyntaxNode(syntaxNode->cond);
        result->fContainer = SExpressionNode::fromSyntaxNode(syntaxNode->container);
        result->fFrom = SExpressionNode::fromSyntaxNode(syntaxNode->from);
        result->fTo = SExpressionNode::fromSyntaxNode(syntaxNode->to);
        result->fBody1 = SExpressionNode::fromSyntaxNode(syntaxNode->body1);
        result->fBody2 = SExpressionNode::fromSyntaxNode(syntaxNode->body2);
        return result;
    } else {
        return NULL;
    }
}

SlotPropertyNode::SlotPropertyNode() : AttributedNode()
{
    fType = ATTR_TYPE_SLOT_PROP;
    fInitform = NULL;
}

QString SlotPropertyNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\n:";
    switch (fSubType) {
    case SLOT_PROP_TYPE_INITFORM: {
        tmp += "initform\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n" + fInitform->dotCode(tmp, "");
    }
    case SLOT_PROP_TYPE_READER: {
        tmp += "reader " + fId + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case SLOT_PROP_TYPE_WRITER: {
        tmp += "writer " + fId + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case SLOT_PROP_TYPE_ACCESSOR: {
        tmp += "accessor " + fId + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case SLOT_PROP_TYPE_ALLOCATION: {
        if (fAllocationType == SLOT_ALLOC_TYPE_INSTANCE) {
            tmp += "alloc instance\"";
        } else {
            tmp += "alloc class\"";
        }
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    default: {
        return "";
    }
    }
}

bool SlotPropertyNode::isCalculable() const
{
    return false;
}

QLinkedList<AttributedNode *> SlotPropertyNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    if (fInitform != NULL) {
        result << fInitform;
    }
    return result;
}

void SlotPropertyNode::transform()
{
    foreach (AttributedNode * child, childNodes()) {
        child->transform();
    }
}

void SlotPropertyNode::semantics(QMap<QString, SemanticClass *> * classTable, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const
{
    // The only thing to check is calculability of the initform.
    if (fSubType == SLOT_PROP_TYPE_INITFORM) {
        if (!fInitform->isCalculable()) {
            *errorList << "Only calculable expressions can be used for :initform.";
        }
    }
}

SlotPropertyNode * SlotPropertyNode::fromSyntaxNode(const slot_prop_struct * syntaxNode)
{
    SlotPropertyNode * result = NULL;
    if (syntaxNode != NULL) {
        result = new SlotPropertyNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fSubType = syntaxNode->type;
        result->fInitform = SExpressionNode::fromSyntaxNode(syntaxNode->initform);
        result->fId = syntaxNode->id;
        result->fAllocationType = syntaxNode->alloc;
    }
    return result;
}

SlotDefinitionNode::SlotDefinitionNode() : AttributedNode()
{
    fType = ATTR_TYPE_SLOT_DEF;
}

QString SlotDefinitionNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\nslot " + fId + "\"";
    QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    foreach (AttributedNode * node, childNodes()) {
        result += node->dotCode(tmp);
    }
    return result;
}

bool SlotDefinitionNode::isCalculable() const
{
    return false;
}

QLinkedList<AttributedNode *> SlotDefinitionNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    foreach (SlotPropertyNode * node, fProperties) {
        result << node;
    }
    return result;
}

void SlotDefinitionNode::transform()
{
    foreach (AttributedNode * node, childNodes()) {
        node->transform();
    }
}

void SlotDefinitionNode::semantics(QMap<QString, SemanticClass *> * classTable, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const
{
    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(classTable, errorList, curClass, curMethod);
    }
}

SlotDefinitionNode * SlotDefinitionNode::fromSyntaxNode(const slot_def_struct * syntaxNode)
{
    SlotDefinitionNode * result = NULL;
    if (syntaxNode != NULL) {
        result = new SlotDefinitionNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fId = syntaxNode->id;
        slot_prop_struct * prop = (syntaxNode->props != NULL ? syntaxNode->props->first : NULL);
        while (prop != NULL) {
            result->fProperties << SlotPropertyNode::fromSyntaxNode(prop);
            prop = prop->next;
        }
    }
    return result;
}

DefinitionNode::DefinitionNode() : AttributedNode()
{
    fType = ATTR_TYPE_DEFINITION;
}

QString DefinitionNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\nlist:";
    switch (fSubType) {

    case DEF_TYPE_FUNC: {
        tmp += "defun " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        int cnt = 0;
        foreach (SExpressionNode * node, fArguments) {
            result += node->dotCode(tmp, "argument " + QString::number(cnt++));
        }
        cnt = 0;
        foreach (SExpressionNode * node, fBody) {
            result += node->dotCode(tmp, "body expr " + QString::number(cnt++));
        }
        return result;
    }
    case DEF_TYPE_CLASS: {
        if (fParent.length() == 0) {
            tmp += "defclass " + fId + "\"";
        } else {
            tmp += "defclass " + fId + "\\nparent:" + fParent + "\"";
        }
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        foreach (SlotDefinitionNode * node, fClassFields) {
            result += node->dotCode(tmp, "field");
        }
        foreach (DefinitionNode * node, fClassMethods) {
            result += node->dotCode(tmp, "method");
        }
        return result;
    }
    default: {
        return "";
    }
    }
}

bool DefinitionNode::isCalculable() const
{
    return false;
}

QLinkedList<AttributedNode *> DefinitionNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    foreach (SExpressionNode * node, fArguments) {
        result << node;
    }
    foreach (SlotDefinitionNode * node, fClassFields) {
        result << node;
    }
    foreach (DefinitionNode * node, fClassMethods) {
        result << node;
    }
    foreach (SExpressionNode * node, fBody) {
        result << node;
    }
    return result;
}

void DefinitionNode::transform()
{
    foreach (AttributedNode * node, childNodes()) {
        node->transform();
    }
}

void DefinitionNode::semantics(QMap<QString, SemanticClass *> * classTable, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const
{
    // Analyse all child nodes.
   /* foreach (AttributedNode * child, childNodes()) {
        child->semantics(classTable, errorList, curClass, curMethod);
    }

    // Now check this node.
    switch (fSubType) {
    case LIST_TYPE_FCALL: {
        // Operands should be calculable.
        foreach (SExpressionNode * op, fOperands) {
            if (!op->isCalculable()) {
                *errorList << "Only calculable expressions can be passed as arguments.";
            }
        }

        break;
    }
    case LIST_TYPE_LOOP_IN: {
        bool result = (fContainer->fSubType == S_EXPR_TYPE_ID) ||
                      (fContainer->fSubType == S_EXPR_TYPE_LIST && (fContainer->fList->fId == NAME_FUNC_VECTOR || fContainer->fList->fId == NAME_FUNC_LIST));
        if (!result) {
            *errorList << "Wrong container specified for the loop.";
        }
        break;
    }
    case LIST_TYPE_LOOP_FROM_TO: {
        // Check if conditions are calculable.
        if (!fFrom->isCalculable() || !fTo->isCalculable()) {
            *errorList << "Only calculable expressions can be used as loop conditions.";
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
            *errorList << "Only calculable expressions can be used as conditions.";
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
                *errorList << "Invalid function parameter name.";
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
            *errorList << "Only calculable expressions can be assigned.";
        }
        break;
    }
    default: {
        break;
    }
    }*/
}

DefinitionNode * DefinitionNode::fromSyntaxNode(const def_struct * syntaxNode)
{
    DefinitionNode * result = NULL;
    if (syntaxNode != NULL) {
        result = new DefinitionNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fSubType = syntaxNode->type;
        result->fId = syntaxNode->id;
        result->fParent = syntaxNode->parent;
        if (result->fSubType == DEF_TYPE_CLASS && result->fParent.isEmpty()) {
            result->fParent = NAME_JAVA_CLASS_OBJECT;
        }
        s_expr_struct * expr = (syntaxNode->args != NULL ? syntaxNode->args->first : NULL);
        while (expr != NULL) {
            result->fArguments << SExpressionNode::fromSyntaxNode(expr);
            expr = expr->next;
        }
        // Decide what slots are fields and what slots are methods.
        slot_def_struct * slotdef = (syntaxNode->slotdefs != NULL ? syntaxNode->slotdefs->first : NULL);
        while (slotdef != NULL) {
            // TODO!!!
            result->fClassFields << SlotDefinitionNode::fromSyntaxNode(slotdef);
            slotdef = slotdef->next;
        }
        expr = (syntaxNode->body != NULL ? syntaxNode->body->first : NULL);
        while (expr != NULL) {
            result->fBody << SExpressionNode::fromSyntaxNode(expr);
            expr = expr->next;
        }

    }
    return result;
}

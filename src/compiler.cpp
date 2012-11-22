#include "compiler.h"

SemanticConstant::SemanticConstant(int number, JavaConstantsTypes type, QString utf8, int integer, SemanticConstant * ref1, SemanticConstant * ref2)
{
    fNumber = number;
    fType = type;
    fUtf8 = utf8;
    fInteger = integer;
    fRef1 = ref1;
    fRef2 = ref2;
}

void SemanticConstant::generateCode(BinaryWriter * writer) const
{
    // Write the constant type.
    writer->writeU2(fType);

    switch (fType) {
    case CONSTANT_Utf8: {
        // Write the string length.
        writer->writeU2(fUtf8.length());

        // Write the string itself.
        foreach (QChar ch, fUtf8) {
            writer->writeU1(ch.toAscii());
        }
        break;
    }
    case CONSTANT_Integer: {
        writer->writeU4(fInteger);
        break;
    }
    case CONSTANT_Float: {
        // Unsupported yet.
        break;
    }
    case CONSTANT_Long: {
        // Unsupported yet.
        break;
    }
    case CONSTANT_Double: {
        // Unsupported yet.
        break;
    }
    case CONSTANT_Class: {
        // Write the "utf8" constant number.
        writer->writeU2(fRef1->fNumber);
        break;
    }
    case CONSTANT_String: {
        // Write the "utf8" constant number.
        writer->writeU2(fRef1->fNumber);
        break;
    }
    case CONSTANT_Fieldref: {
        // Write the "class" constant number.
        writer->writeU2(fRef1->fNumber);

        // Write the "name and type" constant number.
        writer->writeU2(fRef2->fNumber);
        break;
    }
    case CONSTANT_Methodref: {
        // Write the "class" constant number.
        writer->writeU2(fRef1->fNumber);

        // Write the "name and type" constant number.
        writer->writeU2(fRef2->fNumber);
        break;
    }
    case CONSTANT_InterfaceMethodref: {
        // Unsupported yet.
        break;
    }
    case CONSTANT_NameAndType: {
        // Write the "utf8" constant number.
        writer->writeU2(fRef1->fNumber);

        // Write the "utf8" constant number.
        writer->writeU2(fRef2->fNumber);
        break;
    }
    default: {
        break;
    }
    }
}

QString SemanticConstant::dotCode(QString previous) const
{
    QString tmp = "\"const №" + QString::number(fNumber) + "\\n";
    QString result;
    switch (fType) {
    case CONSTANT_Utf8: {
        tmp += "Utf8 " + fUtf8 + "\"";
        break;
    }
    case CONSTANT_Integer: {
        tmp += "Integer " + QString::number(fInteger) + "\"";
        break;
    }
    case CONSTANT_Float: {
        tmp += "Float\"";
        break;
    }
    case CONSTANT_Long: {
        tmp += "Long\"";
        break;
    }
    case CONSTANT_Double: {
        tmp += "Double\"";
        break;
    }
    case CONSTANT_Class: {
        tmp += "Class " + fRef1->fUtf8 + "\"";
        result += fRef1->dotCode(tmp);
        break;
    }
    case CONSTANT_String: {
        tmp += "String\"";
        result += fRef1->dotCode(tmp);
        break;
    }
    case CONSTANT_Fieldref: {
        tmp += "Fieldref\"";
        result += fRef1->dotCode(tmp);
        result += fRef2->dotCode(tmp);
        break;
    }
    case CONSTANT_Methodref: {
        tmp += "Methodref\"";
        result += fRef1->dotCode(tmp);
        result += fRef2->dotCode(tmp);
        break;
    }
    case CONSTANT_InterfaceMethodref: {
        tmp += "InterfaceMethodref\"";
        break;
    }
    case CONSTANT_NameAndType: {
        tmp += "NameAndType\"";
        result += fRef1->dotCode(tmp);
        result += fRef2->dotCode(tmp);
        break;
    }
    default: {
        return "";
        break;
    }
    }

    result += previous + "->" + tmp + ";\n";
    return result;
}

SemanticProgram::SemanticProgram()
{
    fRoot = NULL;
}

SemanticProgram::SemanticProgram(const program_struct * root)
{
    fRoot = ProgramNode::fromSyntaxNode(root);
}

SemanticProgram::~SemanticProgram()
{
    if (fRoot != NULL) {
        delete fRoot;
    }
}

void SemanticProgram::doTransform()
{
    if (fRoot != NULL) {
        fRoot->transform();
    }
}

bool SemanticProgram::doSemantics()
{
    fClassTable.clear();
    fErrors.clear();
    if (fRoot != NULL) {
        fRoot->semantics(this,  &fErrors, NULL, NULL);
    }
    return fErrors.isEmpty();
}

bool SemanticProgram::doGenerateCode(QString dir) const
{
    if (!fErrors.isEmpty()) {
        return false;
    }
    bool result = true;

    // Create the resulting directory.
    if (!QDir(dir).exists()) {
        QDir().mkdir(dir);
    }

    // Copy rtl class files.
    QFile::copy(QString(NAME_JAVA_CLASS_BASE) + ".class", dir + "/" + NAME_JAVA_CLASS_BASE + ".class");
    QFile::copy(QString(NAME_JAVA_CLASS_LISPRTL) + ".class", dir + "/" + NAME_JAVA_CLASS_LISPRTL + ".class");

    // Generate code for all classes.
    foreach (SemanticClass * curClass, fClassTable) {
        result &= curClass->generateCode(dir);
    }

    return result;
}

QString SemanticProgram::dotCode() const
{
    QString result;
    foreach (SemanticClass * semClass, fClassTable) {
        //result += semClass->dotForTables("program");  // ARE YOU SURE YOU WANT THIS?
    }
    result += fRoot->dotCode("", "");
    return result;
}

ProgramNode * SemanticProgram::root() const
{
    return fRoot;
}

QLinkedList<QString> SemanticProgram::errors() const
{
    return fErrors;
}

bool SemanticProgram::hasClass(QString name) const
{
    return fClassTable.contains(name);
}

SemanticClass * SemanticProgram::getClass(QString name) const
{
    return fClassTable[name];
}

SemanticClass * SemanticProgram::addClass(const DefinitionNode * node)
{
    SemanticClass * newClass = new SemanticClass();
    newClass->fConstCode = newClass->addUtf8Constant("Code");
    newClass->fConstClass = newClass->addClassConstant(node->fId);
    newClass->fConstParent = newClass->addClassConstant(node->fParent);
    newClass->fNode = node;
    newClass->addDefaultAndParentConstructor();
    newClass->addRTLConstants();
    fClassTable.insert(node->fId, newClass);
    return newClass;
}

SemanticClass::SemanticClass()
{
    fConstCode = NULL;
    fConstClass = NULL;
    fConstParent = NULL;
    fNode = NULL;
}

bool SemanticClass::generateCode(QString dir) const
{
    BinaryWriter writer(dir + "/" + fNode->fId + ".class");

    if (!writer.isAlive()) {
        return false;
    }

    // Write magic number and version.
    writer.writeU4(MAGIC_NUMBER);
    writer.writeU2(VERSION_MINOR);
    writer.writeU2(VERSION_MAJOR);

    // Write number of class constants + 1.
    writer.writeU2(fConstantsTable.size() + 1);

    // Write class constants table.
    foreach (SemanticConstant * constant, fConstantsTable) {
        constant->generateCode(&writer);
    }

    // Write access flags.
    writer.writeU2(ACC_SUPER + ACC_PUBLIC);

    // Write CONSTANT_Class for this and parent classes.
    writer.writeU2(fConstClass->fNumber);
    writer.writeU2(fConstParent->fNumber);

    // Write number of class interfaces (0), skipping the table itself.
    writer.writeU2(0);

    // Write number of class fields.
    writer.writeU2(fFieldsTable.size());

    // Write class fields table.
    foreach (SemanticField * field, fFieldsTable) {
        // TODO: Goremykin.
    }

    // Write number of class methods.
    writer.writeU2(fMethodsTable.size());

    // Write class methods table.
    foreach (SemanticMethod * method, fMethodsTable) {
        method->generateCode(&writer, this);
    }

    // Write number of class attributes (0), skipping the table itself.
    writer.writeU2(0);
}

QString SemanticClass::dotForTables(QString previous) const
{
    QString tmp = "\"class " + fConstClass->fRef1->fUtf8 + "\\nparent " + fConstParent->fRef1->fUtf8 + "\"";
    QString result = previous + "->" + tmp + ";\n";
    foreach (SemanticConstant * constant, fConstantsTable) {
        result += constant->dotCode(tmp);
    }

    return result;
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
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Utf8, value);
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
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Integer, "", value);
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
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Class, "", 0, utf8const);
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
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_String, "", 0, utf8const);
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
   SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Fieldref, "", 0, classConst, nameAndTypeConst);
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
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Methodref, "", 0, classConst, nameAndTypeConst);
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
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_NameAndType, "", 0, nameConst, typeConst);
    fConstantsTable << result;
    return result;
}

void SemanticClass::addDefaultAndParentConstructor()
{
    // Add constructor name and descriptor.
    addUtf8Constant(NAME_JAVA_CONSTRUCTOR);
    addUtf8Constant(DESC_JAVA_METHOD_VOID_VOID);

    // Add constructor to the methods table.
    SemanticMethod * constructorThis = new SemanticMethod();
    //SemanticMethod * constructorParent = new SemanticMethod();
    constructorThis->fConstMethodref = addMethodrefConstant(fConstClass->fRef1->fUtf8, NAME_JAVA_CONSTRUCTOR, DESC_JAVA_METHOD_VOID_VOID);
    /*constructorParent->fConstMethodref =*/ addMethodrefConstant(fConstParent->fRef1->fUtf8, NAME_JAVA_CONSTRUCTOR, DESC_JAVA_METHOD_VOID_VOID);

    // Add methods to the table.
    fMethodsTable.insert(NAME_JAVA_CONSTRUCTOR, constructorThis);
}

void SemanticClass::addRTLConstants()
{
    addFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_TYPE,         DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUEINT,     DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUECHAR,    DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUESTRING,  DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUEBOOLEAN, DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUEARRAY,   DESC_JAVA_INTEGER);

    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_PLUS,       DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_MINUS,      DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_MULT,       DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_DIV,        DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_GREATER,    DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_GREATER_EQ, DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_LESS,       DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_LESS_EQ,    DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_EQ,         DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_AND,        DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_OR,         DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_NOT,        DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_SETF,       DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_VECTOR,     DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_ELT,        DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_LIST,       DESC_JAVA_METHOD_ARRAYBASE_BASE);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_PRINT,      DESC_JAVA_METHOD_ARRAYBASE_VOID);
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, RTL_METHOD_ARCHEY,     DESC_JAVA_METHOD_VOID_VOID);
}

bool SemanticClass::hasField(QString name) const
{
    return fFieldsTable.contains(name);
}

bool SemanticClass::hasMethod(QString name) const
{
    foreach (SemanticConstant * constant, fConstantsTable) {
        if (constant->fType == CONSTANT_Methodref && constant->fRef2->fRef1->fUtf8 == name) {
            return true;
        }
    }
    return false;   //fMethodsTable.contains(name);
}

SemanticField * SemanticClass::getField(QString name) const
{
    return fFieldsTable[name];
}

SemanticMethod * SemanticClass::getMethod(QString name) const
{
    return fMethodsTable[name];
}


SemanticField * SemanticClass::addField(const DefinitionNode * node)
{
    SemanticField * newField = new SemanticField();
    // TODO: Fieldref?
    //fFieldsTable.insert(node->fId, newField);
    return newField;
}

SemanticMethod * SemanticClass::addMethod(const DefinitionNode * node)
{
    SemanticMethod * newMethod = new SemanticMethod();
    // Create the descriptor.
    QString desc;
    if (node->fId == NAME_JAVA_METHOD_MAIN) {
        desc = DESC_JAVA_METHOD_ARRAYSTRING_VOID;
    } else if (fConstClass->fRef1->fUtf8 == NAME_JAVA_CLASS_MAINCLASS) {
        desc = createMethodDesc(node->fArguments.size() - 1); // Main class has only static members.
    } else {
        desc = createMethodDesc(node->fArguments.size());
    }
    // Add methodref constant.
    newMethod->fConstMethodref = addMethodrefConstant(fConstClass->fRef1->fUtf8, node->fId, desc);
    newMethod->fIsStatic = (fConstClass->fRef1->fUtf8 == NAME_JAVA_CLASS_MAINCLASS);
    newMethod->fNode = node;
    fMethodsTable.insert(node->fId, newMethod);
    return newMethod;
}

QString SemanticClass::createMethodDesc(int numberOfArguments)
{
    QString result = "(";
    for (int i = 0; i < numberOfArguments; i++) {
        result += DESC_JAVA_CLASS_BASE;
    }
    result += QString(")") + DESC_JAVA_CLASS_BASE;
    return result;
}

SemanticField::SemanticField()
{
    fConstFieldref = NULL;
}

SemanticMethod::SemanticMethod()
{
    fConstMethodref = NULL;
    fIsStatic = false;
    fNode = NULL;
}

void SemanticMethod::generateCode(BinaryWriter * writer, const SemanticClass * curClass) const
{
    // Write access flags.
    writer->writeU2(fIsStatic ? ACC_PUBLIC + ACC_STATIC : ACC_PUBLIC);

    // Write the "utf8" (method name) constant number.
    writer->writeU2(fConstMethodref->fRef2->fRef1->fNumber);

    // Write the "utf8" (descriptor) constant number.
    writer->writeU2(fConstMethodref->fRef2->fRef2->fNumber);

    // Write number of attributes (1).
    writer->writeU2(1);

    // Generate the "Code" attribute.
    generateCodeAttribute(writer, curClass);
}

bool SemanticMethod::hasLocalVar(QString name) const
{
    return fLocalVarsTable.contains(name);
}

SemanticLocalVar * SemanticMethod::addLocalVar(QString name)
{
    // Does it already exist?
    if (fLocalVarsTable.contains(name)) {
        return fLocalVarsTable[name];
    }
    // Create the new variable.
    SemanticLocalVar * result = new SemanticLocalVar(fLocalVarsTable.size(), name); // Numbers start from 0.
    fLocalVarsTable.insert(name, result);
    return result;
}

bool SemanticMethod::isRTLMethod() const
{
    QString name = fConstMethodref->fRef2->fRef1->fUtf8;
    return (name == RTL_METHOD_PLUS ||
            name == RTL_METHOD_MINUS ||
            name == RTL_METHOD_MULT ||
            name == RTL_METHOD_DIV ||
            name == RTL_METHOD_GREATER ||
            name == RTL_METHOD_GREATER_EQ ||
            name == RTL_METHOD_LESS ||
            name == RTL_METHOD_LESS_EQ ||
            name == RTL_METHOD_EQ ||
            name == RTL_METHOD_AND ||
            name == RTL_METHOD_OR ||
            name == RTL_METHOD_NOT ||
            name == RTL_METHOD_SETF ||
            name == RTL_METHOD_VECTOR ||
            name == RTL_METHOD_ELT ||
            name == RTL_METHOD_LIST ||
            name == RTL_METHOD_PRINT);
}

void SemanticMethod::generateCodeAttribute(BinaryWriter * writer, const SemanticClass * curClass) const
{
    // Generate byte code.
    QByteArray byteCode = generateByteCodeMethod();

    // Write number of the "Code" utf8 constant.
    writer->writeU2(curClass->fConstCode->fNumber);

    // TODO: write the attribute length.

    // Write the operands stack size.
    writer->writeU2(STACK_SIZE);

    // Write the local vars table size.
    writer->writeU2(fLocalVarsTable.size());

    // Write the byteCode.
    // TODO

    // Write the exceptions table size (0), skip the table itself.
    writer->writeU2(0);

    // Write number of attributes of "Code" (0), skip them too.
    writer->writeU2(0);

}

QByteArray SemanticMethod::generateByteCodeMethod() const
{
    QByteArray result;
    // TODO!!!!!

    return result;
}

SemanticLocalVar::SemanticLocalVar(int number, QString name)
{
    fNumber = number;
    fName = name;
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
    fMainPart = NULL;
}

QString ProgramNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\n program\"";
    QString result;
    foreach (AttributedNode * node, childNodes()) {
        result += node->dotCode(tmp);
    }
    return result;
}

bool ProgramNode::isCalculable() const
{
    return false;
}

QLinkedList<AttributedNode *> ProgramNode::childNodes() const
{
    QLinkedList<AttributedNode *> result;
    if (fMainPart != NULL) {
        result << fMainPart;
    }
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

    // Finally, create a new program part node which contains the main class.
    fMainPart = new ProgramPartNode();
    fMainPart->fNodeId = 100502;
    fMainPart->fSubType = PROGRAM_PART_TYPE_DEF;
    fMainPart->fDefinition = mainClass;
}

void ProgramNode::semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner) const
{
    // Achtung! @BADCODE! Something weird here!
    // First check the main class without its main method,
    // Second - check all other classes with their methods,
    // Third - check the main class's main method.

    // Add main class and method.
    fMainPart->semantics(program, errorList, curClass, curMethod, false);
    curClass = program->getClass(NAME_JAVA_CLASS_MAINCLASS);
    curMethod = curClass->getMethod(NAME_JAVA_METHOD_MAIN);

    // Main class is added in the above call since it's added as a node during transformation. Add the base class to constants.
    SemanticClass * baseClass = new SemanticClass();
    baseClass->fConstCode = baseClass->addUtf8Constant("Code");
    baseClass->fConstClass = baseClass->addClassConstant(NAME_JAVA_CLASS_BASE);
    baseClass->fConstParent = baseClass->addClassConstant(NAME_JAVA_CLASS_OBJECT);
    baseClass->addDefaultAndParentConstructor();
    baseClass->addRTLConstants();

    // Create tables and check all child nodes.
    foreach (AttributedNode * node, childNodes()) {
        if (node != fMainPart) {
            node->semantics(program, errorList, curClass, curMethod);
        }
    }

    // Process the main method.
    foreach (AttributedNode * node, fMainPart->fDefinition->childNodes()) {
        node->semantics(program, errorList, curClass, curMethod);
    }
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

void ProgramPartNode::semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner) const
{
    foreach (AttributedNode * node, childNodes()) {
        node->semantics(program, errorList, curClass, curMethod, processInner);
    }
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
    case S_EXPR_TYPE_MAKEINSTANCE:
    case S_EXPR_TYPE_SLOTVALUE:
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

void SExpressionNode::semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner) const
{
    // Analyse this node.
    switch (fSubType) {
    case S_EXPR_TYPE_INT:
    case S_EXPR_TYPE_CHAR:
    case S_EXPR_TYPE_STRING:
    case S_EXPR_TYPE_BOOL: {
        // Nothing to check.
        break;
    }
    case S_EXPR_TYPE_ID: {
        // Nothing to check, add to vars table?
        break;
    }
    case S_EXPR_TYPE_FCALL: {
        // Check if the function exists.
        if (!curClass->hasMethod(fId)) {
            *errorList << "Calling undefined function: \"" + fId + "\".";
        } else {
            // Check if number of arguments is the same as in the function definition.
            SemanticMethod * method = curClass->getMethod(fId);
            if (method != NULL && !method->isRTLMethod()) {
                int expectedNumberOfArgs = method->fNode->fArguments.size();
                int realNumberOfArgs = fArguments.size();
                if (expectedNumberOfArgs != realNumberOfArgs) {
                    *errorList << "Method \"" + fId + "\" doesn't take " + QString::number(realNumberOfArgs) + " argument(s).";
                }
            }

            // Check if arguments should be calculable. This also checks calculability of the assignment right side.
            foreach (SExpressionNode * arg, fArguments) {
                if (!arg->isCalculable()) {
                    *errorList << "Only calculable expressions can be passed as arguments.";
                    break;
                }
            }
            // Check if ID's are known.
            foreach (SExpressionNode * arg, fArguments) {
                // Function existance checked before this switch-case since it's a child node.
                if (arg->fSubType == S_EXPR_TYPE_ID && !curMethod->hasLocalVar(arg->fId) && !(fId == NAME_FUNC_SETF && arg == fArguments.first())) {
                    *errorList << "Passing undefined variable: \"" + arg->fId + "\".";
                }
            }

            // Check if assigning to an identifier.
            if (fId == NAME_FUNC_SETF) {
                if (!fArguments.isEmpty() && fArguments.first()->fSubType == S_EXPR_TYPE_ID) {
                    curMethod->addLocalVar(fArguments.first()->fId);
                } else {
                    *errorList << "Can't call SETF with given parameters.";
                }
            }

            if (fId == NAME_FUNC_ELT) {
                if (fArguments.isEmpty() || !fArguments.first()->isValidContainer(curClass, curMethod)) {
                    *errorList << "Can't call ELT with given parameters.";
                }
            }
        }
        break;
    }
    case S_EXPR_TYPE_LOOP_IN: {
        // Check correctness of the array.
        if (!fContainer->isValidContainer(curClass, curMethod)) {
            *errorList << "Wrong container specified for the loop.";
        } else {
            curMethod->addLocalVar(fId);
        }
        break;
    }
    case S_EXPR_TYPE_LOOP_FROM_TO: {
        // Check if borders are calculable.
        if (!fFrom->isCalculable() || !fTo->isCalculable()) {
            *errorList << "Only calculable expressions can be used as loop borders.";
        } else {
            curMethod->addLocalVar(fId);
        }
        break;
    }
    case S_EXPR_TYPE_PROGN: {
        // Child nodes are checked before this switch-case.
        break;
    }
    case S_EXPR_TYPE_IF: {
        // Check if the condition is calculable.
        if (!fCondition->isCalculable()) {
            *errorList << "Only calculable expressions can be used as conditions.";
        }
        break;
    }
    case S_EXPR_TYPE_MAKEINSTANCE: {
        // Check if the class exists. TODO
        break;
    }
    case S_EXPR_TYPE_SLOTVALUE: {
        // Check if the field exists. TODO
        break;
    }
    case S_EXPR_TYPE_ASSIGN_ELT: {
        // Checks are same as for FCALL. TODO
        /*if (!fOperands.last()->isCalculable()) {
            *errorList << "Only calculable expressions can be assigned.";
        }*/
        break;
    }
    case S_EXPR_TYPE_ASSIGN_FIELD: {
        // Checks are same as for FCALL. TODO
        break;
    }
    default: {
        break;
    }
    }

    if (!processInner) {
        return;
    }

    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(program, errorList, curClass, curMethod);
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
        if (result->fSubType == S_EXPR_TYPE_FCALL) {
            result->fId = translateMethodNameToRTLName(result->fId);
        }
        return result;
    } else {
        return NULL;
    }
}

bool SExpressionNode::isValidContainer(SemanticClass * curClass, SemanticMethod * curMethod) const
{
    return  (fSubType == S_EXPR_TYPE_ID) ||
            (fSubType == S_EXPR_TYPE_FCALL && (fId == NAME_FUNC_VECTOR || fId == NAME_FUNC_LIST));
}

QString SExpressionNode::translateMethodNameToRTLName(QString originalName)
{
    if (originalName == NAME_FUNC_PLUS) {
        return RTL_METHOD_PLUS;
    } else if (originalName == NAME_FUNC_MINUS) {
        return RTL_METHOD_MINUS;
    } else if (originalName == NAME_FUNC_MULT) {
        return RTL_METHOD_MULT;
    } else if (originalName ==  NAME_FUNC_DIV) {
        return RTL_METHOD_DIV;
    } else if (originalName ==  NAME_FUNC_GREATER) {
        return RTL_METHOD_GREATER;
    } else if (originalName ==  NAME_FUNC_GREATER_EQ) {
        return RTL_METHOD_GREATER_EQ;
    } else if (originalName ==  NAME_FUNC_LESS) {
        return RTL_METHOD_LESS;
    } else if (originalName ==  NAME_FUNC_LESS_EQ) {
        return RTL_METHOD_LESS_EQ;
    } else if (originalName ==  NAME_FUNC_EQ) {
        return RTL_METHOD_EQ;
    } else {
        return originalName;
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

void SlotPropertyNode::semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner) const
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

void SlotDefinitionNode::semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner) const
{
    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(program, errorList, curClass, curMethod, processInner);
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

void DefinitionNode::semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner) const
{
    SemanticClass * curClassForChildNodes = curClass;
    SemanticMethod * curMethodForChildNodes = curMethod;

    // Analyse this node.
    switch (fSubType) {
    case DEF_TYPE_CLASS: {        
        if (program->hasClass(fId)) {
            // Check if there's no class with same name yet.
            *errorList << "Class " + fId + " is already defined.";
        } else {
            // Add this class to the class table.            
            curClassForChildNodes = program->addClass(this);
            // TODO: add fields to the class.
        }
        break;
    }
    case DEF_TYPE_FUNC: {
        if (curClass->hasMethod(fId)) {
            // Check if there's no function with same name yet.
            *errorList << "Function " + fId + " is already defined.";
        } else {
            // Add this method to the class methods table.
            curMethodForChildNodes = curClass->addMethod(this);

            // Check if all arguments are identifiers, their repetitions, add local vars.
            // TODO: Add "this".
            QMap<QString, int> repetitions;
            foreach (SExpressionNode * arg, fArguments) {
                if (arg->fSubType == S_EXPR_TYPE_ID) {
                    curMethodForChildNodes->addLocalVar(arg->fId);
                    if (repetitions.contains(arg->fId)) {
                        repetitions[arg->fId]++;
                    } else {
                        repetitions.insert(arg->fId, 1);
                    }
                } else {
                    *errorList << "Formal arguments should be identifiers.";
                }
            }
            for (QMap<QString, int>::const_iterator iter = repetitions.constBegin(); iter != repetitions.constEnd(); iter++) {
                if (iter.value() > 1) {
                    *errorList << "Duplication of method arguments: \"" + iter.key() +"\" is repeated " + QString::number(iter.value()) + " times.";
                }
            }
            curClass->addMethod(this);
        }        
        break;
    }
    default: {
        break;
    }
    }

    if (!processInner) {
        return;
    }

    // Analyse all child nodes.
    foreach (AttributedNode * child, childNodes()) {
        child->semantics(program, errorList, curClassForChildNodes, curMethodForChildNodes);
    }
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
            result->fParent = NAME_JAVA_CLASS_BASE;
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

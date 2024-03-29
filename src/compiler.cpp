#include "compiler.h"

SemanticConstant::SemanticConstant(quint16 number, JavaConstantsTypes type, QString utf8, qint32 integer, float floating, SemanticConstant * ref1, SemanticConstant * ref2)
{
    fNumber = number;
    fType = type;
    fUtf8 = utf8;
    fInteger = integer;
    fFloat = floating;
    fRef1 = ref1;
    fRef2 = ref2;
}

void SemanticConstant::generateCode(BinaryWriter * writer) const
{
    // Write the constant type.
    writer->writeU1(fType);

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
        writer->writeF4(fFloat);
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
    case CONSTANT_Fieldref:
    case CONSTANT_Methodref:
    case CONSTANT_InterfaceMethodref: {
        // Write the "class" constant number.
        writer->writeU2(fRef1->fNumber);

        // Write the "name and type" constant number.
        writer->writeU2(fRef2->fNumber);
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

QString SemanticConstant::toString() const
{
    QString result = "<tr>";
    switch (fType) {
    case CONSTANT_Utf8: {
        result += "<td>" + QString::number(fNumber) + " Utf8</td><td>" + fUtf8 + "</td>";
        break;
    }
    case CONSTANT_Integer: {
        result += "<td>" + QString::number(fNumber) + " Integer</td><td>" + QString::number(fInteger) + "</td>";
        break;
    }
    case CONSTANT_Float: {
        result += "<td>" + QString::number(fNumber) + " Float</td><td>" + "" + "</td>";
        break;
    }
    case CONSTANT_Long: {
        result += "<td>" + QString::number(fNumber) + " Long</td><td>" + "" + "</td>";
        break;
    }
    case CONSTANT_Double: {
        result += "<td>" + QString::number(fNumber) + " Double</td><td>" + "" + "</td>";
        break;
    }
    case CONSTANT_Class: {
        result += "<td>" + QString::number(fNumber) + " Class</td><td>refers to #" + QString::number(fRef1->fNumber) + "</td>";
        break;
    }
    case CONSTANT_String: {
        result += "<td>" + QString::number(fNumber) + " String</td><td>refers to #" + QString::number(fRef1->fNumber) + "</td>";
        break;
    }
    case CONSTANT_Fieldref: {
        result += "<td>" + QString::number(fNumber) + " Fieldref</td><td>refers to #" + QString::number(fRef1->fNumber) + ", #" + QString::number(fRef2->fNumber) + "</td>";
        break;
    }
    case CONSTANT_Methodref: {
        result += "<td>" + QString::number(fNumber) + " Methodref</td><td>refers to #" + QString::number(fRef1->fNumber) + ", #" + QString::number(fRef2->fNumber) + "</td>";
        break;
    }
    case CONSTANT_InterfaceMethodref: {
        result += "<td>" + QString::number(fNumber) + " InterfaceMethodref</td><td>refers to #" + QString::number(fRef1->fNumber) + ", #" + QString::number(fRef2->fNumber) + "</td>";
        break;
    }
    case CONSTANT_NameAndType: {
        result += "<td>" + QString::number(fNumber) + " NameAndType</td><td>refers to #" + QString::number(fRef1->fNumber) + ", #" + QString::number(fRef2->fNumber) + "</td>";
        break;
    }
    default: {
        break;
    }
    }
    result += "</tr>";
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
    foreach (SemanticClass * curClass, fClassTable) {
        delete curClass;
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

    // Write the RTL files.
    QResource rBaseClass(":/rtl/rtl/BaseClass.class");
    QResource rLispRTL(":/rtl/rtl/LispRTL.class");
    QFile baseFile(dir + "/" + NAME_JAVA_CLASS_BASE + ".class");
    QFile rtlFile(dir + "/" + NAME_JAVA_CLASS_LISPRTL + ".class");
    if (baseFile.open(QIODevice::WriteOnly)) {
        QDataStream stream(&baseFile);
        const uchar * data = rBaseClass.data();
        for (qint64 i = 0; i < rBaseClass.size(); i++) {
            stream << data[i];
        }
    }
    if (rtlFile.open(QIODevice::WriteOnly)) {
        QDataStream stream(&rtlFile);
        const uchar * data = rLispRTL.data();
        for (qint64 i = 0; i < rLispRTL.size(); i++) {
            stream << data[i];
        }
    }

    // Generate code for all classes.
    foreach (SemanticClass * curClass, fClassTable) {
        result &= curClass->generateCode(dir);
    }

    return result;
}

QString SemanticProgram::dotCode() const
{
    QString result;
    result += fRoot->dotCode("", "");
    return result;
}

QString SemanticProgram::tablesToString() const
{
    QString result = "<html><body>";
    foreach (SemanticClass * curClass, fClassTable) {
        result += "Class " + curClass->fNode->fId + "</br>";
        result += curClass->tablesToString();
        result += "</br>";
    }
    result += "</body></html>";
    return result;
}

ProgramNode * SemanticProgram::root() const
{
    return fRoot;
}

QStringList SemanticProgram::errors() const
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
    SemanticClass * newClass = new SemanticClass(node->fId, node->fParent, node);
    fClassTable.insert(node->fId, newClass);
    return newClass;
}

SemanticClass::SemanticClass(QString name, QString parent, const DefinitionNode * node)
{
    fConstCode = addUtf8Constant("Code");
    fConstClass = addClassConstant(name);
    fConstParent = addClassConstant(parent);
    fNode = node;

    addDefaultAndParentConstructor();
    addBaseClassConstants();
}

SemanticClass::~SemanticClass()
{
    foreach (SemanticConstant * constant, fConstantsTable) {
        delete constant;
    }
    foreach (SemanticField * field, fFieldsTable) {
        delete field;
    }
    foreach (SemanticMethod * method, fMethodsTable) {
        delete method;
    }
    if (fConstructorParent != NULL) {
        delete fConstructorParent;
    }
}

QString SemanticClass::tablesToString() const
{
    QString result;
    result += "Constants</br>";
    result += "<table border=1>";
    foreach (SemanticConstant * constant, fConstantsTable) {
        result += constant->toString();
    }
    result += "</table></br>";
    result += "Methods</br>";
    result += "<table border=1>";
    foreach (SemanticMethod * method, fMethodsTable) {
        QString name = method->fConstMethodref->fRef2->fRef1->fUtf8;

        if (name != NAME_JAVA_METHOD_INIT) {
            result += "<tr><td>" + name + "</td>";
            result += "<td>number of arguments: " + QString::number(method->fNode->fArguments.size()) + "</td>";
            result += "<td>number of local vars: " + QString::number(method->numberOfLocalVars()) + "</td>";
        } else {
            result += "<tr><td>init</td>";
            result += "<td>number of arguments: 1</td>";
            result += "<td>number of local vars: 1</td>";
        }
        result += "</tr>";
    }
    result += "</table>";
    return result;
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
        // TODO
    }

    // Write number of class methods.
    writer.writeU2(fMethodsTable.size());

    // Write class methods table.
    foreach (SemanticMethod * method, fMethodsTable) {
        method->generateCode(&writer, this);
    }

    // Write number of class attributes (0), skipping the table itself.
    writer.writeU2(0);

    return true;
}

SemanticConstant * SemanticClass::addUtf8Constant(QString value)
{
    // Does it already exist?
    SemanticConstant * existed = findUtf8Constant(value);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Utf8, value);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addIntegerConstant(qint32 value)
{
    // Does it already exist?
    SemanticConstant * existed = findIntegerConstant(value);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Integer, "", value);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addFloatConstant(float value)
{
    // Does it already exist?
    SemanticConstant * existed = findFloatConstant(value);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Float, "", 0, value);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addClassConstant(QString name)
{
    // Does it already exist?
    SemanticConstant * existed = findClassConstant(name);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * utf8const = addUtf8Constant(name);
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Class, "", 0, 0, utf8const);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addStringConstant(QString value)
{
    // Does it already exist?
    SemanticConstant * existed = findStringConstant(value);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * utf8const = addUtf8Constant(value);
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_String, "", 0, 0, utf8const);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addFieldrefConstant(QString className, QString fieldName, QString descriptor)
{
    // Does it already exist?
    SemanticConstant * existed = findFieldrefConstant(className, fieldName, descriptor);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * classConst = addClassConstant(className);
    SemanticConstant * nameAndTypeConst = addNameAndTypeConstant(fieldName, descriptor);
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Fieldref, "", 0, 0, classConst, nameAndTypeConst);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addMethodrefConstant(QString className, QString methodName, QString descriptor)
{
    // Does it already exist?
    SemanticConstant * existed = findMethodrefConstant(className, methodName, descriptor);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * classConst = addClassConstant(className);
    SemanticConstant * nameAndTypeConst = addNameAndTypeConstant(methodName, descriptor);
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_Methodref, "", 0, 0, classConst, nameAndTypeConst);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addInterfaceMethodrefConstant(QString interfaceName, QString methodName, QString descriptor)
{
    // Does it already exist?
    SemanticConstant * existed = findInterfaceMethodrefConstant(interfaceName, methodName, descriptor);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * interfaceConst = addClassConstant(interfaceName);
    SemanticConstant * nameAndTypeConst = addNameAndTypeConstant(methodName, descriptor);
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_InterfaceMethodref, "", 0, 0, interfaceConst, nameAndTypeConst);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::addNameAndTypeConstant(QString name, QString type)
{
    // Does it already exist?
    SemanticConstant * existed = findNameAndTypeConstant(name, type);
    if (existed != NULL) {
        return existed;
    }
    // Create the new constant.
    SemanticConstant * nameConst = addUtf8Constant(name);
    SemanticConstant * typeConst = addUtf8Constant(type);
    SemanticConstant * result = new SemanticConstant(fConstantsTable.size() + 1, CONSTANT_NameAndType, "", 0, 0, nameConst, typeConst);
    fConstantsTable << result;
    return result;
}

SemanticConstant * SemanticClass::findUtf8Constant(QString value) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_Utf8 && result->fUtf8 == value) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findIntegerConstant(qint32 value) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_Integer && result->fInteger == value) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findFloatConstant(float value) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_Float && result->fFloat == value) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findClassConstant(QString name) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_Class && result->fRef1->fUtf8 == name) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findStringConstant(QString name) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_String && result->fRef1->fUtf8 == name) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findFieldrefConstant(QString className, QString fieldName, QString descriptor) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_Fieldref &&
            result->fRef1->fRef1->fUtf8 == className &&
            result->fRef2->fRef1->fUtf8 == fieldName &&
            result->fRef2->fRef2->fUtf8 == descriptor) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findMethodrefConstant(QString className, QString methodName, QString descriptor) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_Methodref &&
            result->fRef1->fRef1->fUtf8 == className &&
            result->fRef2->fRef1->fUtf8 == methodName &&
            result->fRef2->fRef2->fUtf8 == descriptor) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findInterfaceMethodrefConstant(QString interfaceName, QString methodName, QString descriptor) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_InterfaceMethodref &&
            result->fRef1->fRef1->fUtf8 == interfaceName &&
            result->fRef2->fRef1->fUtf8 == methodName &&
            result->fRef2->fRef2->fUtf8 == descriptor) {
            return result;
        }
    }
    return NULL;
}

SemanticConstant * SemanticClass::findNameAndTypeConstant(QString name, QString type) const
{
    foreach (SemanticConstant * result, fConstantsTable) {
        if (result->fType == CONSTANT_NameAndType &&
            result->fRef1->fUtf8 == name &&
            result->fRef2->fUtf8 == type) {
            return result;
        }
    }
    return NULL;
}

void SemanticClass::addDefaultAndParentConstructor()
{
    // Add constructor to the methods table.
    fConstructorThis = new SemanticMethod();
    fConstructorParent = new SemanticMethod();

    fConstructorThis->fConstMethodref = addMethodrefConstant(fConstClass->fRef1->fUtf8, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_VOID_VOID);
    fConstructorThis->addLocalVar("this");

    fConstructorParent->fConstMethodref = addMethodrefConstant(fConstParent->fRef1->fUtf8, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_VOID_VOID);
    fConstructorParent->addLocalVar("this");

    // Add methods to the table.
    fMethodsTable.insert(NAME_JAVA_METHOD_INIT, fConstructorThis);
}

void SemanticClass::addBaseClassConstants()
{
    addFieldrefConstant(NAME_JAVA_CLASS_BASE,  NAME_JAVA_FIELD_BASE_TYPE,         DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE,  NAME_JAVA_FIELD_BASE_VALUEINT,     DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE,  NAME_JAVA_FIELD_BASE_VALUECHAR,    DESC_JAVA_CHARACTER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE,  NAME_JAVA_FIELD_BASE_VALUESTRING,  DESC_JAVA_CLASS_STRING);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE,  NAME_JAVA_FIELD_BASE_VALUEBOOLEAN, DESC_JAVA_INTEGER);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE,  NAME_JAVA_FIELD_BASE_VALUELIST,    DESC_JAVA_CLASS_LINKEDLIST);
    addFieldrefConstant(NAME_JAVA_CLASS_BASE,  NAME_JAVA_FIELD_BASE_VALUEVECTOR,  DESC_JAVA_CLASS_VECTOR);

    fConstructorBaseV = new SemanticMethod();
    fConstructorBaseV->fConstMethodref = addMethodrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_VOID_VOID);

    fConstructorBaseI = new SemanticMethod();
    fConstructorBaseI->fConstMethodref = addMethodrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_INTEGER_VOID);

    fConstructorBaseF = new SemanticMethod();
    fConstructorBaseF->fConstMethodref = addMethodrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_FLOAT_VOID);

    fConstructorBaseC = new SemanticMethod();
    fConstructorBaseC->fConstMethodref = addMethodrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_CHARACTER_VOID);

    fConstructorBaseS = new SemanticMethod();
    fConstructorBaseS->fConstMethodref = addMethodrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_STRING_VOID);

    fConstructorBaseB = new SemanticMethod();
    fConstructorBaseB->fConstMethodref = addMethodrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_METHOD_INIT, DESC_JAVA_METHOD_INT_INT_VOID);

    foreach (QString methodName, SemanticMethod::getBaseClassMethods()) {
        addMethodrefConstant(NAME_JAVA_CLASS_BASE, methodName, SemanticMethod::getDescForBaseClassMethod(methodName));
    }
}

void SemanticClass::addRTLMethod(QString methodName)
{
    addMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, methodName, SemanticMethod::getDescForRTLMethod(methodName));
}

/*bool SemanticClass::hasField(QString name) const
{
    return fFieldsTable.contains(name);
}*/

bool SemanticClass::hasMethod(QString name) const
{
    foreach (SemanticConstant * constant, fConstantsTable) {
        if (constant->fType == CONSTANT_Methodref && constant->fRef2->fRef1->fUtf8 == name) {
            return true;
        }
    }
    return false;   //fMethodsTable.contains(name);
}

/*SemanticField * SemanticClass::getField(QString name) const
{
    return fFieldsTable[name];
}*/

SemanticMethod * SemanticClass::getMethod(QString name) const
{
    return fMethodsTable[name];
}

/*SemanticField * SemanticClass::addField(const DefinitionNode * node)
{
    SemanticField * newField = new SemanticField();
    // ...
    return newField;
}*/

SemanticMethod * SemanticClass::addMethod(const DefinitionNode * node)
{
    SemanticMethod * newMethod = new SemanticMethod();
    // Create the descriptor.
    QString desc;
    if (node->fId == NAME_JAVA_METHOD_MAIN) {
        desc = DESC_JAVA_METHOD_ARRAYSTRING_VOID;
    } else if (fConstClass->fRef1->fUtf8 == NAME_JAVA_CLASS_MAINCLASS) {
        desc = SemanticMethod::getDescForRegularMethod(node->fArguments.size()); // Main class has only static members.
    } else {
        desc = SemanticMethod::getDescForRegularMethod(node->fArguments.size() + 1);   // Local vars + "this".
    }
    // Add methodref constant.
    newMethod->fConstMethodref = addMethodrefConstant(fConstClass->fRef1->fUtf8, node->fId, desc);
    newMethod->fIsStatic = (fConstClass->fRef1->fUtf8 == NAME_JAVA_CLASS_MAINCLASS);
    newMethod->fNode = node;
    fMethodsTable.insert(node->fId, newMethod);
    return newMethod;
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

SemanticMethod::~SemanticMethod()
{
    foreach (SemanticLocalVar * var, fLocalVarsTable) {
        delete var;
    }
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

SemanticLocalVar * SemanticMethod::getLocalVar(QString name) const
{
    return fLocalVarsTable[name];
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

quint16 SemanticMethod::numberOfLocalVars() const
{
    return fLocalVarsTable.size();
}

QStringList SemanticMethod::getBaseClassMethods() {
    QStringList result;

    result << "iterator";

    return result;
}

QStringList SemanticMethod::getRTLMethods()
{
    QStringList result;

    result << "plus";
    result << "minus";
    result << "mult";
    result << "div";
    result << "greater";
    result << "greater_eq";
    result << "less";
    result << "less_eq";
    result << "eq";
    result << "and";
    result << "or";
    result << "not";
    result << "setf";
    result << "elt";
    result << "setf_elt";
    result << "list";
    result << "vector";
    result << "length";
    result << "push";
    result << "pop";
    result << "print";
    result << "read_int";
    result << "read_float";
    result << "read_char";
    result << "read_string";
    result << "read_boolean";
    result << "archey";

    return result;
}

bool SemanticMethod::isBaseClassMethod(QString name)
{
    return getBaseClassMethods().contains(name);
}

bool SemanticMethod::isRTLMethod(QString name)
{
    return getRTLMethods().contains(name);
}

bool SemanticMethod::rtlMethodHasArguments(QString name)
{
    return (name != "archey" && !name.startsWith("read_"));
}

QString SemanticMethod::getDescForBaseClassMethod(QString name)
{
    if (name == "iterator") {
        return DESC_JAVA_METHOD_VOID_ITERATOR;
    } else {
        return "";  // Should never get here.
    }
}

QString SemanticMethod::getDescForRTLMethod(QString name)
{
    if (rtlMethodHasArguments(name)) {
        return DESC_JAVA_METHOD_ARRAYBASE_BASE;
    } else {
        return DESC_JAVA_METHOD_VOID_BASE;
    }
}

QString SemanticMethod::getDescForRegularMethod(int numberOfArguments)
{
    QString result = "(";
    for (int i = 0; i < numberOfArguments; i++) {
        result += DESC_JAVA_CLASS_BASE;
    }
    result += QString(")") + DESC_JAVA_CLASS_BASE;
    return result;
}

void SemanticMethod::generateCodeAttribute(BinaryWriter * writer, const SemanticClass * curClass) const
{
    const quint32 EMPTY_CODE_ATTR_LENGTH = 18;  // Length of the "Code" attributes without any tables.

    // Generate byte code.
    QByteArray byteCode = generateByteCodeForMethod(curClass);

    // Write number of the "Code" utf8 constant.
    writer->writeU2(curClass->fConstCode->fNumber);

    // Write the attribute length.
    writer->writeU4(byteCode.size() + EMPTY_CODE_ATTR_LENGTH - 6); // excluding first 6 bytes.

    // Write the operands stack size.
    writer->writeU2(STACK_SIZE);

    // Write the local vars table size.
    writer->writeU2(fLocalVarsTable.size());

    // Write the byteCode size.
    writer->writeU4(byteCode.size());

    // Write the byteCode itself.
    writer->writeByteArray(byteCode);

    // Write the exceptions table size (0), skip the table itself.
    writer->writeU2(0);

    // Write number of attributes of "Code" (0), skip them too.
    writer->writeU2(0);
}

QByteArray SemanticMethod::generateByteCodeForMethod(const SemanticClass * curClass) const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);

    if (fConstMethodref->fRef2->fRef1->fUtf8 == NAME_JAVA_METHOD_INIT) {
        // Generate code for default constructor.
        stream << CMD_ALOAD_0;
        stream << CMD_INVOKESPECIAL << curClass->fConstructorParent->fConstMethodref->fNumber;
        stream << CMD_RETURN;
    } else {
        foreach (quint8 byte, fNode->generateCode(curClass, this)) {
            stream << byte;
        }
    }

    return result;
}

SemanticLocalVar::SemanticLocalVar(quint8 number, QString name)
{
    fNumber = number;
    fName = name;
}

AttributedNode::AttributedNode()
{
}

AttributedNode::~AttributedNode()
{
}

QByteArray AttributedNode::generateCode(const SemanticClass * curClass, const SemanticMethod * curMethod) const
{
    return QByteArray();
}

ProgramNode::ProgramNode() : AttributedNode()
{
    fType = ATTR_TYPE_PROGRAM;
    fMainPart = NULL;
}

ProgramNode::~ProgramNode()
{
    foreach (AttributedNode * child, childNodes()) {
        delete child;
    }
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

    // Add "argv" to arguments.
    SExpressionNode * argv = new SExpressionNode();
    argv->fNodeId = 100501;
    argv->fSubType = S_EXPR_TYPE_ID;
    argv->fId = "argv";
    mainMethod->fArguments << argv;

    // Move all global operators to the main method.
    QLinkedList<ProgramPartNode *>::iterator iter = fParts.begin();
    while (iter != fParts.end()) {
        ProgramPartNode * curNode = *iter;
        if (curNode->fSubType == PROGRAM_PART_TYPE_S_EXPR) {
            mainMethod->fBody << curNode->fSExpression;
            curNode->fSExpression = NULL;
            delete curNode;
            iter = fParts.erase(iter);
        } else {
            ++iter;
        }

    }

    // Main method is done, create main class definition node, add the main method definition node.
    DefinitionNode * mainClass = new DefinitionNode();
    mainClass->fNodeId = 100502;
    mainClass->fSubType = DEF_TYPE_CLASS;
    mainClass->fId = NAME_JAVA_CLASS_MAINCLASS;
    mainClass->fParent = NAME_JAVA_CLASS_OBJECT;
    mainClass->fClassMethods << mainMethod;

    // Finally, create a new program part node which contains the main class.
    fMainPart = new ProgramPartNode();
    fMainPart->fNodeId = 100503;
    fMainPart->fSubType = PROGRAM_PART_TYPE_DEF;
    fMainPart->fDefinition = mainClass;
}

void ProgramNode::semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner)
{
    // Achtung! @BADCODE! Something weird here!
    // First check the main class without its main method,
    // Second - check all other classes with their methods,
    // Third - check the main class's main method.

    // Add main class and method.
    fMainPart->semantics(program, errorList, curClass, curMethod, false);
    curClass = program->getClass(NAME_JAVA_CLASS_MAINCLASS);
    curMethod = curClass->getMethod(NAME_JAVA_METHOD_MAIN);

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

ProgramPartNode::~ProgramPartNode()
{
    foreach (AttributedNode * child, childNodes()) {
        delete child;
    }
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

void ProgramPartNode::semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner)
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
    fCondition = NULL;
    fContainer = NULL;
    fFrom = NULL;
    fTo = NULL;
    fBody1 = NULL;
    fBody2 = NULL;
    fIterator = NULL;
}

SExpressionNode::~SExpressionNode()
{
    foreach (AttributedNode * child, childNodes()) {
        delete child;
    }
}

QString SExpressionNode::dotCode(QString parent, QString label) const
{
    QString tmp = "\"id" + QString::number(fNodeId) + "\\ns_expr:";
    switch (fSubType) {
    case S_EXPR_TYPE_EMPTY: {
        tmp += "empty list\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_INT: {
        tmp += "int\\n" + QString::number(fInteger) + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_FLOAT: {
        tmp += "float\\n" + QString::number(fFloat) + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_CHAR: {
        tmp += "char\\n" + QString(fCharacter) + "\"";
        return parent + "->" + tmp + "[label=\"" + label + "\"];\n";
    }
    case S_EXPR_TYPE_STRING: {
        tmp += "string\\n" + fString + "\"";
        tmp.replace('\n', ' ');
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
        foreach (SExpressionNode * op, fArguments) {
            result += op->dotCode(tmp, "body");
        }
        return result;
    }
    case S_EXPR_TYPE_LOOP_FROM_DOWNTO: {
        tmp += "loop (from downto) " + fId + "\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fFrom->dotCode(tmp, "from");
        result += fTo->dotCode(tmp, "downto");
        foreach (SExpressionNode * op, fArguments) {
            result += op->dotCode(tmp, "body");
        }
        return result;
    }
    case S_EXPR_TYPE_LOOP_WHILE: {
        tmp += "loop (while)\"";
        QString result = parent + "->" + tmp + "[label=\"" + label + "\"];\n";
        result += fCondition->dotCode(tmp, "condition");
        foreach (SExpressionNode * op, fArguments) {
            result += op->dotCode(tmp, "body");
        }
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
    case S_EXPR_TYPE_SETF_FIELD: {
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
    case S_EXPR_TYPE_FLOAT:
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
                fId = NAME_FUNC_SETF_ELT;
                // Remove the first element and concatenate the rest to the ELT's operands list.
                fArguments.removeFirst();
                op1->fArguments << fArguments;
                fArguments = op1->fArguments;
                // Clear the ELT's operands list and delete it.
                op1->fArguments.clear();
                delete op1;
            } else if (op1->fSubType == S_EXPR_TYPE_SLOTVALUE) {
                // Convert "obj.field = value" to ternary operator.
                fSubType = S_EXPR_TYPE_SETF_FIELD;
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

void SExpressionNode::semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner)
{
    // Analyse this node.
    switch (fSubType) {
    case S_EXPR_TYPE_EMPTY: {
        // Nothing to do.
        break;
    }
    case S_EXPR_TYPE_INT: {
        // Add an integer constant if 2 bytes not enough.
        if (fInteger > TWOBYTES_MAX || fInteger < TWOBYTES_MIN) {
            curClass->addIntegerConstant(fInteger);
        }
        break;
    }
    case S_EXPR_TYPE_FLOAT: {
        // Store the float constant.
        curClass->addFloatConstant(fFloat);
        break;
    }
    case S_EXPR_TYPE_CHAR: {
        // Nothing to do.
        break;
    }
    case S_EXPR_TYPE_STRING: {
        // Add a string constant.
        curClass->addStringConstant(fString);
        break;
    }
    case S_EXPR_TYPE_BOOL: {
        // Nothing to do.
        break;
    }
    case S_EXPR_TYPE_ID: {
        // Nothing to do.
        break;
    }
    case S_EXPR_TYPE_FCALL: {
        // Check if the function exists.
        bool isRTLMethod = SemanticMethod::isRTLMethod(fId);
        bool hasMethod = curClass->hasMethod(fId);

        if (!hasMethod && !isRTLMethod) {
            // Error: calling unknow method.
            *errorList << "Calling undefined function: \"" + fId + "\".";
        } else if (!hasMethod && isRTLMethod) {
            // Add the RTL method to constants.
            curClass->addRTLMethod(fId);
        } else if (hasMethod && !isRTLMethod){
            // Check if number of arguments is the same as in the function definition.
            SemanticMethod * method = curClass->getMethod(fId);
            // Check number of arguments.
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
                *errorList << "Can't call \"setf\" with given arguments.";
            }
        }

        // Check if ELT is called on a container.
        if (fId == NAME_FUNC_ELT) {
            if (fArguments.isEmpty() || !fArguments.first()->isValidContainer(curClass, curMethod)) {
                *errorList << "Can't call \"elt\" with given arguments.";
            }
        }
        break;
    }
    case S_EXPR_TYPE_LOOP_IN: {
        // Check correctness of the array.
        if (!fContainer->isValidContainer(curClass, curMethod)) {
            *errorList << "Wrong container specified for the loop.";
        } else {
            // Add constants to deal with iterators.
            curClass->addInterfaceMethodrefConstant(NAME_JAVA_INTERFACE_ITERATOR, NAME_JAVA_METHOD_HASNEXT, DESC_JAVA_METHOD_VOID_BOOLEAN);
            curClass->addInterfaceMethodrefConstant(NAME_JAVA_INTERFACE_ITERATOR, NAME_JAVA_METHOD_NEXT, DESC_JAVA_METHOD_VOID_OBJECT);
            curClass->addMethodrefConstant(NAME_JAVA_CLASS_LINKEDLIST, NAME_JAVA_METHOD_ITERATOR, DESC_JAVA_METHOD_VOID_ITERATOR);
            curClass->addMethodrefConstant(NAME_JAVA_CLASS_VECTOR, NAME_JAVA_METHOD_ITERATOR, DESC_JAVA_METHOD_VOID_ITERATOR);

            // Add the local variable for value.
            curMethod->addLocalVar(fId);
            // Add the local variable for iterator.
            fIterator = curMethod->addLocalVar("," + fId);  // Commas are disabled for regular variable names.
        }
        break;
    }
    case S_EXPR_TYPE_LOOP_FROM_TO:
    case S_EXPR_TYPE_LOOP_FROM_DOWNTO:{
        // Check if borders are calculable.
        if (!fFrom->isCalculable() || !fTo->isCalculable()) {
            *errorList << "Only calculable expressions can be used as loop borders.";
        } else {
            // Add the local variable.
            curMethod->addLocalVar(fId);
        }
        break;
    }
    case S_EXPR_TYPE_LOOP_WHILE: {
        // Check if the condition is calculable.
        if (!fCondition->isCalculable()) {
            *errorList << "Only calculable expressions can be used as conditions.";
        }
        break;
    }
    case S_EXPR_TYPE_PROGN: {
        // Nothing to do, child nodes are checked after this switch-case.
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
    case S_EXPR_TYPE_SETF_FIELD: {
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

QByteArray SExpressionNode::generateCode(const SemanticClass * curClass, const SemanticMethod * curMethod) const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);

    SemanticConstant * constBaseClass = curClass->findClassConstant(NAME_JAVA_CLASS_BASE);

    switch (fSubType) {
    case S_EXPR_TYPE_EMPTY: {
        // Just store a null reference.
        stream << CMD_ACONST_NULL;
        break;
    }
    case S_EXPR_TYPE_INT: {
        // Create an instance of the base class (constructor witn an int).
        SemanticConstant * constValue = curClass->findIntegerConstant(fInteger);
        stream << CMD_NEW << constBaseClass->fNumber;
        stream << CMD_DUP;
        if (constValue != NULL) {
            stream << CMD_LDC_W << constValue->fNumber; // Store the constant number.
        } else {
            stream << CMD_SIPUSH << (qint16)fInteger;   // Store the operand itself.
        }
        stream << CMD_INVOKESPECIAL << curClass->fConstructorBaseI->fConstMethodref->fNumber;
        break;
    }
    case S_EXPR_TYPE_FLOAT: {
        // Create an instance of the base class (constructor witn a float).
        SemanticConstant * constValue = curClass->findFloatConstant(fFloat);
        stream << CMD_NEW << constBaseClass->fNumber;
        stream << CMD_DUP;
        stream << CMD_LDC_W << constValue->fNumber; // Store the constant number.
        stream << CMD_INVOKESPECIAL << curClass->fConstructorBaseF->fConstMethodref->fNumber;
        break;
    }
    case S_EXPR_TYPE_CHAR: {
        // Create an instance of the base class (constructor witn a char).
        stream << CMD_NEW << constBaseClass->fNumber;
        stream << CMD_DUP;
        stream << CMD_BIPUSH << fCharacter;   // Store the operand itself.
        stream << CMD_INVOKESPECIAL << curClass->fConstructorBaseC->fConstMethodref->fNumber;
        break;
    }
    case S_EXPR_TYPE_STRING: {
        // Create an instance of the base class (constructor witn a string).
        SemanticConstant * constValue = curClass->findStringConstant(fString);
        stream << CMD_NEW << constBaseClass->fNumber;
        stream << CMD_DUP;
        stream << CMD_LDC_W << constValue->fNumber;
        stream << CMD_INVOKESPECIAL << curClass->fConstructorBaseS->fConstMethodref->fNumber;
        break;
    }
    case S_EXPR_TYPE_BOOL: {
        // Create an instance of the base class (constructor witn two ints).
        stream << CMD_NEW << constBaseClass->fNumber;
        stream << CMD_DUP;
        stream << CMD_BIPUSH << (quint8)(fBoolean ? 1 : 0);   // Store the operand itself.
        stream << CMD_BIPUSH << (quint8)0;                    // Reserved argument, see the RTL.
        stream << CMD_INVOKESPECIAL << curClass->fConstructorBaseB->fConstMethodref->fNumber;
        break;
    }
    case S_EXPR_TYPE_ID: {
        stream << CMD_ALOAD << curMethod->getLocalVar(fId)->fNumber;
        break;
    }
    case S_EXPR_TYPE_FCALL: {
        if (fId == NAME_FUNC_SETF) {
            // Special case for the SETF call.
            SExpressionNode * left = fArguments.first();
            SExpressionNode * right = fArguments.last();
            // Generate code for the right part.
            foreach (quint8 byte, right->generateCode(curClass, curMethod)) {
                stream << byte;
            }
            // Generate code for the left part.
            stream << CMD_DUP;  // For returning value of the expression.
            stream << CMD_ASTORE << curMethod->getLocalVar(left->fId)->fNumber;
        } else {
            // General case, user-defined or RTL method.
            bool isRTL = SemanticMethod::isRTLMethod(fId);

            // Call the method.
            SemanticConstant * constMethod = NULL;
            if (isRTL) {
                constMethod = curClass->findMethodrefConstant(NAME_JAVA_CLASS_LISPRTL, fId, SemanticMethod::getDescForRTLMethod(fId));
                // Put arguments onto the stack AS AN ARRAY.
                if (SemanticMethod::rtlMethodHasArguments(fId)) {
                    foreach (quint8 byte, collectExpressionsToArray(curClass, curMethod, fArguments)) {
                        stream << byte;
                    }
                }
                // Call the method.
                stream << CMD_INVOKESTATIC << constMethod->fNumber;
            } else {
                int numberOfDescArgs = fArguments.size();  // TODO: +1 for dynamic methods.
                constMethod = curClass->findMethodrefConstant(curClass->fNode->fId, fId, SemanticMethod::getDescForRegularMethod(numberOfDescArgs));
                // Put arguments onto the stack NOT AS AN ARRAY.
                foreach (SExpressionNode * expr, fArguments) {
                    foreach (quint8 byte, expr->generateCode(curClass, curMethod)) {
                        stream << byte;
                    }
                }
                if (curClass->getMethod(fId)->fIsStatic) {
                    stream << CMD_INVOKESTATIC << constMethod->fNumber;
                } else {
                    stream << CMD_INVOKEVIRTUAL << constMethod->fNumber;
                }
            }
        }
        break;
    }
    case S_EXPR_TYPE_LOOP_IN: {
        SemanticConstant * constMethodIterator = curClass->findMethodrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_METHOD_ITERATOR, DESC_JAVA_METHOD_VOID_ITERATOR);
        SemanticConstant * constMethodHasNext = curClass->findInterfaceMethodrefConstant(NAME_JAVA_INTERFACE_ITERATOR, NAME_JAVA_METHOD_HASNEXT, DESC_JAVA_METHOD_VOID_BOOLEAN);
        SemanticConstant * constMethodNext = curClass->findInterfaceMethodrefConstant(NAME_JAVA_INTERFACE_ITERATOR, NAME_JAVA_METHOD_NEXT, DESC_JAVA_METHOD_VOID_OBJECT);
        SemanticLocalVar * value = curMethod->getLocalVar(fId);

        QByteArray codeBody;
        QDataStream streamBody(&codeBody, QIODevice::WriteOnly);
        foreach (SExpressionNode * expr, fArguments) {
            // Generate code for current expression.
            foreach (quint8 byte, expr->generateCode(curClass, curMethod)) {
                streamBody << byte;
            }
            streamBody << CMD_POP;
        }

        const qint16 LENGTH_NEW_ITER  = 2 + 5;           // ALOAD + CMD_INVOKEINTERFACE
        const qint16 LENGTH_IF        = 3;               // IFEQ
        const qint16 LENGTH_NEW_VALUE = 2 + 5 + 3 + 2;   // ALOAD + CMD_INVOKEINTERFACE + CHECKCAST + ASTORE
        const qint16 LENGTH_BODY      = codeBody.size(); // body expressions
        const qint16 LENGTH_GOTO      = 3;               // GOTO

        // Get the iterator by calling BaseClass's method.
        foreach (quint8 byte, fContainer->generateCode(curClass, curMethod)) {
            stream << byte;
        }
        stream << CMD_INVOKEVIRTUAL << constMethodIterator->fNumber;
        // Save it to the local variable.
        stream << CMD_ASTORE << fIterator->fNumber;
        stream << CMD_ALOAD << fIterator->fNumber;

        // The party starts here.
        stream << CMD_INVOKEINTERFACE << constMethodHasNext->fNumber << (quint8)1 << (quint8)0; // Zero at the end is required by jvm.

        stream << CMD_IFEQ << (qint16)(qint16)(LENGTH_IF + LENGTH_NEW_VALUE + LENGTH_BODY + LENGTH_GOTO);

        // Get the current value.
        stream << CMD_ALOAD << fIterator->fNumber;
        stream << CMD_INVOKEINTERFACE << constMethodNext->fNumber << (quint8)1 << (quint8)0;
        stream << CMD_CHECKCAST << curClass->findClassConstant(NAME_JAVA_CLASS_BASE)->fNumber;
        stream << CMD_ASTORE << value->fNumber;

        // Write code of the body.
        foreach (quint8 byte, codeBody) {
            stream << byte;
        }

        // Add the goto.
        stream << CMD_GOTO << (qint16)(-LENGTH_NEW_ITER - LENGTH_IF - LENGTH_NEW_VALUE - LENGTH_BODY);

        // Add a fictive returning value.
        stream << CMD_ACONST_NULL;
        break;
    }
    case S_EXPR_TYPE_LOOP_FROM_TO:
    case S_EXPR_TYPE_LOOP_FROM_DOWNTO: {
        SemanticConstant * constFieldValueInt = curClass->findFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUEINT, DESC_JAVA_INTEGER);
        SemanticLocalVar * counter = curMethod->getLocalVar(fId);

        QByteArray codeFrom = fFrom->generateCode(curClass, curMethod);
        QByteArray codeTo = fTo->generateCode(curClass, curMethod);
        QByteArray codeBody;
        QDataStream streamBody(&codeBody, QIODevice::WriteOnly);
        foreach (SExpressionNode * expr, fArguments) {
            // Generate code for current expression.
            foreach (quint8 byte, expr->generateCode(curClass, curMethod)) {
                streamBody << byte;
            }
            streamBody << CMD_POP;
        }

        const qint16 LENGTH_NEW_ITER = 2 + 3 + codeTo.size() + 3; // ALOAD + GETFIELD + to
        const qint16 LENGTH_IF       = 3;                         // IF_ICMPGT
        const qint16 LENGTH_BODY     = codeBody.size();           // body expressions
        const qint16 LENGTH_IINC     = 11;                        // IINC
        const qint16 LENGTH_GOTO     = 3;                         // GOTO

        // Generate code for the "from" value, assign it to the counter.
        foreach (quint8 byte, codeFrom) {
            stream << byte;
        }
        stream << CMD_ASTORE << counter->fNumber;

        // New iteration: push the counter onto the stack.
        stream << CMD_ALOAD << counter->fNumber;
        stream << CMD_GETFIELD << constFieldValueInt->fNumber;
        // Push the "to" value onto the stack.
        foreach (quint8 byte, codeTo) {
            stream << byte;
        }
        stream << CMD_GETFIELD << constFieldValueInt->fNumber;

        // Compare the counter and the "to" values.
        if (fSubType == S_EXPR_TYPE_LOOP_FROM_TO) {
            stream << CMD_ICMPGT << (qint16)(LENGTH_IF + LENGTH_BODY + LENGTH_IINC + LENGTH_GOTO);
        } else {
            stream << CMD_ICMPLT << (qint16)(LENGTH_IF + LENGTH_BODY + LENGTH_IINC + LENGTH_GOTO);
        }

        // Write code of the body.
        foreach (quint8 byte, codeBody) {
            stream << byte;
        }

        // Add the counter increment/decrement.
        stream << CMD_ALOAD << counter->fNumber;
        stream << CMD_DUP;
        stream << CMD_GETFIELD << constFieldValueInt->fNumber;
        stream << CMD_ICONST_1;
        if (fSubType == S_EXPR_TYPE_LOOP_FROM_TO) {
            stream << CMD_IADD;
        } else {
            stream << CMD_ISUB;
        }
        stream << CMD_PUTFIELD << constFieldValueInt->fNumber;

        // Add the goto.
        stream << CMD_GOTO << (qint16)(-LENGTH_NEW_ITER - LENGTH_IF - LENGTH_BODY - LENGTH_IINC);

        // Add a fictive returning value.
        stream << CMD_ACONST_NULL;
        break;
    }
    case S_EXPR_TYPE_LOOP_WHILE: {
        SemanticConstant * constFieldValueBoolean = curClass->findFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUEBOOLEAN, DESC_JAVA_INTEGER);

        QByteArray codeCondition = fCondition->generateCode(curClass, curMethod);
        QByteArray codeBody;
        QDataStream streamBody(&codeBody, QIODevice::WriteOnly);
        foreach (SExpressionNode * expr, fArguments) {
            // Generate code for current expression.
            foreach (quint8 byte, expr->generateCode(curClass, curMethod)) {
                streamBody << byte;
            }
            streamBody << CMD_POP;
        }

        const qint16 LENGTH_COND = codeCondition.size() + 3; // Including CMD_GETFIELD.
        const qint16 LENGTH_IF = 3;
        const qint16 LENGTH_BODY = codeBody.size();
        const qint16 LENGTH_GOTO = 3;

        // Write code for condition.
        foreach (quint8 byte, fCondition->generateCode(curClass, curMethod)) {
            stream << byte;
        }

        stream << CMD_GETFIELD << constFieldValueBoolean->fNumber;
        stream << CMD_IFEQ << (qint16)(LENGTH_IF + LENGTH_BODY + LENGTH_GOTO);

        // Write code of the body.
        foreach (quint8 byte, codeBody) {
            stream << byte;
        }
        stream << CMD_GOTO << (qint16)(-LENGTH_BODY - LENGTH_IF - LENGTH_COND);

        // Add a fictive returning value.
        stream << CMD_ACONST_NULL;
        break;
    }
    case S_EXPR_TYPE_PROGN: {
        foreach (SExpressionNode * expr, fArguments) {
            // Generate code for current expression.
            foreach (quint8 byte, expr->generateCode(curClass, curMethod)) {
                stream << byte;
            }
            if (expr != fArguments.last()) {
                stream << CMD_POP;
            }
        }
        break;
    }
    case S_EXPR_TYPE_IF: {
        SemanticConstant * constFieldValueBoolean = curClass->findFieldrefConstant(NAME_JAVA_CLASS_BASE, NAME_JAVA_FIELD_BASE_VALUEBOOLEAN, DESC_JAVA_INTEGER);

        QByteArray codeBody1;
        QDataStream streamBody1(&codeBody1, QIODevice::WriteOnly);
        // Generate code for body1.
        foreach (quint8 byte, fBody1->generateCode(curClass, curMethod)) {
            streamBody1 << byte;
        }
        if (codeBody1.isEmpty()) {
            // So this branch has a returning value too.
            streamBody1 << CMD_DUP;
        }

        QByteArray codeBody2;
        QDataStream streamBody2(&codeBody2, QIODevice::WriteOnly);
        if (fBody2 != NULL){
            // Generate code for body2.
            foreach (quint8 byte, fBody2->generateCode(curClass, curMethod)) {
                streamBody2 << byte;
            }
        }
        if (codeBody2.isEmpty()) {
            // So this branch has a returning value too.
            streamBody2 << CMD_DUP;
        }

        const qint16 LENGTH_BODY1 = codeBody1.size();
        const qint16 LENGTH_BODY2 = codeBody2.size();
        const qint16 LENGTH_IF = 3;
        const qint16 LENGTH_GOTO = 3;

        // Write code of the condition.
        foreach (quint8 byte, fCondition->generateCode(curClass, curMethod)) {
            stream << byte;
        }

        stream << CMD_GETFIELD << constFieldValueBoolean->fNumber;
        stream << CMD_IFNE << (qint16)(LENGTH_IF + LENGTH_BODY2 + LENGTH_GOTO);

        // Write code of the body2.
        foreach (quint8 byte, codeBody2) {
            stream << byte;
        }
        stream << CMD_GOTO << (qint16)(LENGTH_GOTO + LENGTH_BODY1);

        // Write code of the body1.
        foreach (quint8 byte, codeBody1) {
            stream << byte;
        }

        break;
    }
    case S_EXPR_TYPE_MAKEINSTANCE: {
        // TODO
        break;
    }
    case S_EXPR_TYPE_SLOTVALUE: {
        // TODO
        break;
    }
    //case S_EXPR_TYPE_SETF_ELT: Handled with S_EXPR_TYPE_FCALL
    case S_EXPR_TYPE_SETF_FIELD: {
        // TODO
        break;
    }
    default: {
        break;
    }
    }
    return result;
}

SExpressionNode * SExpressionNode::fromSyntaxNode(const s_expr_struct * syntaxNode)
{
    if (syntaxNode != NULL) {
        SExpressionNode * result = new SExpressionNode();
        result->fNodeId = syntaxNode->nodeId;
        result->fSubType = syntaxNode->type;
        result->fInteger = syntaxNode->integer;
        result->fFloat = syntaxNode->floating;
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

bool SExpressionNode::isValidContainer(const SemanticClass * curClass, const SemanticMethod * curMethod) const
{
    return  ((fSubType == S_EXPR_TYPE_ID && curMethod->getLocalVar(fId) != NULL) || fSubType == S_EXPR_TYPE_FCALL);
}

QByteArray SExpressionNode::collectExpressionsToArray(const SemanticClass * curClass, const SemanticMethod * curMethod, QLinkedList<SExpressionNode *> expressions) const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);

    SemanticConstant * constBaseClass = curClass->findClassConstant(NAME_JAVA_CLASS_BASE);

    // Create a new array.
    stream << CMD_SIPUSH << (quint16)expressions.size();
    stream << CMD_ANEWARRAY << constBaseClass->fNumber;

    // Set the items.
    quint16 index = 0;
    foreach (SExpressionNode * expr, expressions) {
        stream << CMD_DUP;
        stream << CMD_SIPUSH << index++;

        // Generate code for current expression.
        foreach (quint8 byte, expr->generateCode(curClass, curMethod)) {
            stream << byte;
        }

        stream << CMD_AASTORE;
    }

    return result;
}

SlotPropertyNode::SlotPropertyNode() : AttributedNode()
{
    fType = ATTR_TYPE_SLOT_PROP;
    fInitform = NULL;
}

SlotPropertyNode::~SlotPropertyNode()
{
    foreach (AttributedNode * child, childNodes()) {
        delete child;
    }
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

void SlotPropertyNode::semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner)
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

SlotDefinitionNode::~SlotDefinitionNode()
{
    foreach (AttributedNode * child, childNodes()) {
        delete child;
    }
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

void SlotDefinitionNode::semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner)
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

DefinitionNode::~DefinitionNode()
{
    foreach (AttributedNode * child, childNodes()) {
        delete child;
    }
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

void DefinitionNode::semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner)
{
    SemanticClass * curClassForChildNodes = curClass;
    SemanticMethod * curMethodForChildNodes = curMethod;

    // Analyse this node.
    switch (fSubType) {
    case DEF_TYPE_CLASS: {
        // DEFCLASS is unsupported.
        if (fId != NAME_JAVA_CLASS_MAINCLASS) {
            *errorList << "DEFCLASS is unsupported.";
        }

        if (program->hasClass(fId)) {
            // Check if there's no class with same name yet.
            *errorList << "Class \"" + fId + "\" is already defined.";
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
            *errorList << "Function \"" + fId + "\" is already defined.";
        } else {
            // Add this method to the class methods table.
            curMethodForChildNodes = curClass->addMethod(this);


            // Add "this" to localvars.
            if (!curMethodForChildNodes->fIsStatic) {
                curMethodForChildNodes->addLocalVar("this");
            }

            // Check if all arguments are identifiers, their repetitions, add local vars.
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

QByteArray DefinitionNode::generateCode(const SemanticClass * curClass, const SemanticMethod * curMethod) const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);

    switch (fSubType) {
    case DEF_TYPE_CLASS: {
        break;
    }
    case DEF_TYPE_FUNC: {
        // At least the function should return NULL.
        stream << CMD_ACONST_NULL;
        foreach (SExpressionNode * expr, fBody) {
            // Generate code for current expression.
            foreach (quint8 byte, expr->generateCode(curClass, curMethod)) {
                stream << byte;
            }
            // Remove the calculated value from stack, except the last expression.
            // The last expression is also removed in case of main method.
            if (expr != fBody.last() || fId == NAME_JAVA_METHOD_MAIN) {
                stream << CMD_POP;
            }
        }

        // Add RETURN.
        if (fId == NAME_JAVA_METHOD_MAIN) {
            stream << CMD_RETURN;
        } else {
            stream << CMD_ARETURN;
        }

        break;
    }
    default: {
        break;
    }
    }

    return result;
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
            // TODO
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

#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <QString>
#include <QMap>
#include <QLinkedList>
#include <QFile>
#include <QDir>
#include <QResource>
#include "nodetypes.h"
#include "parser_structs.h"
#include "binarywriter.h"

class SemanticConstant;
class SemanticProgram;
class SemanticClass;
class SemanticField;
class SemanticMethod;
class SemanticLocalVar;
class SemanticAnalyzer;
class AttributedNode;
class ProgramNode;
class ProgramPartNode;
class SExpressionNode;
class SlotPropertyNode;
class SlotDefinitionNode;
class DefinitionNode;

// Names of special RTL functions.
#define NAME_FUNC_SETF                      "setf"
#define NAME_FUNC_ELT                       "elt"
#define NAME_FUNC_SETF_ELT                  "setf_elt"

// Constants to deal with Java and RTL.
#define NAME_JAVA_CLASS_OBJECT              "java/lang/Object"
#define NAME_JAVA_CLASS_STRING              "java/lang/String"
#define NAME_JAVA_CLASS_LINKEDLIST          "java/util/LinkedList"
#define NAME_JAVA_CLASS_VECTOR              "java/util/Vector"
#define NAME_JAVA_CLASS_BASE                "BaseClass"
#define NAME_JAVA_CLASS_LISPRTL             "LispRTL"
#define NAME_JAVA_CLASS_MAINCLASS           "MainClass"
#define NAME_JAVA_INTERFACE_ITERATOR        "java/util/Iterator"
#define NAME_JAVA_METHOD_MAIN               "main"
#define NAME_JAVA_METHOD_INIT               "<init>"
#define NAME_JAVA_METHOD_HASNEXT            "hasNext"
#define NAME_JAVA_METHOD_NEXT               "next"
#define NAME_JAVA_METHOD_ITERATOR           "iterator"

#define NAME_JAVA_FIELD_BASE_TYPE           "type"
#define NAME_JAVA_FIELD_BASE_VALUEINT       "valueInt"
#define NAME_JAVA_FIELD_BASE_VALUEFLOAT     "valueFloat"
#define NAME_JAVA_FIELD_BASE_VALUECHAR      "valueChar"
#define NAME_JAVA_FIELD_BASE_VALUESTRING    "valueString"
#define NAME_JAVA_FIELD_BASE_VALUEBOOLEAN   "valueBoolean"
#define NAME_JAVA_FIELD_BASE_VALUELIST      "valueList"
#define NAME_JAVA_FIELD_BASE_VALUEVECTOR    "valueVector"

#define DESC_JAVA_VOID                      "V"
#define DESC_JAVA_INTEGER                   "I"
#define DESC_JAVA_FLOAT                     "F"
#define DESC_JAVA_CHARACTER                 "C"
#define DESC_JAVA_BOOLEAN                   "Z"
#define DESC_JAVA_CLASS_OBJECT              "L"NAME_JAVA_CLASS_OBJECT";"
#define DESC_JAVA_CLASS_STRING              "L"NAME_JAVA_CLASS_STRING";"
#define DESC_JAVA_CLASS_LINKEDLIST          "L"NAME_JAVA_CLASS_LINKEDLIST";"
#define DESC_JAVA_CLASS_VECTOR              "L"NAME_JAVA_CLASS_VECTOR";"
#define DESC_JAVA_CLASS_BASE                "L"NAME_JAVA_CLASS_BASE";"
#define DESC_JAVA_INTERFACE_ITERATOR        "L"NAME_JAVA_INTERFACE_ITERATOR";"
#define DESC_JAVA_ARRAY_STRING              "["DESC_JAVA_CLASS_STRING
#define DESC_JAVA_ARRAY_BASE                "["DESC_JAVA_CLASS_BASE

#define DESC_JAVA_METHOD_VOID_VOID          "()"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_VOID_BOOLEAN       "()"DESC_JAVA_BOOLEAN
#define DESC_JAVA_METHOD_VOID_OBJECT        "()"DESC_JAVA_CLASS_OBJECT
#define DESC_JAVA_METHOD_VOID_BASE          "()"DESC_JAVA_CLASS_BASE
#define DESC_JAVA_METHOD_VOID_ITERATOR      "()"DESC_JAVA_INTERFACE_ITERATOR
#define DESC_JAVA_METHOD_INTEGER_VOID       "("DESC_JAVA_INTEGER")"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_FLOAT_VOID         "("DESC_JAVA_FLOAT")"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_CHARACTER_VOID     "("DESC_JAVA_CHARACTER")"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_STRING_VOID        "("DESC_JAVA_CLASS_STRING")"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_INT_INT_VOID       "("DESC_JAVA_INTEGER DESC_JAVA_INTEGER")"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_BASE_VOID          "("DESC_JAVA_CLASS_BASE")"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_ARRAYSTRING_VOID   "("DESC_JAVA_ARRAY_STRING")"DESC_JAVA_VOID
#define DESC_JAVA_METHOD_ARRAYBASE_BASE     "("DESC_JAVA_ARRAY_BASE")"DESC_JAVA_CLASS_BASE

const qint16 TWOBYTES_MAX         = 32767;
const qint16 TWOBYTES_MIN         = -32768;

const quint32 MAGIC_NUMBER        = 0xCAFEBABE;
const quint16 VERSION_MINOR       = 0x0000;
const quint16 VERSION_MAJOR       = 0x0032;   // Took a few hours to find out that java 7 is a buggy pile of shit.

const quint16 ACC_PUBLIC          = 0x0001;
const quint16 ACC_PRIVATE         = 0x0002;
const quint16 ACC_PROTECTED       = 0x0004;
const quint16 ACC_STATIC          = 0x0008;
const quint16 ACC_FINAL           = 0x0010;
const quint16 ACC_SUPER           = 0x0020;
const quint16 ACC_VOLATILE        = 0x0040;
const quint16 ACC_TRANSIENT       = 0x0080;
const quint16 ACC_INTERFACE       = 0x0200;
const quint16 ACC_ABSTRACT        = 0x0400;
const quint16 ACC_SYNTHETIC       = 0x1000;
const quint16 ACC_ANNOTATION      = 0x2000;
const quint16 ACC_ENUM            = 0x4000;

const quint8  CMD_ACONST_NULL     = 0x01;
const quint8  CMD_ICONST_1        = 0x04;
const quint8  CMD_BIPUSH          = 0x10;
const quint8  CMD_SIPUSH          = 0x11;
const quint8  CMD_LDC             = 0x12;
const quint8  CMD_LDC_W           = 0x13;
const quint8  CMD_ILOAD           = 0x15;
const quint8  CMD_ALOAD           = 0x19;
const quint8  CMD_ALOAD_0         = 0x2A;
const quint8  CMD_ISTORE          = 0x36;
const quint8  CMD_ASTORE          = 0x3A;
const quint8  CMD_AASTORE         = 0x53;
const quint8  CMD_POP             = 0x57;
const quint8  CMD_DUP             = 0x59;
const quint8  CMD_IADD            = 0x60;
const quint8  CMD_ISUB            = 0x64;
const quint8  CMD_IFEQ            = 0x99;
const quint8  CMD_IFNE            = 0x9A;
const quint8  CMD_ICMPEQ          = 0x9F;
const quint8  CMD_ICMPNE          = 0xA0;
const quint8  CMD_ICMPLT          = 0xA1;
const quint8  CMD_ICMPLE          = 0xA4;
const quint8  CMD_ICMPGT          = 0xA3;
const quint8  CMD_ICMPGE          = 0xA2;
const quint8  CMD_GOTO            = 0xA7;
const quint8  CMD_IINC            = 0x84;
const quint8  CMD_ARETURN         = 0xB0;
const quint8  CMD_RETURN          = 0xB1;
const quint8  CMD_GETFIELD        = 0xB4;
const quint8  CMD_PUTFIELD        = 0xB5;
const quint8  CMD_INVOKEVIRTUAL   = 0xB6;
const quint8  CMD_INVOKESPECIAL   = 0xB7;
const quint8  CMD_INVOKESTATIC    = 0xB8;
const quint8  CMD_INVOKEINTERFACE = 0xB9;
const quint8  CMD_NEW             = 0xBB;
const quint8  CMD_ANEWARRAY       = 0xBD;
const quint8  CMD_CHECKCAST       = 0xC0;

const quint16 STACK_SIZE          = 1024;

/**
 * @brief Java constants types.
 */
enum JavaConstantsTypes
{
    CONSTANT_Utf8 = 1,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_Class = 7,
    CONSTANT_String = 8,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_NameAndType = 12
};

/**
 * @brief Attributed nodes types.
 */
enum AttributedNodeType
{
    ATTR_TYPE_PROGRAM,      // Program.
    ATTR_TYPE_PROGRAM_PART, // Program part (s-expression or class/function definition).
    ATTR_TYPE_S_EXPR,       // S-expression.
    ATTR_TYPE_SLOT_PROP,    // Slot property.
    ATTR_TYPE_SLOT_DEF,     // Slot definition.
    ATTR_TYPE_DEFINITION    // Class or function definition.
};

/**
 * @brief Represents a constant.
 */
class SemanticConstant
{
public:
    quint16            fNumber;   // Number in the constants table, counts from 0.
    JavaConstantsTypes fType;     // Type of the constant.
    QString            fUtf8;     // UTF-8 string.
    qint32             fInteger;  // Integer value.
    float              fFloat;    // Float value.
    SemanticConstant * fRef1;     // Pointer to another constant if required.
    SemanticConstant * fRef2;     // Pointer to another constant if required.

    SemanticConstant(quint16 number = 0, JavaConstantsTypes type = CONSTANT_Utf8, QString utf8 = "",
                     qint32 integer = 0, float floating = 0, SemanticConstant * ref1 = NULL, SemanticConstant * ref2 = NULL);

    void generateCode(BinaryWriter * writer) const;
    QString toString() const;
};

/**
 * @brief Represents the semantic analyzer that encapsulates all stuff.
 */
class SemanticProgram
{
public:
    SemanticProgram();
    SemanticProgram(const program_struct * root);
    ~SemanticProgram();

    void doTransform();
    bool doSemantics();
    bool doGenerateCode(QString dir) const;

    QString dotCode() const;
    QString tablesToString() const;

    ProgramNode * root() const;
    QStringList errors() const;

    bool hasClass(QString name) const;
    SemanticClass * getClass(QString name) const;
    SemanticClass * addClass(const DefinitionNode * node);

private:
    ProgramNode                    * fRoot;       // Root of the attributed tree.
    QMap<QString, SemanticClass *>   fClassTable; // Class table.
    QStringList                      fErrors;     // Semantic errors messages.
};

/**
 * @brief Represents a class.
 */
class SemanticClass
{
public:
    SemanticConstant               * fConstCode;      // Utf8 constant "Code".
    SemanticConstant               * fConstClass;     // Name of the class (CONSTANT_Class).
    SemanticConstant               * fConstParent;    // Name of the parent class (CONSTANT_Class).
    DefinitionNode const           * fNode;           // Corresponding tree node.

    SemanticMethod                 * fConstructorBaseV;
    SemanticMethod                 * fConstructorBaseI;
    SemanticMethod                 * fConstructorBaseF;
    SemanticMethod                 * fConstructorBaseC;
    SemanticMethod                 * fConstructorBaseS;
    SemanticMethod                 * fConstructorBaseB;
    SemanticMethod                 * fConstructorThis;
    SemanticMethod                 * fConstructorParent;

    SemanticClass(QString name, QString parent, const DefinitionNode * node = NULL);
    ~SemanticClass();

    QString tablesToString() const;
    bool generateCode(QString dir) const;

    SemanticConstant * addUtf8Constant(QString value);
    SemanticConstant * addIntegerConstant(qint32 value);
    SemanticConstant * addFloatConstant(float value);
    SemanticConstant * addClassConstant(QString name);
    SemanticConstant * addStringConstant(QString value);
    SemanticConstant * addFieldrefConstant(QString className, QString fieldName, QString descriptor);
    SemanticConstant * addMethodrefConstant(QString className, QString methodName, QString descriptor);
    SemanticConstant * addInterfaceMethodrefConstant(QString interfaceName, QString methodName, QString descriptor);
    SemanticConstant * addNameAndTypeConstant(QString name, QString type);

    SemanticConstant * findUtf8Constant(QString value) const;
    SemanticConstant * findIntegerConstant(qint32 value) const;
    SemanticConstant * findFloatConstant(float value) const;
    SemanticConstant * findClassConstant(QString name) const;
    SemanticConstant * findStringConstant(QString name) const;
    SemanticConstant * findFieldrefConstant(QString className, QString fieldName, QString descriptor) const;
    SemanticConstant * findMethodrefConstant(QString className, QString methodName, QString descriptor) const;
    SemanticConstant * findInterfaceMethodrefConstant(QString interfaceName, QString methodName, QString descriptor) const;
    SemanticConstant * findNameAndTypeConstant(QString name, QString type) const;

    void addDefaultAndParentConstructor();
    void addBaseClassConstants();
    void addRTLMethod(QString methodName);

    //bool hasField(QString name) const;
    bool hasMethod(QString name) const;
    //SemanticField * getField(QString name) const;
    SemanticMethod * getMethod(QString name) const;
    //SemanticField * addField(const DefinitionNode * node);
    SemanticMethod * addMethod(const DefinitionNode * node);

private:
    QLinkedList<SemanticConstant *>  fConstantsTable; // Constants table.
    QMap<QString, SemanticField *>   fFieldsTable;    // Fields table.
    QMap<QString, SemanticMethod *>  fMethodsTable;   // Methods table.
};

/**
 * @brief Represents a class field.
 */
class SemanticField
{
public:
    SemanticConstant * fConstFieldref;  // CONSTANT_Fieldref constant.
    bool               fIsStatic;       // Is this field static?

    SemanticField();
};

/**
 * @brief Represents a class method.
 */
class SemanticMethod
{
public:
    SemanticConstant     * fConstMethodref; // CONSTANT_Methodref constant.
    bool                   fIsStatic;       // Is this method static?
    DefinitionNode const * fNode;           // Corresponding tree node.

    SemanticMethod();
    ~SemanticMethod();

    void generateCode(BinaryWriter * writer, const SemanticClass * curClass) const;

    bool hasLocalVar(QString name) const;
    SemanticLocalVar * getLocalVar(QString name) const;
    SemanticLocalVar * addLocalVar(QString name);
    quint16 numberOfLocalVars() const;

    static QStringList getBaseClassMethods();
    static QStringList getRTLMethods();
    static bool isBaseClassMethod(QString name);
    static bool isRTLMethod(QString name);
    static bool rtlMethodHasArguments(QString name);
    static QString getDescForBaseClassMethod(QString name);
    static QString getDescForRTLMethod(QString name);
    static QString getDescForRegularMethod(int numberOfArguments);

private:
    QMap<QString, SemanticLocalVar *> fLocalVarsTable; // Local variables table.

    void generateCodeAttribute(BinaryWriter * writer, const SemanticClass * curClass) const;
    QByteArray generateByteCodeForMethod(const SemanticClass * curClass) const;
};

/**
 * @brief Represents a local variable.
 */
class SemanticLocalVar
{
public:
    quint8   fNumber; // Number of the variable.
    QString  fName;   // Name of the variable.

    SemanticLocalVar(quint8 number = 0, QString name = "");
};

/**
 * @brief Represents an attributed node.
 */
class AttributedNode
{
public:
    int                fNodeId; // Id of the node.
    AttributedNodeType fType;   // Type of the node.

    /**
     * @brief Default constructor.
     */
    AttributedNode();

    /**
     * @brief Destructor should be implemented in child classes for memory deallocation.
     */
    virtual ~AttributedNode();

    /**
     * @brief Generates dot code for this node.
     */
    virtual QString dotCode(QString parent, QString label = "") const = 0;

    /**
     * @brief Checks if this node calculable, e.g. can be used as a condition, initform etc.
     */
    virtual bool isCalculable() const = 0;

    /**
     * @brief Returns all child nodes of this node.
     */
    virtual QLinkedList<AttributedNode *> childNodes() const = 0;

    /**
     * @brief Transforms the subtree of this node.
     */
    virtual void transform() = 0;

    /**
     * @brief Does the semantic analysis of the node: checks, tables, etc.
     */
    virtual void semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner = true) = 0;

    /**
     * @brief Generates bytecode for this node.
     */
    virtual QByteArray generateCode(const SemanticClass * curClass, const SemanticMethod * curMethod) const;
};

/**
 * @brief Represents an attributed program node.
 */
class ProgramNode : public AttributedNode
{
public:
    ProgramPartNode                * fMainPart; // Main class and method definition.
    QLinkedList<ProgramPartNode *>   fParts;    // All other parts.

    ProgramNode();
    ~ProgramNode();
    virtual QString dotCode(QString parent, QString label = "") const;
    virtual void semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner = true);
    virtual bool isCalculable() const;
    virtual QLinkedList<AttributedNode *> childNodes() const;
    virtual void transform();
    static ProgramNode * fromSyntaxNode(const program_struct * syntaxNode);
};

/**
 * @brief Represents an attributed program part node.
 */
class ProgramPartNode : public AttributedNode
{
public:
    program_part_type fSubType;
    SExpressionNode * fSExpression;
    DefinitionNode  * fDefinition;

    ProgramPartNode();
    ~ProgramPartNode();
    virtual QString dotCode(QString parent, QString label = "") const;
    virtual bool isCalculable() const;
    virtual QLinkedList<AttributedNode *> childNodes() const;
    virtual void transform();
    virtual void semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner = true);
    static ProgramPartNode * fromSyntaxNode(const program_part_struct * syntaxNode);
};

/**
 * @brief Represents an attributed s-expression node.
 */
class SExpressionNode : public AttributedNode
{
public:
    s_expr_type                       fSubType;
    qint32                            fInteger;
    float                             fFloat;
    qint8                             fCharacter;
    QString                           fString;
    qint32                            fBoolean;
    QString                           fId;
    QString                           fSlotValueObject;
    QString                           fSlotValueSlot;
    QLinkedList<SExpressionNode *>    fArguments;
    SExpressionNode                 * fCondition;
    SExpressionNode                 * fContainer;
    SExpressionNode                 * fFrom;
    SExpressionNode                 * fTo;
    SExpressionNode                 * fBody1;
    SExpressionNode                 * fBody2;
    SemanticLocalVar                * fIterator; // Additional local variable to deal with iterators.

    SExpressionNode();
    ~SExpressionNode();
    virtual QString dotCode(QString parent, QString label = "") const;
    virtual bool isCalculable() const;
    virtual QLinkedList<AttributedNode *> childNodes() const;
    virtual void transform();
    virtual void semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner = true);
    virtual QByteArray generateCode(const SemanticClass * curClass, const SemanticMethod * curMethod) const;
    static SExpressionNode * fromSyntaxNode(const s_expr_struct * syntaxNode);

private:
    bool isValidContainer(const SemanticClass * curClass, const SemanticMethod * curMethod) const;
    QByteArray collectExpressionsToArray(const SemanticClass * curClass, const SemanticMethod * curMethod, QLinkedList<SExpressionNode *> expressions) const;
};

/**
 * @brief Represents an attributed slot property node.
 */
class SlotPropertyNode : public AttributedNode
{
public:
    slot_prop_type    fSubType;
    SExpressionNode * fInitform;
    QString           fId;
    slot_alloc_type   fAllocationType;

    SlotPropertyNode();
    ~SlotPropertyNode();
    virtual QString dotCode(QString parent, QString label = "") const;
    virtual bool isCalculable() const;
    virtual QLinkedList<AttributedNode *> childNodes() const;
    virtual void transform();
    virtual void semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner = true);
    static SlotPropertyNode * fromSyntaxNode(const slot_prop_struct * syntaxNode);
};

/**
 * @brief Represents an attributed slot definition node.
 */
class SlotDefinitionNode : public AttributedNode
{
public:
    QString                         fId;
    QLinkedList<SlotPropertyNode *> fProperties;

    SlotDefinitionNode();
    ~SlotDefinitionNode();
    virtual QString dotCode(QString parent, QString label = "") const;
    virtual bool isCalculable() const;
    virtual QLinkedList<AttributedNode *> childNodes() const;
    virtual void transform();
    virtual void semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner = true);
    static SlotDefinitionNode * fromSyntaxNode(const slot_def_struct * syntaxNode);
};

/**
 * @brief Represents an attributed list node.
 */
class DefinitionNode : public AttributedNode
{
public:
    def_type                          fSubType;
    QString                           fId;
    QString                           fParent;
    QLinkedList<SExpressionNode *>    fArguments;
    QLinkedList<SlotDefinitionNode *> fClassFields;
    QLinkedList<DefinitionNode *>     fClassMethods;
    QLinkedList<SExpressionNode *>    fBody;

    DefinitionNode();
    ~DefinitionNode();
    virtual QString dotCode(QString parent, QString label = "") const;
    virtual bool isCalculable() const;
    virtual QLinkedList<AttributedNode *> childNodes() const;
    virtual void transform();
    virtual void semantics(SemanticProgram * program, QStringList * errorList, SemanticClass * curClass, SemanticMethod * curMethod, bool processInner = true);
    virtual QByteArray generateCode(const SemanticClass * curClass, const SemanticMethod * curMethod) const;
    static DefinitionNode * fromSyntaxNode(const def_struct * syntaxNode);
};

#endif // SEMANTICANALYZER_H

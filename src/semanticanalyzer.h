#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <QString>
#include <QMap>
#include <QLinkedList>
#include "nodetypes.h"
#include "parser_structs.h"

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

#define TWOBYTES_MAX = 327687;
#define TWOBYTES_MIN = -32768;

// Names of supported Lisp functions.
#define NAME_FUNC_PLUS               "+"
#define NAME_FUNC_MINUS              "="
#define NAME_FUNC_MULT               "*"
#define NAME_FUNC_DIV                "/"
#define NAME_FUNC_GREATER            ">"
#define NAME_FUNC_GREATER_EQ         ">="
#define NAME_FUNC_LESS               "<"
#define NAME_FUNC_LESS_EQ            "<="
#define NAME_FUNC_EQ                 "="
#define NAME_FUNC_AND                "and"
#define NAME_FUNC_OR                 "or"
#define NAME_FUNC_NOT                "not"
#define NAME_FUNC_DEFPARAMETER       "defparameter"
#define NAME_FUNC_DEFVAR             "defvar"
#define NAME_FUNC_SETF               "setf"
#define NAME_FUNC_LET                "let"
#define NAME_FUNC_VECTOR             "vector"
#define NAME_FUNC_VECTOR_PUSH        "vector-push"
#define NAME_FUNC_VECTOR_POP         "vector-pop"
#define NAME_FUNC_ELT                "elt"
#define NAME_FUNC_LIST               "list"
#define NAME_FUNC_LIST_LENGTH        "list-length"
#define NAME_FUNC_LENGTH             "length"
#define NAME_FUNC_FIND               "find"
#define NAME_FUNC_POSITION           "position"
#define NAME_FUNC_REMOVE             "remove"
#define NAME_FUNC_SUBSTITUTE         "substitute"
#define NAME_FUNC_CONCATENATE        "concatenate"
#define NAME_FUNC_OPEN               "open"
#define NAME_FUNC_CLOSE              "close"
#define NAME_FUNC_WITH_OPEN_FILE     "with-open-file"
#define NAME_FUNC_PRINT              "print"

// Constants to deal with Java and RTL.
#define NAME_JAVA_CLASS_OBJECT       "java/lang/Object"
#define NAME_JAVA_CLASS_BASECLASS    "BaseClass"
#define NAME_JAVA_CLASS_MAINCLASS    "MainClass"
#define NAME_JAVA_METHOD_MAIN        "main"
#define NAME_JAVA_CONSTRUCTOR        "<init>"

#define DESC_JAVA_VOID                      "V"
#define DESC_JAVA_INTEGER                   "I"
#define DESC_JAVA_STRING                    "Ljava/lang/String;"
#define DESC_JAVA_CLASS_BASECLASS           "L"NAME_JAVA_CLASS_BASECLASS";"
#define DESC_JAVA_ARRAY                     "["DESC_JAVA_CLASS_BASECLASS
#define DESC_JAVA_CONSTRUCTOR_VOID          "()V"
#define DESC_JAVA_CONSTRUCTOR_INT           "("DESC_JAVA_STRING")V"
#define DESC_JAVA_CONSTRUCTOR_STRING        "("DESC_JAVA_STRING")V"
#define DESC_JAVA_CONSTRUCTOR_ARRAY_STRING  "(["DESC_JAVA_STRING")V"
#define DESC_JAVA_CONSTRUCTOR_BASE          "("NAME_JAVA_CLASS_BASECLASS")V"

#define RTL_BASECLASS_TYPE_DAFUQ     0
#define RTL_BASECLASS_TYPE_INT       1
#define RTL_BASECLASS_TYPE_CHAR      2
#define RTL_BASECLASS_TYPE_STRING    3
#define RTL_BASECLASS_TYPE_BOOLEAN   4
#define RTL_BASECLASS_TYPE_ARRAY     5
#define RTL_METHOD_PLUS              "plus"
#define RTL_METHOD_MINUS             "minus"
#define RTL_METHOD_MULT              "mult"
#define RTL_METHOD_DIV               "div"
#define RTL_METHOD_GREATER           "greater"
#define RTL_METHOD_GREATER_EQ        "greater_eq"
#define RTL_METHOD_LESS              "less"
#define RTL_METHOD_LESS_EQ           "less_eq"
#define RTL_METHOD_EQ                "eq"
#define RTL_METHOD_AND               "and"
#define RTL_METHOD_OR                "or"
#define RTL_METHOD_NOT               "not"
#define RTL_METHOD_SETF              "setf"
#define RTL_METHOD_VECTOR            "vector"
#define RTL_METHOD_ELT               "elt"
#define RTL_METHOD_LIST              "list"
#define RTL_METHOD_PRINT             "print"

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
    int                fId;       // Number in the constants table.
    JavaConstantsTypes fType;     // Type of the constant.
    QString            fUtf8;     // UTF-8 string.
    int                fInteger;  // Integer value.
    SemanticConstant * fRef1;     // Pointer to another constant if required.
    SemanticConstant * fRef2;     // Pointer to another constant if required.

    SemanticConstant(int id = -1, JavaConstantsTypes type = CONSTANT_Utf8, QString utf8 = "",
                     int integer = 0, SemanticConstant * ref1 = NULL, SemanticConstant * ref2 = NULL);
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
    ProgramNode * getRoot() const;
    QLinkedList<QString> getErrors() const;
    bool doSemantics();
    void doTransform();
    SemanticClass * addClass(const DefinitionNode * node);

//private:
    ProgramNode                    * fRoot;       // Root of the attributed tree.
    QMap<QString, SemanticClass *>   fClassTable; // Class table.
    QLinkedList<QString>             fErrors;     // Semantic errors messages.
};

/**
 * @brief Represents a class.
 */
class SemanticClass
{
public:
    SemanticConstant               * fConstClass;     // Name of the class (CONSTANT_Class).
    SemanticConstant               * fConstParent;    // Name of the parent class (CONSTANT_Class).
    QLinkedList<SemanticConstant *>  fConstantsTable; // Constants table.
    QMap<QString, SemanticField *>   fFieldsTable;    // Fields table.
    QMap<QString, SemanticMethod *>  fMethodsTable;   // Methods table.
    DefinitionNode const           * fNode;           // Corresponding tree node.

    SemanticClass();
    SemanticConstant * addUtf8Constant(QString value);
    SemanticConstant * addIntegerConstant(int value);
    SemanticConstant * addClassConstant(QString name);
    SemanticConstant * addStringConstant(QString value);
    SemanticConstant * addFieldrefConstant(QString className, QString fieldName, QString descriptor);
    SemanticConstant * addMethodrefConstant(QString className, QString methodName, QString descriptor);
    SemanticConstant * addNameAndTypeConstant(QString name, QString type);

    void addDefaultAndParentConstructor();
    void addRTLConstants();

    SemanticField * addField(const DefinitionNode * node);
    SemanticMethod * addMethod(const DefinitionNode * node);

private:
    static QString createMethodDesc(int numberOfArguments);
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
    SemanticConstant                  * fConstMethodref; // CONSTANT_Methodref constant.
    QMap<QString, SemanticLocalVar *>   fLocalVarsTable; // Local variables table.
    bool                                fIsStatic;       // Is this method static?
    DefinitionNode const              * fNode;           // Corresponding tree node.

    SemanticMethod();
    SemanticLocalVar * addLocalVarConstant(QString name);
};

/**
 * @brief Represents a local variable.
 */
class SemanticLocalVar
{
public:
    int      fNumber; // Number of the variable.
    QString  fName;   // Name of the variable.
    SemanticLocalVar(int number = -1, QString name = "");
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
    virtual QLinkedList<AttributedNode *> childNodes() const;

    /**
     * @brief Transforms the subtree of this node.
     */
    virtual void transform() = 0;

    /**
     * @brief Does the semantic analysis of the node: checks, tables, etc.
     */
    virtual void semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const = 0;
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
    QString dotCode(QString parent, QString label = "") const;
    void semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();
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
    QString dotCode(QString parent, QString label = "") const;
    void semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();
    static ProgramPartNode * fromSyntaxNode(const program_part_struct * syntaxNode);
};

/**
 * @brief Represents an attributed s-expression node.
 */
class SExpressionNode : public AttributedNode
{
public:
    s_expr_type                       fSubType;
    int                               fInteger;
    char                              fCharacter;
    QString                           fString;
    int                               fBoolean;
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

    SExpressionNode();
    QString dotCode(QString parent, QString label = "") const;
    void semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();
    static SExpressionNode * fromSyntaxNode(const s_expr_struct * syntaxNode);
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
    QString dotCode(QString parent, QString label = "") const;
    void semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();
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
    QString dotCode(QString parent, QString label = "") const;
    void semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();
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
    QString dotCode(QString parent, QString label = "") const;
    void semantics(SemanticProgram * program, QLinkedList<QString> * errorList, SemanticClass * curClass, SemanticMethod * curMethod) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();
    static DefinitionNode * fromSyntaxNode(const def_struct * syntaxNode);
};

#endif // SEMANTICANALYZER_H

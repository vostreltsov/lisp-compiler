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
class SemanticLocalVariable;
class SemanticAnalyzer;
class AttributedNode;
class ProgramNode;
class SExpressionNode;
class ListNode;

// Names of supported Lisp functions.
#define NAME_FUNC_SETF    "setf"
#define NAME_FUNC_VECTOR  "vector"
#define NAME_FUNC_ELT     "elt"
#define NAME_FUNC_LIST    "list"

// Names of misc Java stuff.
#define NAME_JAVA_CLASS_OBJECT     "java/lang/Object"
#define NAME_JAVA_CLASS_BASECLASS  "BaseClass"
#define NAME_JAVA_CLASS_MAINCLASS  "MainClass"
#define NAME_JAVA_METHOD_MAIN      "main"
#define NAME_JAVA_CONSTRUCTOR      "<init>"

#define DESC_JAVA_CLASS_BASECLASS  "L"NAME_CLASS_JAVA_BASECLASS";"

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
    CONSTANT_Class = 7 ,
    CONSTANT_String = 8,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_NameAndType = 12
};

/**
 * @brief Attributed nodes types.
 */
enum AttributedTypes
{
    ATTR_TYPE_PROGRAM,  // Program.
    ATTR_TYPE_S_EXPR,   // S-expression.
    ATTR_TYPE_SLOT_DEF, // Slot definition
    ATTR_TYPE_LIST      // List.
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

    SemanticConstant();
};

/**
 * @brief Represents a program.
 */
class SemanticProgram
{
public:
    QMap<QString, SemanticClass *> fClassTable; // Class table.

    SemanticProgram();
};

/**
 * @brief Represents a class.
 */
class SemanticClass
{
public:
    SemanticClass();
};

/**
 * @brief Represents a class field.
 */
class SemanticField
{
public:
    SemanticField();
};

/**
 * @brief Represents a class method.
 */
class SemanticMethod
{
public:
    SemanticMethod();
};

/**
 * @brief Represents a local variable.
 */
class SemanticLocalVariable
{
public:
    SemanticLocalVariable();
};

/**
 * @brief Represents the semantic analyzer that encapsulates all stuff.
 */
class SemanticAnalyzer
{
public:
    SemanticAnalyzer();
    SemanticAnalyzer(const program_struct * root);
    ~SemanticAnalyzer();
    ProgramNode * getRoot() const;
    QLinkedList<QString> getErrors() const;
    bool doSemantics();
    void doTransform();
private:
    ProgramNode * root;
    QLinkedList<QString> errors;
};

/**
 * @brief Represents an attributed node.
 */
class AttributedNode
{
public:
    int             fNodeId; // Id of the node.
    AttributedTypes fType;   // Type of the node.

    /**
     * @brief Default constructor.
     */
    AttributedNode();

    /**
     * @brief Destructor should be implemented in child classes for memory deallocation.
     */
    ~AttributedNode();

    /**
     * @brief Generates dot code for this node.
     */
    virtual QString dotCode(QString parent, QString label) const = 0;

    /**
     * @brief Checks if this s-expression is calculable, e.g. can be used as a condition, initform etc.
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
    virtual void semantics(QLinkedList<QString> * errorList) const = 0;
};

/**
 * @brief Represents an attributed program node.
 */
class ProgramNode : public AttributedNode
{
public:
    QLinkedList<SExpressionNode *> fExpressions;
    ProgramNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();

    /**
     * @brief Creates an instance of ProgramNode from a program_struct node.
     */
    static ProgramNode * fromSyntaxNode(const program_struct * syntaxNode);
};

/**
 * @brief Represents an attributed s-expression node.
 */
class SExpressionNode : public AttributedNode
{
public:
    s_expr_type fSubType;
    int         fInteger;
    char        fCharacter;
    QString     fString;
    int         fBoolean;
    QString     fId;
    ListNode  * fList;
    SExpressionNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();

    /**
     * @brief Creates an instance of SExpressionNode from a s_expr_struct node.
     */
    static SExpressionNode * fromSyntaxNode(const s_expr_struct * syntaxNode);
};

/**
 * @brief Represents an attributed slot definition node.
 */
class SlotDefinitionNode : public AttributedNode
{
public:
    slot_def_type     fSubType;
    SExpressionNode * fInitform;
    QString           fId;
    slot_alloc_type   fAllocType;
    SlotDefinitionNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();

    /**
     * @brief Creates an instance of SlotDefinitionNode from a slot_def_struct node.
     */
    static SlotDefinitionNode * fromSyntaxNode(const slot_def_struct * syntaxNode);
};

/**
 * @brief Represents an attributed list node.
 */
class ListNode : public AttributedNode
{
public:
    list_type                         fSubType;
    QString                           fId;
    QLinkedList<SExpressionNode *>    fOperands;
    SExpressionNode                 * fCondition;
    SExpressionNode                 * fContainer;
    SExpressionNode                 * fFrom;
    SExpressionNode                 * fTo;
    SExpressionNode                 * fBody1;
    SExpressionNode                 * fBody2;
    QLinkedList<SlotDefinitionNode *> fSlotDefs;
    QString                           fParent;
    ListNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    QLinkedList<AttributedNode *> childNodes() const;
    void transform();

    /**
     * @brief Creates an instance of ListNode from a list_struct node.
     */
    static ListNode * fromSyntaxNode(const list_struct * syntaxNode);
};

#endif // SEMANTICANALYZER_H

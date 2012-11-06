#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <QString>
#include <QLinkedList>
#include "nodetypes.h"
#include "parser_structs.h"

#define FUNC_NAME_SETF    "setf"
#define FUNC_NAME_VECTOR  "vector"
#define FUNC_NAME_ELT     "elt"
#define FUNC_NAME_LIST    "list"

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

enum AttributedTypes
{
    ATTR_TYPE_PROGRAM,  // Program.
    ATTR_TYPE_S_EXPR,   // S-expression.
    ATTR_TYPE_SLOT_DEF, // Slot definition
    ATTR_TYPE_LIST      // List.
};

class AttributedNode;
class ProgramNode;
class SExpressionNode;
class ListNode;

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
    ProgramNode * program;
    QLinkedList<QString> errors;
};

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
    virtual ~AttributedNode();

    /**
     * @brief Generates dot code for this node.
     */
    virtual QString dotCode(QString parent, QString label) const = 0;

    /**
     * @brief Checks if this s-expression is calculable, e.g. can be used as a condition, initform etc.
     */
    virtual bool isCalculable() const = 0;

    /**
     * @brief Transforms the subtree of this node.
     */
    virtual void transform() = 0;

    /**
     * @brief Does the semantic analysis of the node: checks, tables, etc.
     */
    virtual void semantics(QLinkedList<QString> * errorList) const = 0;
};

class ProgramNode : public AttributedNode
{
public:
    QLinkedList<SExpressionNode *> fExpressions;
    ProgramNode();
    ~ProgramNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    void transform();

    /**
     * @brief Creates an instance of ProgramNode from a program_struct node.
     */
    static ProgramNode * fromSyntaxNode(const program_struct * syntaxNode);
};

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
    ~SExpressionNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    void transform();

    /**
     * @brief Creates an instance of SExpressionNode from a s_expr_struct node.
     */
    static SExpressionNode * fromSyntaxNode(const s_expr_struct * syntaxNode);
};

class SlotDefinitionNode : public AttributedNode
{
public:
    slot_def_type     fSubType;
    SExpressionNode * fInitform;
    QString           fId;
    slot_alloc_type   fAllocType;
    SlotDefinitionNode();
    ~SlotDefinitionNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    void transform();

    /**
     * @brief Creates an instance of SlotDefinitionNode from a slot_def_struct node.
     */
    static SlotDefinitionNode * fromSyntaxNode(const slot_def_struct * syntaxNode);
};

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
    ~ListNode();
    QString dotCode(QString parent, QString label) const;
    void semantics(QLinkedList<QString> * errorList) const;
    bool isCalculable() const;
    void transform();

    /**
     * @brief Creates an instance of ListNode from a list_struct node.
     */
    static ListNode * fromSyntaxNode(const list_struct * syntaxNode);
};

#endif // SEMANTICANALYZER_H

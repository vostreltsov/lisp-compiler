#ifndef ATTRNODES_H
#define ATTRNODES_H

#include <QString>
#include <QLinkedList>
#include "nodetypes.h"
#include "parser_structs.h"

class AttributedNode;
class ProgramNode;
class SExpressionNode;
class ListNode;

enum AttributedTypes
{
    ATTR_TYPE_PROGRAM,  // Program.
    ATTR_TYPE_S_EXPR,   // S-expression.
    ATTR_TYPE_SLOT_DEF, // Slot definition
    ATTR_TYPE_LIST      // List.
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
     * @brief Checks the semantics of the node.
     */
    virtual void doCheck(QLinkedList<QString> * errorList) const = 0;
};

class ProgramNode : public AttributedNode
{
public:
    QLinkedList<SExpressionNode *> fExpressions;
    ProgramNode();
    ~ProgramNode();
    QString dotCode(QString parent, QString label) const;
    void doCheck(QLinkedList<QString> * errorList) const;

    /**
     * @brief Creates an instance of ProgramNode from a program_struct node.
     */
    static ProgramNode * fromSyntaxNode(program_struct * syntaxNode);
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
    void doCheck(QLinkedList<QString> * errorList) const;

    /**
     * @brief Creates an instance of SExpressionNode from a s_expr_struct node.
     */
    static SExpressionNode * fromSyntaxNode(s_expr_struct * syntaxNode);
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
    void doCheck(QLinkedList<QString> * errorList) const;

    /**
     * @brief Creates an instance of SlotDefinitionNode from a slot_def_struct node.
     */
    static SlotDefinitionNode * fromSyntaxNode(slot_def_struct * syntaxNode);
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
    void doCheck(QLinkedList<QString> * errorList) const;

    /**
     * @brief Creates an instance of ListNode from a list_struct node.
     */
    static ListNode * fromSyntaxNode(list_struct * syntaxNode);
};

#endif // ATTRNODES_H

#ifndef ATTRNODES_H
#define ATTRNODES_H

#include <QString>
#include <QLinkedList>
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

enum SlotDefTypes
{
    SLOT_DEF_TYPE_INITFORM, // :initform
    SLOT_DEF_TYPE_READER,   // :reader
    SLOT_DEF_TYPE_WRITER,   // :writer
    SLOT_DEF_TYPE_ACCESSOR, // :accessor
    SLOT_DEF_TYPE_ALLOC     // :allocation
};

class AttributedNode
{
public:
    int                  fNodeId;         // Id of the node.
    AttributedTypes      fType;           // Type of the node.
    QLinkedList<QString> fErrorMessages;  // Errors found in this node.

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
    virtual QString dotCode() const = 0;
};

class ProgramNode : public AttributedNode
{
public:
    QLinkedList<AttributedNode *> fExpressions;
    ProgramNode();
    ~ProgramNode();
    QString dotCode() const;
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
    QString dotCode() const;
    static SExpressionNode * fromSyntaxNode(s_expr_struct * syntaxNode);
};

class SlotDefinitionNode : public AttributedNode
{
public:
    SlotDefTypes fSubType;
    QString      fId;
    SlotDefinitionNode();
    ~SlotDefinitionNode();
    QString dotCode() const;
    static SlotDefinitionNode * fromSyntaxNode(slot_def_struct * syntaxNode);
};

class ListNode : public AttributedNode
{
public:
    ListNode();
    ~ListNode();
    QString dotCode() const;
    static ListNode * fromSyntaxNode(list_struct * syntaxNode);
};

#endif // ATTRNODES_H

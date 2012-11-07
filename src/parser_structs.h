#ifndef PARSER_STRUCTS_H
#define PARSER_STRUCTS_H

#include "nodetypes.h"

struct program_struct {
    int                        nodeId;        // Identifier of the node in the syntax tree.
    struct s_expr_seq_struct * s_expr_seq;    // Program is an s-expression sequence.
};

struct s_expr_struct {
    int                    nodeId;            // Identifier of the node in the syntax tree.
    enum s_expr_type       type;              // Type of the s-expression.
    int                    integer;           // S-expression may be an integer constant.
    char                   character;         // S-expression may be a character constant.
    char                 * string;            // S-expression may be a string constant.
    int                    boolean;           // S-expression may be an boolean constant.
    char                 * id;                // S-expression may be an identifier.
    struct list_struct   * list;              // S-expression may be a list.
    struct s_expr_struct * next;              // Pointer to the next s_expr_struct.
};

struct s_expr_seq_struct {
    int                        nodeId;        // Identifier of the node in the syntax tree.
    struct s_expr_struct     * first;         // First s-expression in the list.
    struct s_expr_struct     * last;          // Last s-expression in the list.
};

struct slot_def_struct {
    int                        nodeId;        // Identifier of the node in the syntax tree.
    enum slot_def_type         type;          // Type of the slot definition.
    struct s_expr_struct     * initform;      // Definition of an initial value.
    char                     * id;            // Definition of a reader, writer or accessor method.
    enum slot_alloc_type       alloc;         // Allocation type.
    struct slot_def_struct   * next;          // Pointer to the next slot_def_struct.
};

struct slot_def_seq_struct {
    int                        nodeId;        // Identifier of the node in the syntax tree.
    struct slot_def_struct   * first;         // First slot definition in the list.
    struct slot_def_struct   * last;          // Last slot definition in the list.
};

struct list_struct {
    int                            nodeId;    // Identifier of the node in the syntax tree.
    enum list_type                 type;      // Type of the list.
    char                         * id;        // Id of the function to call.
    struct s_expr_seq_struct     * ops;       // List of arguments in case of a function definition/call or slots in case of a class definition.
    struct s_expr_struct         * cond;      // Condition in case of "IF" or "LOOP WHILE".
    struct s_expr_struct         * container; // Container to iterate over in case of "LOOP".
    struct s_expr_struct         * from;      // "FROM" value in case of "LOOP".
    struct s_expr_struct         * to;        // "TO" value in case of "LOOP".
    struct s_expr_seq_struct     * body;      // List of s-expressions in case of "DEFUN".
    struct s_expr_struct         * body1;     // Positive branch in case of "IF" or a body in case of "LOOP".
    struct s_expr_struct         * body2;     // Negative branch in case of "IF".
    struct slot_def_seq_struct   * slotdefs;  // Slot definitions in case of a class definition.
    char                         * parent;    // Name of the parent class in case of a class definition.
};

#endif

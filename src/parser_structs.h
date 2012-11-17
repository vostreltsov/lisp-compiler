#ifndef PARSER_STRUCTS_H
#define PARSER_STRUCTS_H

#include "nodetypes.h"

struct program_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    struct program_part_seq_struct * parts;     // Program is an s-expression sequence.
};

struct program_part_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    enum program_part_type           type;      // Type of the program part.
    struct s_expr_struct           * s_expr;    // S-expression.
    struct def_struct              * def;       // Class or function definition.
    struct program_part_struct     * next;      // Pointer to the next program_part_struct.
};

struct program_part_seq_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    struct program_part_struct     * first;     // First s-expression in the list.
    struct program_part_struct     * last;      // Last s-expression in the list.
};

struct s_expr_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    enum s_expr_type                 type;      // Type of the s-expression.
    int                              integer;   // An integer constant.
    char                             character; // A character constant.
    char                           * string;    // A string constant.
    int                              boolean;   // A boolean constant.
    char                           * id;        // An identifier (variable, class, function).
    char                           * slvalobj;  // Object name in case of slot-value.
    char                           * slvalslot; // Slot name in case of slot-value.
    struct s_expr_seq_struct       * args;      // Arguments in case of "DEFUN".
    struct s_expr_struct           * cond;      // Condition in case of "IF" or "LOOP WHILE".
    struct s_expr_struct           * container; // Container to iterate over in case of "LOOP".
    struct s_expr_struct           * from;      // "FROM" value in case of "LOOP".
    struct s_expr_struct           * to;        // "TO" value in case of "LOOP".
    struct s_expr_struct           * body1;     // Positive branch in case of "IF" or a body in case of "LOOP".
    struct s_expr_struct           * body2;     // Negative branch in case of "IF".
    struct s_expr_struct           * next;      // Pointer to the next s_expr_struct.
};

struct s_expr_seq_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    struct s_expr_struct           * first;     // First s-expression in the list.
    struct s_expr_struct           * last;      // Last s-expression in the list.
};

struct slot_prop_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    enum slot_prop_type              type;      // Type of the slot property.
    struct s_expr_struct           * initform;  // Definition of an initial value.
    char                           * id;        // Name of the reader, writer or accessor method.
    enum slot_alloc_type             alloc;     // Allocation type.
    struct slot_prop_struct        * next;      // Pointer to the next slot_prop_struct.
};

struct slot_prop_seq_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    struct slot_prop_struct        * first;     // First slot property in the list.
    struct slot_prop_struct        * last;      // Last slot property in the list.
};

struct slot_def_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    char                           * id;        // Id of the slot.
    slot_prop_seq_struct           * props;     // Properties of the slot.
    struct slot_def_struct         * next;      // Pointer to the next slot_def_struct.
};

struct slot_def_seq_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    struct slot_def_struct         * first;     // First slot definition in the list.
    struct slot_def_struct         * last;      // Last slot definition in the list.
};

struct def_struct {
    int                              nodeId;    // Identifier of the node in the syntax tree.
    enum def_type                    type;      // Type of the definition.
    char                           * id;        // Name of the class or function.
    char                           * parent;    // Name of the parent class in case of "DEFCLASS".
    struct s_expr_seq_struct       * args;      // Arguments in case of "DEFUN".
    struct slot_def_seq_struct     * slotdefs;  // Slot definitions in case of "DEFCLASS".
    struct s_expr_seq_struct       * body;      // S-expressions in case of "DEFUN".
};

#endif

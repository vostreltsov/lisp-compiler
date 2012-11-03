#ifndef NODETYPES_H
#define NODETYPES_H

struct program_struct;
struct s_expr_struct;
struct s_expr_seq_struct;
struct slot_def_struct;
struct list_struct;

enum s_expr_type {
    S_EXPR_TYPE_INT,        // Scalar integer value.
    S_EXPR_TYPE_CHAR,       // Scalar character value.
    S_EXPR_TYPE_STRING,     // Scalar string value.
    S_EXPR_TYPE_BOOL,       // Scalar boolean value.
    S_EXPR_TYPE_ID,         // Scalar identifier.
    S_EXPR_TYPE_LIST        // List.
};

enum slot_def_type {
    SLOT_DEF_RESERVED,      // Default value.
    SLOT_DEF_INITFORM,      // :initform.
    SLOT_DEF_READER,        // :reader.
    SLOT_DEF_WRITER,        // :writer.
    SLOT_DEF_ACCESSOR,      // :accessor.
    SLOT_DEF_ALLOCATION     // :allocation.
};

enum slot_alloc_type {
    SLOT_ALLOC_INSTANCE,    // Slot-per-instance.
    SLOT_ALLOC_CLASS        // Slot-per-class.
};

enum list_type {
    LIST_TYPE_EMPTY,        // Reserved value.
    LIST_TYPE_FCALL,        // Function call.
    LIST_TYPE_LOOP_IN,      // Loop "loop for x in array do (print x)".
    LIST_TYPE_LOOP_FROM_TO, // Loop "loop for x from 1 to 10 do (print x)".
    LIST_TYPE_PROGN,        // Uniting a couple of expressions to a one expression.
    LIST_TYPE_IF,           // Conditional operator.
    LIST_TYPE_SLOTDEF,      // Slot definition.
    LIST_TYPE_DEFUN,        // Function definition.
    LIST_TYPE_DEFCLASS,     // Class definition.

    LIST_TYPE_ASSIGN_ELT    // Ternary operator []=
};

#endif // NODETYPES_H

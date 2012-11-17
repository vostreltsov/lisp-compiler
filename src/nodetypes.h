#ifndef NODETYPES_H
#define NODETYPES_H

struct program_struct;
struct program_part_struct;
struct program_part_seq_struct;
struct s_expr_struct;
struct s_expr_seq_struct;
struct slot_prop_struct;
struct slot_prop_seq_struct;
struct slot_def_struct;
struct slot_def_seq_struct;
struct def_struct;

enum program_part_type {
    PROGRAM_PART_TYPE_RESERVED, // Default invalid value.
    PROGRAM_PART_TYPE_S_EXPR,   // S-expression.
    PROGRAM_PART_TYPE_DEF       // Class or function definition.
};

enum s_expr_type {
    S_EXPR_TYPE_RESERVED,       // Default invalid value.
    S_EXPR_TYPE_INT,            // Integer value.
    S_EXPR_TYPE_CHAR,           // Character value.
    S_EXPR_TYPE_STRING,         // String value.
    S_EXPR_TYPE_BOOL,           // Boolean value.
    S_EXPR_TYPE_ID,             // Identifier.
    S_EXPR_TYPE_FCALL,          // Function call.
    S_EXPR_TYPE_LOOP_IN,        // Loop "loop for x in array do (print x)".
    S_EXPR_TYPE_LOOP_FROM_TO,   // Loop "loop for x from 1 to 10 do (print x)".
    S_EXPR_TYPE_PROGN,          // Uniting a couple of expressions to one expression.
    S_EXPR_TYPE_IF,             // Conditional operator.
    S_EXPR_TYPE_MAKEINSTANCE,   // Making an instance of a class.

    S_EXPR_TYPE_ASSIGN_ELT,     // Ternary operator array[index] = value.
    S_EXPR_TYPE_ASSIGN_FIELD    // Ternary operator object.field = value.
};

enum slot_prop_type {
    SLOT_PROP_TYPE_RESERVED,    // Default invalid value.
    SLOT_PROP_TYPE_INITFORM,    // :initform.
    SLOT_PROP_TYPE_READER,      // :reader.
    SLOT_PROP_TYPE_WRITER,      // :writer.
    SLOT_PROP_TYPE_ACCESSOR,    // :accessor.
    SLOT_PROP_TYPE_ALLOCATION   // :allocation.
};

enum slot_alloc_type {
    SLOT_ALLOC_TYPE_RESERVED,   // Default invalid value.
    SLOT_ALLOC_TYPE_INSTANCE,   // Slot-per-instance.
    SLOT_ALLOC_TYPE_CLASS       // Slot-per-class.
};

enum def_type {
    DEF_TYPE_RESERVED,          // Default invalid value.
    DEF_TYPE_CLASS,             // Class definition.
    DEF_TYPE_FUNC               // Function definition.
};

#endif // NODETYPES_H

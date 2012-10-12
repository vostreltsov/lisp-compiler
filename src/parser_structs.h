#ifndef PARSER_STRUCTS_H
#define PARSER_STRUCTS_H

struct program_struct;
struct s_expr_struct;
struct s_expr_seq_struct;
struct list_struct;

enum s_expr_type {
    S_EXPR_TYPE_INT,
    S_EXPR_TYPE_CHAR,
    S_EXPR_TYPE_STRING,
    S_EXPR_TYPE_BOOL,
    S_EXPR_TYPE_ID,
    S_EXPR_TYPE_LIST

};

enum list_type {
    LIST_TYPE_EMPTY,    // Reserved value.
    LIST_TYPE_FCALL,    // Function call.

    LIST_TYPE_LOOP_IN,
    LIST_TYPE_LOOP_FROM_TO,

    LIST_TYPE_SETF      // Assignment.


    // TODO: all the functions and keywords go here
};

struct program_struct {
    int                    nodeId;     // Identifier of the node in the syntax tree.
    struct s_expr_struct * s_expr;     // Program is an s-expression.
};

struct s_expr_struct {
    int                    nodeId;     // Identifier of the node in the syntax tree.
    enum s_expr_type       type;       // Type of the s-expression.
    long int               integer;    // S-expression may be an integer constant.
    char                   character;  // S-expression may be a character constant.
    char                 * string;     // S-expression may be a string constant.
    int                    boolean;    // S-expression may be an boolean constant.
    char                 * id;         // S-expression may be an identifier.
    struct list_struct   * list;       // S-expression may be a list.
    struct s_expr_struct * next;       // Pointer to the next s_expr_struct.
};

struct s_expr_seq_struct {
    int                        nodeId; // Identifier of the node in the syntax tree.
    struct s_expr_struct     * first;  // First s-expression in the list.
    struct s_expr_struct     * last;   // Last s-expression in the list.
};

struct list_struct {
    int                            nodeId; // Identifier of the node in the syntax tree.
    enum list_type                 type;   // Type of the list.
    char                         * id;     // Id of the function to call.
    struct s_expr_seq_struct     * ops;    // List of operands.
};

#endif

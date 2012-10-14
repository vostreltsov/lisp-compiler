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
    LIST_TYPE_EMPTY,        // Reserved value.
    LIST_TYPE_FCALL,        // Function call.
    LIST_TYPE_LOOP_IN,      // Loop "loop for x in array do (print x)".
    LIST_TYPE_LOOP_FROM_TO, // Loop "loop for x from 1 to 10 do (print x)".
    LIST_TYPE_PROGN,        // Uniting a couple of expressions to a one expression.
    LIST_TYPE_IF            // Conditional operator.
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
    int                            nodeId;     // Identifier of the node in the syntax tree.
    enum list_type                 type;       // Type of the list.
    char                         * id;         // Id of the function to call.
    struct s_expr_seq_struct     * ops;        // List of operands in case of a function call.
    struct s_expr_struct         * cond;       // Condition in case of "IF" or "LOOP WHILE".
    struct s_expr_struct         * container;  // Container to iterate over in case of "LOOP".
    struct s_expr_struct         * from;       // "FROM" value in case of "LOOP".
    struct s_expr_struct         * to;         // "TO" value in case of "LOOP".
    struct s_expr_struct         * body1;      // Positive branch in case of "IF" or a body in case of "LOOP".
    struct s_expr_struct         * body2;      // Negative branch in case of "IF".
};

#endif

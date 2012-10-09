#ifndef PARSER_STRUCTS_H
#define PARSER_STRUCTS_H

struct program_struct;
struct s_expr_struct;
struct s_expr_list_struct;
struct list_struct;


/*enum s_expr_type
{
    S_EXPR_TYPE_INT,      // Value: integer.
    S_EXPR_TYPE_CHAR,     // Value: character.
    S_EXPR_TYPE_STRING,   // Value: string.
    S_EXPR_TYPE_BOOL,     // Value: boolean.
    S_EXPR_TYPE_ID,       // Identifier.
};*/

enum list_type
{
    LIST_TYPE_PLUS,     // Binary plus.
    LIST_TYPE_MINUS,    // Binary minus.
    LIST_TYPE_MULT,     // Binary multiplication.
    LIST_TYPE_DIV,      // Binary division.
    LIST_TYPE_GRTR,     // Binary greater.
    LIST_TYPE_GRTR_EQ,  // Binary greater or equal.
    LIST_TYPE_LESS,     // Binary less.
    LIST_TYPE_LESS_EQ,  // Binary less or equal.
    LIST_TYPE_EQ,       // Binary equal.
    LIST_TYPE_AND,      // Binary and.
    LIST_TYPE_OR,       // Binary or.
    LIST_TYPE_NOT,      // Unary not.
    LIST_TYPE_SETF      // Assignment.

    // TODO: all the functions and keywords go here

};

struct program_struct
{
    struct s_expr_struct * s_expr; // Program is an s-expression.
};

struct s_expr_struct
{
    //enum s_expr_type       type;         // Type of the s-expression.
    long int               const_int;    // S-expression may be an integer constant.
    char                   const_char;   // S-expression may be a character constant.
    char                 * const_string; // S-expression may be a string constant.
    int                    const_bool;   // S-expression may be an boolean constant.
    char                 * const_id;     // S-expression may be an identifier.
    struct list_struct   * list;         // S-expression may be a list.

    struct s_expr_struct * next;         // Pointer to the next s_expr_struct.
};

struct s_expr_list_struct
{
    struct s_expr_struct * first; // First s-expression in the list.
    struct s_expr_struct * last;  // Last s-expression in the list.
};

struct list_struct
{
    struct s_expr_struct      * s_expr;      // List may be an (s-expression).
    struct s_expr_list_struct * s_expr_list; // List may be an (s-expression list).
};

#endif

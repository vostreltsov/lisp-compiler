#ifndef PARSER_STRUCTS
#define PARSER_STRUCTS

struct atom_struct;
struct s_expr_struct;
struct s_expr_list_struct;
struct list_struct;
struct program_struct;

enum atom_type
{
    ATOM_TYPE_INT,      // Value: integer.
    ATOM_TYPE_CHAR,     // Value: character.
    ATOM_TYPE_STRING,   // Value: string.
    ATOM_TYPE_BOOL,     // Value: boolean.

    ATOM_TYPE_PLUS,     // Binary plus.
    ATOM_TYPE_MINUS,    // Binary minus.
    ATOM_TYPE_MULT,     // Binary multiplication.
    ATOM_TYPE_DIV,      // Binary division.
    ATOM_TYPE_GRTR,     // Binary greater.
    ATOM_TYPE_GRTR_EQ,  // Binary greater or equal.
    ATOM_TYPE_LESS,     // Binary less.
    ATOM_TYPE_LESS_EQ,  // Binary less or equal.
    ATOM_TYPE_EQ,       // Binary equal.
    ATOM_TYPE_AND,      // Binary and.
    ATOM_TYPE_OR,       // Binary or.
    ATOM_TYPE_NOT,      // Unary not.

    ATOM_TYPE_ID,       // Identifier.
    ATOM_TYPE_SETF      // Assignment.

    // TODO: all the functions and keywords go here

};

struct atom_struct
{
    enum atom_type type;         // Type of the atom.
    long int       const_int;    // Integer constant.
    char           const_char;   // Character constant.
    char         * const_string; // String constant.
    int            const_bool;   // Boolean constant.
    char         * const_id;     // Identifier.
};

struct s_expr_struct
{
    struct atom_struct   * atom; // S-expression may be an atom.
    struct list_struct   * list; // S-expression may be a list.
    struct s_expr_struct * next; // Pointer to the next s_expr_struct.
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

struct program_struct
{
    struct s_expr_struct * s_expr; // Program is an s-expression.
};

#endif

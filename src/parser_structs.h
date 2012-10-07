#ifndef PARSER_STRUCTS
#define PARSER_STRUCTS

enum var_type
{
    VAR_TYPE_CONST,        // Constant.
    VAR_TYPE_LOCAL,        // Local variable.
    VAR_TYPE_GLOBAL        // Global variable.
};

enum expr_type
{
    EXPR_TYPE_INT,      // Value: integer.
    EXPR_TYPE_CHAR,     // Value: character.
    EXPR_TYPE_STRING,   // Value: string.
    EXPR_TYPE_BOOL,     // Value: boolean.
    EXPR_TYPE_T,        // Value: T (true).
    EXPR_TYPE_NIL,      // Value: NIL (false).

    EXPR_TYPE_PLUS,     // Binary plus.
    EXPR_TYPE_MINUS,    // Binary minus.
    EXPR_TYPE_MULT,     // Binary multiplication.
    EXPR_TYPE_DIV,      // Binary division.
    EXPR_TYPE_UPLUS,    // Unary plus.
    EXPR_TYPE_UMINUS,   // Unary minus.
    EXPR_TYPE_GRTR,     // Binary greater.
    EXPR_TYPE_GRTR_EQ,  // Binary greater or equal.
    EXPR_TYPE_LESS,     // Binary less.
    EXPR_TYPE_LESS_EQ,  // Binary less or equal.
    EXPR_TYPE_EQ,       // Binary equal.
    EXPR_TYPE_AND,      // Binary and.
    EXPR_TYPE_OR,       // Binary or.
    EXPR_TYPE_NOT       // Unary not.

};

enum stmt_type
{
    // TODO.
};

// Represents a variable.
struct variable_struct
{
    enum var_type    type;  // Type of the
    char           * name;
};

struct expression_struct
{
    long int const_int;         // Integer constant.
    char     const_char;        // Character constant.
    char   * const_string;      // String constant.
    int      const_bool;        // Boolean constant.

    enum expr_type      type;   // Type of the expression.
    expression_struct * left;   // Left operand.
    expression_struct * right;  // Right operand.
};

struct statement_struct
{
    enum stmt_type      type;   // Type of the statement.
};

struct expression_list
{
    expression_struct * expr;
    expression_list   * next;
};

struct statement_list
{
    statement_struct * stmt;
    statement_list   * next;
};

// Represents a program.
struct program_struct
{
    statement_list * stmts;
};

#endif

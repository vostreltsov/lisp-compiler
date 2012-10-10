#include "dotcode.h"

void exec_dot(const char * dotFileName, const char * pngFileName) {
    char cmd[256];
    sprintf(cmd, "dot -Tpng -o%s %s", pngFileName, dotFileName);
    system(cmd);
}

void dot_for_program(FILE * file, struct program_struct * program) {
    char tmp[BUFFER_SIZE];
    sprintf(tmp, "\"id%d\\n program\"", program->nodeId);
    fprintf(file, "digraph {\n");
    fprintf(file, "%s;\n", tmp);
    dot_for_s_expr(file, tmp, program->s_expr);
    fprintf(file, "}");
}

void dot_for_s_expr(FILE * file, char * lastNode, struct s_expr_struct * expr) {
    char tmp[BUFFER_SIZE];

    switch (expr->type) {
        case S_EXPR_TYPE_INT:
            sprintf(tmp, "\"id%d\\ns_expr-int\\n%d\"", expr->nodeId, expr->integer);
            break;
        case S_EXPR_TYPE_CHAR:
            sprintf(tmp, "\"id%d\\ns_expr-char\\n%c\"", expr->nodeId, expr->character);
            break;
        case S_EXPR_TYPE_STRING:
            sprintf(tmp, "\"id%d\\ns_expr-string\\n%s\"", expr->nodeId, expr->string);
            break;
        case S_EXPR_TYPE_BOOL:
            sprintf(tmp, "\"id%d\\ns_expr-bool\\n%s\"", expr->nodeId, expr->boolean ? "TRUE" : "FALSE");
            break;
        case S_EXPR_TYPE_ID:
            sprintf(tmp, "\"id%d\\ns_expr-id\\n%s\"", expr->nodeId, expr->id);
            break;
        case S_EXPR_TYPE_LIST:
            sprintf(tmp, "\"id%d\\ns_expr-list\"", expr->nodeId);
            dot_for_list(file, tmp, expr->list);
            break;
        default:
            strcpy(tmp, "");
            break;
    }
    fprintf(file, "%s->%s;\n", lastNode, tmp);
}

void dot_for_s_expr_seq(FILE * file, char * lastNode, struct s_expr_seq_struct * expr_seq) {
    char tmp[BUFFER_SIZE];
    sprintf(tmp, "\"id%d\\ns_expr_seq\"", expr_seq->nodeId);
    fprintf(file, "%s->%s;\n", lastNode, tmp);
    struct s_expr_struct * expr = expr_seq->first;
    while (expr != NULL) {
        dot_for_s_expr(file, tmp, expr);
        expr = expr->next;
    }
}

void dot_for_list(FILE * file, char * lastNode, struct list_struct * list) {
    char tmp[BUFFER_SIZE];
    if (list->id == NULL) {
        sprintf(tmp, "\"id%d\\nlist\"", list->nodeId);
    } else {
        sprintf(tmp, "\"id%d\\nlist\\n%s\"", list->nodeId, list->id);
    }
    fprintf(file, "%s->%s;\n", lastNode, tmp);
    if (list->s_expr != NULL) {
        dot_for_s_expr(file, tmp, list->s_expr);
    } else if (list->s_expr_seq != NULL) {
        dot_for_s_expr_seq(file, tmp, list->s_expr_seq);
    } else {
        // Empty list, do nothing.
    }
}

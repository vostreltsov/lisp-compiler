#include "dotcode.h"

void exec_dot(const char * dotFileName, const char * pngFileName) {
    char cmd[256] = "";
    sprintf(cmd, "dot -Tpng -o%s %s", pngFileName, dotFileName);
    system(cmd);
}

void dot_for_program(FILE * file, struct program_struct * program) {
    char tmp[BUFFER_SIZE] = "";
    sprintf(tmp, "\"id%d\\n program\"", program->nodeId);
    fprintf(file, "digraph {\n");
    fprintf(file, "%s;\n", tmp);
    dot_for_s_expr(file, tmp, "", program->s_expr);
    fprintf(file, "}");
}

void dot_for_s_expr(FILE * file, char * lastNode, char * label, struct s_expr_struct * expr) {
    char tmp[BUFFER_SIZE] = "";

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
            dot_for_list(file, tmp, "", expr->list);
            break;
        default:
            strcpy(tmp, "");
            break;
    }
    fprintf(file, "%s->%s[label=\"%s\"];\n", lastNode, tmp, label);
}

void dot_for_s_expr_seq(FILE * file, char * lastNode, char * label, struct s_expr_seq_struct * expr_seq) {
    char tmp[BUFFER_SIZE] = "";
    sprintf(tmp, "\"id%d\\ns_expr_seq\"", expr_seq->nodeId);
    fprintf(file, "%s->%s[label=\"%s\"];\n", lastNode, tmp, label);
    struct s_expr_struct * expr = expr_seq->first;
    while (expr != NULL) {
        dot_for_s_expr(file, tmp, "", expr);
        expr = expr->next;
    }
}

void dot_for_list(FILE * file, char * lastNode, char * label, struct list_struct * list) {
    char tmp[BUFFER_SIZE] = "";
    switch (list->type) {
    case LIST_TYPE_FCALL:
        sprintf(tmp, "\"id%d\\ncall %s\"", list->nodeId, list->id != NULL ? list->id : "");
        dot_for_s_expr_seq(file, tmp, "", list->ops);
        break;
    case LIST_TYPE_LOOP_IN:
        sprintf(tmp, "\"id%d\\nloop %s in\"", list->nodeId, list->id);
        dot_for_s_expr(file, tmp, "container", list->container);
        dot_for_s_expr(file, tmp, "body", list->body1);
        break;
    case LIST_TYPE_LOOP_FROM_TO:
        sprintf(tmp, "\"id%d\\nloop %s in\"", list->nodeId, list->id);
        dot_for_s_expr(file, tmp, "from", list->from);
        dot_for_s_expr(file, tmp, "to", list->to);
        dot_for_s_expr(file, tmp, "body", list->body1);
        break;
    case LIST_TYPE_PROGN:
        sprintf(tmp, "\"id%d\\nprogn\"", list->nodeId);
        dot_for_s_expr_seq(file, tmp, "", list->ops);
        break;
    case LIST_TYPE_IF:
        sprintf(tmp, "\"id%d\\nif\"", list->nodeId);
        dot_for_s_expr(file, tmp, "cond", list->cond);
        dot_for_s_expr(file, tmp, "pos", list->body1);
        if (list->body2 != NULL) {
            dot_for_s_expr(file, tmp, "neg", list->body2);
        }
        break;
    default:
        break;
    }
    fprintf(file, "%s->%s[label=\"%s\"];\n", lastNode, tmp, label);
}
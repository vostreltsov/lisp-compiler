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
            sprintf(tmp, "\"id%d\\n s_expr %d\"", expr->nodeId, expr->integer);
            fprintf(file, "%s->%s;\n", lastNode, tmp);
            break;
        case S_EXPR_TYPE_CHAR:
            sprintf(tmp, "\"id%d\\n s_expr %c\"", expr->nodeId, expr->character);
            fprintf(file, "%s->%s;\n", lastNode, tmp);
            break;
        case S_EXPR_TYPE_STRING:
            sprintf(tmp, "\"id%d\\n s_expr %s\"", expr->nodeId, expr->string);
            fprintf(file, "%s->%s;\n", lastNode, tmp);
            break;
        case S_EXPR_TYPE_BOOL:
            sprintf(tmp, "\"id%d\\n s_expr %s\"", expr->nodeId, expr->boolean ? "TRUE" : "FALSE");
            fprintf(file, "%s->%s;\n", lastNode, tmp);
            break;
        case S_EXPR_TYPE_ID:
            sprintf(tmp, "\"id%d\\n s_expr %s\"", expr->nodeId, expr->identifier);
            fprintf(file, "%s->%s;\n", lastNode, tmp);
            break;
        case S_EXPR_TYPE_LIST:
            sprintf(tmp, "\"id%d\\n s_expr\"", expr->nodeId);
            fprintf(file, "%s->%s;\n", lastNode, tmp);
            dot_for_list(file, tmp, expr->list);
            break;
        default:
            break;
    }
}

void dot_for_list(FILE * file, char * lastNode, struct list_struct * list) {

}

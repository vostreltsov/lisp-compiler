#include "syntaxdotcode.h"

void SyntaxDotCode::dot_for_program(QTextStream & out, struct program_struct * program) {
    QString tmp = "\"id" + QString::number(program->nodeId) + "\\n program\"";
    out << "digraph {\n";
    out << tmp <<";\n";
    dot_for_s_expr_seq(out, tmp, "", program->s_expr_seq);
    out << "}";
}

void SyntaxDotCode::dot_for_s_expr(QTextStream & out, const QString & lastNode, const QString & label, struct s_expr_struct * expr) {
    QString tmp;
    switch (expr->type) {
        case S_EXPR_TYPE_INT:
            tmp = "\"id" + QString::number(expr->nodeId) + "\\ns_expr:int\\n" + QString::number(expr->integer) + "\"";
            break;
        case S_EXPR_TYPE_CHAR:
            tmp = "\"id" + QString::number(expr->nodeId) + "\\ns_expr:char\\n" + QChar(expr->character) + "\"";
            break;
        case S_EXPR_TYPE_STRING:
            tmp = "\"id" + QString::number(expr->nodeId) + "\\ns_expr:string\\n" + expr->string + "\"";
            break;
        case S_EXPR_TYPE_BOOL:
            tmp = "\"id" + QString::number(expr->nodeId) + "\\ns_expr:bool\\n" + (expr->boolean ? "TRUE" : "FALSE") + "\"";
            break;
        case S_EXPR_TYPE_ID:
            tmp = "\"id" + QString::number(expr->nodeId) + "\\ns_expr:id\\n" + expr->id + "\"";
            break;
        case S_EXPR_TYPE_LIST:
            tmp = "\"id" + QString::number(expr->nodeId) + "\\ns_expr:list\"";
            dot_for_list(out, tmp, "", expr->list);
            break;
        default:
            break;
    }
    out << lastNode << "->" << tmp << "[label=\"" << label << "\"];\n";
}

void SyntaxDotCode::dot_for_s_expr_seq(QTextStream & out, const QString & lastNode, const QString & label, struct s_expr_seq_struct * expr_seq) {
    QString tmp = "\"id" + QString::number(expr_seq->nodeId) + "\\ns_expr_seq\"";
    out << lastNode << "->" << tmp << "[label=\"" << label << "\"];\n";
    struct s_expr_struct * expr = expr_seq->first;
    while (expr != NULL) {
        dot_for_s_expr(out, tmp, "", expr);
        expr = expr->next;
    }
}

void SyntaxDotCode::dot_for_slot_def(QTextStream & out, const QString & lastNode, const QString & label, struct slot_def_struct * slot_def) {
    QString tmp;
    if (slot_def->type == SLOT_DEF_INITFORM) {
        tmp = "\"id" + QString::number(slot_def->nodeId) + "\\n:initform\"";
        dot_for_s_expr(out, tmp, "", slot_def->initform);
    } else if (slot_def->type == SLOT_DEF_READER) {
        tmp = "\"id" + QString::number(slot_def->nodeId) + "\\n:reader " + slot_def->id + "\"";
    } else if (slot_def->type == SLOT_DEF_WRITER) {
        tmp = "\"id" + QString::number(slot_def->nodeId) + "\\n:writer " + slot_def->id + "\"";
    } else if (slot_def->type == SLOT_DEF_ACCESSOR) {
        tmp = "\"id" + QString::number(slot_def->nodeId) + "\\n:accessor " + slot_def->id + "\"";
    } else if (slot_def->type == SLOT_DEF_ALLOCATION && slot_def->alloc == SLOT_ALLOC_INSTANCE) {
        tmp = "\"id" + QString::number(slot_def->nodeId) + "\\n:alloc instance\"";
    } else if (slot_def->type == SLOT_DEF_ALLOCATION && slot_def->alloc == SLOT_ALLOC_CLASS) {
        tmp = "\"id" + QString::number(slot_def->nodeId) + "\\n:alloc class\"";
    }
    out << lastNode << "->" << tmp << "[label=\"" << label << "\"];\n";
}

void SyntaxDotCode::dot_for_slot_def_seq(QTextStream & out, const QString & lastNode, const QString & label, struct slot_def_seq_struct * slot_def_seq) {
    QString tmp = "\"id" + QString::number(slot_def_seq->nodeId) + "\\nslot_def_seq\"";
    out << lastNode << "->" << tmp << "[label=\"" << label << "\"];\n";
    struct slot_def_struct * slot_def = slot_def_seq->first;
    while (slot_def != NULL) {
        dot_for_slot_def(out, tmp, "", slot_def);
        slot_def = slot_def->next;
    }
}

void SyntaxDotCode::dot_for_list(QTextStream & out, const QString & lastNode, const QString & label, struct list_struct * list) {
    QString tmp;
    switch (list->type) {
    case LIST_TYPE_FCALL:
        tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:call " + list->id + "\"";
        dot_for_s_expr_seq(out, tmp, "", list->ops);
        break;
    case LIST_TYPE_LOOP_IN:
        tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:loop " + list->id + "\"";
        dot_for_s_expr(out, tmp, "container", list->container);
        dot_for_s_expr(out, tmp, "body", list->body1);
        break;
    case LIST_TYPE_LOOP_FROM_TO:
        tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:loop " + list->id + "\"";
        dot_for_s_expr(out, tmp, "from", list->from);
        dot_for_s_expr(out, tmp, "to", list->to);
        dot_for_s_expr(out, tmp, "body", list->body1);
        break;
    case LIST_TYPE_PROGN:
        tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:progn\"";
        dot_for_s_expr_seq(out, tmp, "", list->ops);
        break;
    case LIST_TYPE_IF:
        tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:if\"";
        dot_for_s_expr(out, tmp, "cond", list->cond);
        dot_for_s_expr(out, tmp, "pos", list->body1);
        if (list->body2 != NULL) {
            dot_for_s_expr(out, tmp, "neg", list->body2);
        }
        break;
    case LIST_TYPE_SLOTDEF:
        tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:slot " + list->id + "\"";
        dot_for_slot_def_seq(out, tmp, "", list->slotdefs);
        break;
    case LIST_TYPE_DEFUN:
        tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:defun " + list->id + "\"";
        if (list->ops != NULL) {
            dot_for_s_expr_seq(out, tmp, "args", list->ops);
        }
        dot_for_s_expr(out, tmp, "body", list->body1);
        break;
    case LIST_TYPE_DEFCLASS:
        if (list->parent == NULL) {
            tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:defclass " + list->id + "\"";
        } else {
            tmp = "\"id" + QString::number(list->nodeId) + "\\nlist:defclass " + list->id + "\\nparent:" + list->parent + "\"";
        }
        dot_for_s_expr_seq(out, tmp, "slots", list->ops);
        break;
    default:
        break;
    }
    out << lastNode << "->" << tmp << "[label=\"" << label << "\"];\n";
}

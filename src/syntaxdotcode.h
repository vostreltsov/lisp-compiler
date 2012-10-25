#ifndef DOTCODE_H
#define DOTCODE_H

#include <QProcess>
#include <QTextStream>
#include "parser_structs.h"

class SyntaxDotCode
{
public:
    static void dot_for_program(QTextStream & out, struct program_struct * program);
    static void dot_for_s_expr(QTextStream & out, const QString & lastNode, const QString & label, struct s_expr_struct * expr);
    static void dot_for_s_expr_seq(QTextStream & out, const QString & lastNode, const QString & label, struct s_expr_seq_struct * expr_list);
    static void dot_for_slot_def(QTextStream & out, const QString & lastNode, const QString & label, struct slot_def_struct * slot_def);
    static void dot_for_slot_def_seq(QTextStream & out, const QString & lastNode, const QString & label, struct slot_def_seq_struct * slot_def_seq);
    static void dot_for_list(QTextStream & out, const QString & lastNode, const QString & label, struct list_struct * list);
};

#endif

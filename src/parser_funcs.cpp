#include "parser_funcs.h"

struct program_struct * create_program_empty() {
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->nodeId = 0;
    result->parts = NULL;
    return result;
}

struct program_part_struct * create_program_part_empty() {
    struct program_part_struct * result = (struct program_part_struct *)malloc(sizeof(struct program_part_struct));
    result->nodeId = 0;
    result->type = PROGRAM_PART_TYPE_RESERVED;
    result->s_expr = NULL;
    result->def = NULL;
    result->next = NULL;
    return result;
}

struct program_part_seq_struct * create_program_part_seq_empty() {
    struct program_part_seq_struct * result = (struct program_part_seq_struct *)malloc(sizeof(struct program_part_seq_struct));
    result->nodeId = 0;
    result->first = NULL;
    result->last = NULL;
    return result;
}

struct s_expr_struct * create_s_expr_empty() {
    struct s_expr_struct * result = (struct s_expr_struct *)malloc(sizeof(struct s_expr_struct));
    result->nodeId = 0;
    result->type = S_EXPR_TYPE_RESERVED;
    result->integer = 0;
    result->character = '\0';
    result->string = NULL;
    result->boolean = 0;
    result->id = NULL;
    result->slvalobj = NULL;
    result->slvalslot = NULL;
    result->args = NULL;
    result->cond = NULL;
    result->container = NULL;
    result->from = NULL;
    result->to = NULL;
    result->body1 = NULL;
    result->body2 = NULL;
    result->next = NULL;
    return result;
}

struct s_expr_seq_struct * create_s_expr_seq_empty() {
    struct s_expr_seq_struct * result = (struct s_expr_seq_struct *)malloc(sizeof(struct s_expr_seq_struct));
    result->nodeId = 0;
    result->first = NULL;
    result->last = NULL;
    return result;
}

struct slot_prop_struct * create_slot_prop_empty() {
    struct slot_prop_struct * result = (struct slot_prop_struct *)malloc(sizeof(struct slot_prop_struct));
    result->nodeId = 0;
    result->type = SLOT_PROP_TYPE_RESERVED;
    result->initform = NULL;
    result->id = NULL;
    result->alloc = SLOT_ALLOC_TYPE_RESERVED;
    result->next = NULL;
    return result;
}

struct slot_prop_seq_struct * create_slot_prop_seq_empty() {
    struct slot_prop_seq_struct * result = (struct slot_prop_seq_struct *)malloc(sizeof(struct slot_prop_seq_struct));
    result->nodeId = 0;
    result->first = NULL;
    result->last = NULL;
    return result;
}

struct slot_def_struct * create_slot_def_empty() {
    struct slot_def_struct * result = (struct slot_def_struct *)malloc(sizeof(struct slot_def_struct));
    result->nodeId = 0;
    result->id = NULL;
    result->props = NULL;
    result->next = NULL;
    return result;
}

struct slot_def_seq_struct * create_slot_def_seq_empty() {
    struct slot_def_seq_struct * result = (struct slot_def_seq_struct *)malloc(sizeof(struct slot_def_seq_struct));
    result->nodeId = 0;
    result->first = NULL;
    result->last = NULL;
    return result;
}

struct def_struct * create_def_empty() {
    struct def_struct * result = (struct def_struct *)malloc(sizeof(struct def_struct));
    result->nodeId = 0;
    result->type = DEF_TYPE_RESERVED;
    result->id = NULL;
    result->parent = NULL;
    result->args = NULL;
    result->slotdefs = NULL;
    result->body = NULL;
    return result;
}

struct program_struct * create_program(int nodeId,
                                       struct program_part_seq_struct * parts) {
    struct program_struct * result = create_program_empty();
    result->nodeId = nodeId;
    result->parts = parts;
    return result;
}

struct program_part_struct * create_program_part(int nodeId,
                                                 enum program_part_type type,
                                                 struct s_expr_struct * s_expr,
                                                 struct def_struct * def) {
    struct program_part_struct * result = create_program_part_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->s_expr = s_expr;
    result->def = def;
    return result;
}

struct program_part_seq_struct * create_program_part_seq(int nodeId,
                                                         struct program_part_struct * first) {
    struct program_part_seq_struct * result = create_program_part_seq_empty();
    result->nodeId = nodeId;
    result->first = first;
    result->last = first;
    return result;
}

struct program_part_seq_struct * add_to_program_part_seq(struct program_part_seq_struct * seq,
                                                         struct program_part_struct * part) {
    seq->last->next = part;
    seq->last = part;
    return seq;
}

struct s_expr_struct * create_s_expr_simple(int nodeId,
                                            enum s_expr_type type,
                                            int integer,
                                            char character,
                                            char * string,
                                            int boolean,
                                            char * id) {
    struct s_expr_struct * result = create_s_expr_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->integer = integer;
    result->character = character;
    result->string = string;
    result->boolean = boolean;
    result->id = id;
    return result;
}

struct s_expr_struct * create_s_expr_funcall(int nodeId,
                                             enum s_expr_type type,
                                             char * id,
                                             struct s_expr_seq_struct * args) {
    struct s_expr_struct * result = create_s_expr_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->id = id;
    result->args = args;
    return result;
}

struct s_expr_struct * create_s_expr_loop(int nodeId,
                                          enum s_expr_type type,
                                          char * id,
                                          struct s_expr_seq_struct * args,
                                          struct s_expr_struct * container,
                                          struct s_expr_struct * from,
                                          struct s_expr_struct * to) {
    struct s_expr_struct * result = create_s_expr_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->id = id;
    result->args = args;
    result->container = container;
    result->from = from;
    result->to = to;
    return result;
}

struct s_expr_struct * create_s_expr_progn(int nodeId,
                                           enum s_expr_type type,
                                           struct s_expr_seq_struct * args) {
    struct s_expr_struct * result = create_s_expr_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->args = args;
    return result;
}

struct s_expr_struct * create_s_expr_if(int nodeId,
                                        enum s_expr_type type,
                                        struct s_expr_struct * cond,
                                        struct s_expr_struct * body1,
                                        struct s_expr_struct * body2) {
    struct s_expr_struct * result = create_s_expr_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->cond = cond;
    result->body1 = body1;
    result->body2 = body2;
    return result;
}

struct s_expr_struct * create_s_expr_makeinstance(int nodeId,
                                                  enum s_expr_type type,
                                                  char * id) {
    struct s_expr_struct * result = create_s_expr_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->id = id;
    return result;
}

struct s_expr_struct * create_s_expr_slotvalue(int nodeId,
                                               enum s_expr_type type,
                                               char * slvalobj,
                                               char * slvalslot) {
    struct s_expr_struct * result = create_s_expr_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->slvalobj = slvalobj;
    result->slvalslot = slvalslot;
    return result;
}

struct s_expr_seq_struct * create_s_expr_seq(int nodeId,
                                             struct s_expr_struct * first) {
    struct s_expr_seq_struct * result = create_s_expr_seq_empty();
    result->nodeId = nodeId;
    result->first = first;
    result->last = first;
    return result;
}

struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * seq,
                                             struct s_expr_struct * expr){
    seq->last->next = expr;
    seq->last = expr;
    return seq;
}

struct slot_prop_struct * create_slot_prop(int nodeId,
                                           enum slot_prop_type type,
                                           struct s_expr_struct * initform,
                                           char * id,
                                           enum slot_alloc_type alloc) {
    struct slot_prop_struct * result = create_slot_prop_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->initform = initform;
    result->id = id;
    result->alloc = alloc;
    return result;
}

struct slot_prop_seq_struct * create_slot_prop_seq(int nodeId,
                                                   struct slot_prop_struct * first) {
    struct slot_prop_seq_struct * result = create_slot_prop_seq_empty();
    result->nodeId = nodeId;
    result->first = first;
    result->last = first;
    return result;
}

struct slot_prop_seq_struct * add_to_slot_prop_seq(struct slot_prop_seq_struct * seq,
                                                   struct slot_prop_struct * prop) {
    seq->last->next = prop;
    seq->last = prop;
    return seq;
}

struct slot_def_struct * create_slot_def(int nodeId,
                                         char * id,
                                         struct slot_prop_seq_struct * props) {
    struct slot_def_struct * result = create_slot_def_empty();
    result->nodeId = nodeId;
    result->id = id;
    result->props = props;
    return result;
}

struct slot_def_seq_struct * create_slot_def_seq(int nodeId,
                                                 struct slot_def_struct * first) {
    struct slot_def_seq_struct * result = create_slot_def_seq_empty();
    result->nodeId = nodeId;
    result->first = first;
    result->last = first;
    return result;
}

struct slot_def_seq_struct * add_to_slot_def_seq(struct slot_def_seq_struct * seq,
                                                 struct slot_def_struct * def) {
    seq->last->next = def;
    seq->last = def;
    return seq;
}

struct def_struct * create_def(int nodeId,
                               enum def_type type,
                               char * id,
                               char * parent,
                               struct s_expr_seq_struct * args,
                               struct slot_def_seq_struct * slotdefs,
                               struct s_expr_seq_struct * body) {

    struct def_struct * result = create_def_empty();
    result->nodeId = nodeId;
    result->type = type;
    result->id = id;
    result->parent = parent;
    result->args = args;
    result->slotdefs = slotdefs;
    result->body = body;
    return result;
}

void free_char(char * ch) {
    if (ch != NULL) {
        free(ch);
    }
}

void free_program(struct program_struct * program) {
    if (program != NULL) {
        free_program_part_seq(program->parts);
        free(program);
    }
}

void free_program_part(struct program_part_struct * program_part) {
    if (program_part != NULL) {
        free_s_expr(program_part->s_expr);
        free_def(program_part->def);
        free(program_part);
    }
}

void free_program_part_seq(struct program_part_seq_struct * program_part_seq) {
    if (program_part_seq != NULL) {
        struct program_part_struct * cur = program_part_seq->first;
        while (cur != NULL) {
            struct program_part_struct * tmp = cur;
            cur = cur->next;
            free_program_part(tmp);
        }
        free(program_part_seq);
    }
}

void free_s_expr(struct s_expr_struct * s_expr) {
    if (s_expr != NULL) {
        free_char(s_expr->string);
        free_char(s_expr->id);
        free_s_expr_seq(s_expr->args);
        free_s_expr(s_expr->cond);
        free_s_expr(s_expr->container);
        free_s_expr(s_expr->from);
        free_s_expr(s_expr->to);
        free_s_expr(s_expr->body1);
        free_s_expr(s_expr->body2);
        free(s_expr);
    }
}

void free_s_expr_seq(struct s_expr_seq_struct * s_expr_seq) {
    if (s_expr_seq != NULL) {
        struct s_expr_struct * cur = s_expr_seq->first;
        while (cur != NULL) {
            struct s_expr_struct * tmp = cur;
            cur = cur->next;
            free_s_expr(tmp);
        }
        free(s_expr_seq);
    }
}

void free_slot_prop(struct slot_prop_struct * slot_prop) {
    if (slot_prop != NULL) {
        free_s_expr(slot_prop->initform);
        free_char(slot_prop->id);
        free(slot_prop);
    }
}

void free_slot_prop_seq(struct slot_prop_seq_struct * slot_prop_seq) {
    if (slot_prop_seq != NULL) {
        struct slot_prop_struct * cur = slot_prop_seq->first;
        while (cur != NULL) {
            struct slot_prop_struct * tmp = cur;
            cur = cur->next;
            free_slot_prop(tmp);
        }
        free(slot_prop_seq);
    }
}

void free_slot_def(struct slot_def_struct * slot_def) {
    if (slot_def != NULL) {
        free_char(slot_def->id);
        free_slot_prop_seq(slot_def->props);
        free(slot_def);
    }
}

void free_slot_def_seq(struct slot_def_seq_struct * slot_def_seq) {
    if (slot_def_seq != NULL) {
        struct slot_def_struct * cur = slot_def_seq->first;
        while (cur != NULL) {
            struct slot_def_struct * tmp = cur;
            cur = cur->next;
            free_slot_def(tmp);
        }
        free(slot_def_seq);
    }
}

void free_def(struct def_struct * def) {
    if (def != NULL) {
        free_char(def->id);
        free_char(def->parent);
        free_s_expr_seq(def->args);
        free_slot_def_seq(def->slotdefs);
        free_s_expr_seq(def->body);
        free(def);
    }
}


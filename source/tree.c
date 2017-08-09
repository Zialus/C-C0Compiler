#include <stdlib.h>
#include <stdio.h>

#include "tree.h"
#include "utils.h"

// ----- Expression CONSTRUCTORS ----- //
EXP make_Int_EXP_(int i) {
    EXP p = malloc(sizeof(*p));
    p->kind = EXP_int;
    p->u.integer = i;
    return p;
}

EXP make_Bool_EXP_(bool b) {
    EXP p = malloc(sizeof(*p));
    p->kind = EXP_bool;
    p->u.boolean = b;
    return p;
}

EXP make_Var_EXP_(char* c) {
    EXP p = malloc(sizeof(*p));
    p->kind = EXP_Var;

    int n = snprintf(p->u.var, sizeof(p->u.var), "%s", c);
    check_if_buffer_was_big_enough(n, sizeof(p->u.var));

    return p;
}

EXP make_A_Op_EXP_(A_Operand op, EXP l, EXP r) {
    EXP p = malloc(sizeof(*p));
    p->kind = EXP_A_Op;
    p->u.opA.oper = op;
    p->u.opA.left = l;
    p->u.opA.right = r;
    return p;
}

EXP make_B_Op_EXP_(B_Operand op, EXP l, EXP r) {
    EXP p = malloc(sizeof(*p));
    p->kind = EXP_B_Op;
    p->u.opB.oper = op;
    p->u.opB.left = l;
    p->u.opB.right = r;
    return p;
}
// ----- Expression CONSTRUCTORS ----- //


// ----- Commands CONSTRUCTORS ----- //
CMD CMD_if_then_else(EXP if_exp, I_List then_, I_List else_) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = IF_KIND;
    cmd->u.if_else.if_ = if_exp;
    cmd->u.if_else.then_I_list_ = then_;
    cmd->u.if_else.else_I_list_ = else_;
    return cmd;
}

CMD CMD_while(EXP while_exp, I_List while_) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = WHILE_KIND;
    cmd->u.w.while_ = while_exp;
    cmd->u.w.while_I_list_ = while_;
    return cmd;
}

CMD CMD_assignment(char* v, EXP exp) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = ASSIGN_KIND;

    int n = snprintf(cmd->u.ass.var_, sizeof(cmd->u.ass.var_), "%s", v);
    check_if_buffer_was_big_enough(n, sizeof(cmd->u.ass.var_));

    cmd->u.ass.assignment_ = exp;
    return cmd;
}
// ----- Commands CONSTRUCTORS ----- //


// ----- Declarations CONSTRUCTORS ----- //
DECL DECL_declare(Type t, char* v) {
    DECL decl = malloc(sizeof(*decl));
    decl->type = t;

    int n = snprintf(decl->var, sizeof(decl->var), "%s", v);
    check_if_buffer_was_big_enough(n, sizeof(decl->var));

    return decl;
}
// ----- Declarations CONSTRUCTORS ----- //


// ----- Instruction List CONSTRUCTORS ----- //
I_List Head_CMD(CMD head, I_List tail) {
    I_List il = malloc(sizeof(*il));
    il->kind = CMD_;
    il->head.cmd = head;
    il->tail = tail;
    return il;
}

I_List Head_DECL(DECL head, I_List tail) {
    I_List il = malloc(sizeof(*il));
    il->kind = DECL_;
    il->head.decl = head;
    il->tail = tail;
    return il;
}
// ----- Instruction List CONSTRUCTORS ----- //

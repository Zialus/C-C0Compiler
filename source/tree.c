#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "utils.h"

// ----- Expression CONSTRUCTORS ----- //
EXP A_IntExp_(int i) {
    EXP p = malloc(sizeof(*p));
    p->kind = A_intExp;
    p->u.intt = i;
    return p;
}

EXP A_BoolExp_(Bool b) {
    EXP p = malloc(sizeof(*p));
    p->kind = A_boolExp;
    p->u.booll = b;
    return p;
}

EXP A_VarExp_(char* c) {
    EXP p = malloc(sizeof(*p));
    p->kind = A_varExp;

    int n = snprintf(p->u.var, sizeof(p->u.var), "%s", c);
    check_if_buffer_was_big_enough(n, sizeof(p->u.var));

    return p;
}

EXP A_AOpExp_(A_AOper oper, EXP l, EXP r) {
    EXP p = malloc(sizeof(*p));
    p->kind = A_AopExp;
    p->u.opA.oper = oper;
    p->u.opA.left = l;
    p->u.opA.right = r;
    return p;
}

EXP A_BOpExp_(A_BOper oper, EXP l, EXP r) {
    EXP p = malloc(sizeof(*p));
    p->kind = A_BopExp;
    p->u.opB.oper = oper;
    p->u.opB.left = l;
    p->u.opB.right = r;
    return p;
}
// ----- Expression CONSTRUCTORS ----- //


// ----- Commands CONSTRUCTORS ----- //
CMD CMD_if_then_else(EXP if_exp, I_list then_, I_list else_) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = IF_KIND;
    cmd->u.if_else.if_ = if_exp;
    cmd->u.if_else.then_I_list_ = then_;
    cmd->u.if_else.else_I_list_ = else_;
    return cmd;
}

CMD CMD_while(EXP while_exp, I_list while_) {
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
I_list Head_A_exp(EXP head, I_list tail) {
    I_list il = malloc(sizeof(*il));
    il->kind = A_EXP_;
    il->head.a_exp = head;
    il->tail = tail;
    return il;
}

I_list Head_CMD(CMD head, I_list tail) {
    I_list il = malloc(sizeof(*il));
    il->kind = CMD_;
    il->head.cmd = head;
    il->tail = tail;
    return il;
}

I_list Head_DECL(DECL head, I_list tail) {
    I_list il = malloc(sizeof(*il));
    il->kind = DECL_;
    il->head.decl = head;
    il->tail = tail;
    return il;
}
// ----- Instruction List CONSTRUCTORS ----- //

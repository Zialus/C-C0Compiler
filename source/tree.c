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
    memset(p, 0, sizeof(*p));
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
CMD CMD_if_then_else(EXP if_exp, I_List then_list, I_List else_list) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = IF_KIND;
    cmd->u.if_cmd.if_exp = if_exp;
    cmd->u.if_cmd.then_I_list = then_list;
    cmd->u.if_cmd.else_I_list = else_list;
    return cmd;
}

CMD CMD_while(EXP exp, I_List il) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = WHILE_KIND;
    cmd->u.while_cmd.while_exp = exp;
    cmd->u.while_cmd.while_I_list = il;
    return cmd;
}

CMD CMD_assignment(char* var, EXP exp) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = ASSIGN_KIND;

    int n = snprintf(cmd->u.assign_cmd.assignment_var, sizeof(cmd->u.assign_cmd.assignment_var), "%s", var);
    check_if_buffer_was_big_enough(n, sizeof(cmd->u.assign_cmd.assignment_var));

    cmd->u.assign_cmd.assignment_exp = exp;
    return cmd;
}

void delete_CMD(CMD cmd) {

    switch (cmd->kind) {
        case WHILE_KIND:
            delete_CMD_while(cmd);
            break;
        case IF_KIND:
            delete_CMD_if_then_else(cmd);
            break;
        case ASSIGN_KIND:
            delete_CMD_assignment(cmd);
            break;
    }

    free(cmd);
}

void delete_CMD_while(CMD cmd) {
    free(cmd->u.while_cmd.while_exp);
    free(cmd->u.while_cmd.while_I_list);
}

void delete_CMD_if_then_else(CMD cmd) {
    free(cmd->u.if_cmd.if_exp);
    free(cmd->u.if_cmd.else_I_list);
    free(cmd->u.if_cmd.then_I_list);
}

void delete_CMD_assignment(CMD cmd) {
    free(cmd->u.assign_cmd.assignment_exp);
}
// ----- Commands CONSTRUCTORS ----- //


// ----- Declarations CONSTRUCTORS ----- //
DECL DECL_declare(Type t, char* v) {
    DECL decl = malloc(sizeof(*decl));
    decl->var_type = t;

    int n = snprintf(decl->var_name, sizeof(decl->var_name), "%s", v);
    check_if_buffer_was_big_enough(n, sizeof(decl->var_name));

    return decl;
}

void delete_DECL(DECL decl) {
    free(decl);
}
// ----- Declarations CONSTRUCTORS ----- //


// ----- Instruction List CONSTRUCTORS ----- //
I_List make_List_CMD_Head(CMD head, I_List tail) {
    I_List il = malloc(sizeof(*il));
    il->kind = CMD_;
    il->head.cmd = head;
    il->tail = tail;
    return il;
}

I_List make_List_DECL_Head(DECL head, I_List tail) {
    I_List il = malloc(sizeof(*il));
    il->kind = DECL_;
    il->head.decl = head;
    il->tail = tail;
    return il;
}

void delete_IL(I_List il) {

    switch (il->kind) {
        case CMD_:
            delete_CMD(il->head.cmd);
            break;
        case DECL_:
            delete_DECL(il->head.decl);
            break;
    }

    if (il->tail != NULL) {
        delete_IL(il->tail);
    }
}
// ----- Instruction List CONSTRUCTORS ----- //

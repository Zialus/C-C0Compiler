#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*** CONSTRUTORES ***/

/*** EXPRESSOES ***/
A_exp A_IntExp_(int i) {
    A_exp p = malloc(sizeof(*p));
    p->kind = A_intExp;
    p->u.intt = i;
    return p;
}

A_exp A_BoolExp_(Bool b) {
    A_exp p = malloc(sizeof(*p));
    p->kind = A_boolExp;
    p->u.booll = b;
    return p;
}

A_exp A_VarExp_(char* c) {
    A_exp p = malloc(sizeof(*p));
    p->kind = A_varExp;
    strcpy(p->u.var, c);
    return p;
}

A_exp A_AOpExp_(A_AOper oper, A_exp l, A_exp r) {
    A_exp p = malloc(sizeof(*p));
    p->kind = A_AopExp;
    p->u.opA.oper = oper;
    p->u.opA.left = l;
    p->u.opA.right = r;
    return p;
}

A_exp A_BOpExp_(A_BOper oper, A_exp l, A_exp r) {
    A_exp p = malloc(sizeof(*p));
    p->kind = A_BopExp;
    p->u.opB.oper = oper;
    p->u.opB.left = l;
    p->u.opB.right = r;
    return p;
}

/*** COMANDOS ***/
CMD CMD_if_then_else(A_exp if_exp, I_list then_, I_list else_) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = IF_KIND;
    cmd->u.if_else.if_ = if_exp;
    cmd->u.if_else.then_I_list_ = then_;
    cmd->u.if_else.else_I_list_ = else_;
    return cmd;
}

CMD CMD_while(A_exp while_exp, I_list while_) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = WHILE_KIND;
    cmd->u.w.while_ = while_exp;
    cmd->u.w.while_I_list_ = while_;
    return cmd;
}

CMD CMD_assignment(char* v, A_exp exp) {
    CMD cmd = malloc(sizeof(*cmd));
    cmd->kind = ASSIGN_KIND;
    strcpy(cmd->u.ass.var_, v);
    cmd->u.ass.assignment_ = exp;
    return cmd;
}

/*** DECLARAÇOES ***/
DECL DECL_declare(Type t, char* v) {
    DECL decl = malloc(sizeof(*decl));
    decl->type = t;
    strcpy(decl->var, v);
    return decl;
}

/*** LISTA DE INSTRUÇOES ***/
I_list Head_A_exp(A_exp head, I_list tail) {
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

#include <stdio.h>

#include "tree.h"
#include "print_ast.h"


void print_tree(I_List il) {
    printf("{ ");
    if (il != NULL) {
        if (il->kind == CMD_ && il->head.cmd != NULL) {
            print_CMD(il->head.cmd);
        } else if (il->head.decl != NULL) {
            print_DECL(il->head.decl);
        }
        printf("}\n");
        if (il->tail != NULL) {
            print_tree(il->tail);
        }
    } else {
        printf("NULL } ");
    }
}


void print_EXP(EXP e) {
    printf("EXP( ");
    switch (e->kind) {
        case EXP_int:
            printf("Int(%d) ", e->u.integer);
            break;

        case EXP_bool:
            printf("Bool(");
            if (e->u.boolean) {
                printf("true) ");
            } else {
                printf("false) ");
            }
            break;

        case EXP_A_Op:
            print_EXP(e->u.opA.left);

            printf(" A_Operand(");
            switch (e->u.opA.oper) {
                case OpPlus:
                    printf("Plus) ");
                    break;
                case OpMinus:
                    printf("Minus) ");
                    break;
                case OpTimes:
                    printf("Times) ");
                    break;
                case OpDiv:
                    printf("Div) ");
                    break;
            }

            print_EXP(e->u.opA.right);
            break;

        case EXP_B_Op:
            print_EXP(e->u.opA.left);

            printf(" B_Operand(");
            switch (e->u.opB.oper) {
                case OpG:
                    printf("G) ");
                    break;
                case OpGEQ:
                    printf("GEQ) ");
                    break;
                case OpL:
                    printf("L) ");
                    break;
                case OpLEQ:
                    printf("LEQ) ");
                    break;
                case OpNOTEQ:
                    printf("NOTEQ) ");
                    break;
                case OpAND:
                    printf("AND) ");
                    break;
                case OpOR:
                    printf("OR) ");
                    break;
                case OpEQ:
                    printf("EQ) ");
                    break;
                case OpASSIGN:
                    printf("ASSIGN) ");
                    break;
            }

            print_EXP(e->u.opA.right);
            break;

        case EXP_Var:
            printf("Var( %s ) ", e->u.var);
    }
    printf(")");
}


void print_DECL(DECL d) {
    printf("Declaration(Type(");
    switch (d->var_type) {
        case BOOL_TYPE:
            printf("Bool) ");
            break;
        case INT_TYPE:
            printf("Int) ");
            break;
    }
    printf("Var(%s)) ", d->var_name);
}


void print_CMD(CMD c) {
    switch (c->kind) {
        case IF_KIND:
            printf("IF(");
            print_EXP(c->u.if_cmd.if_exp);
            printf(") ");
            printf("THEN");
            print_tree(c->u.if_cmd.then_I_list);
            printf("ELSE");
            print_tree(c->u.if_cmd.else_I_list);
            break;

        case WHILE_KIND:
            printf("WHILE(");
            print_EXP(c->u.while_cmd.while_exp);
            printf(") ");
            printf("WHILE_I_LIST");
            print_tree(c->u.while_cmd.while_I_list);
            break;

        case ASSIGN_KIND:
            printf("Assignment(Var(%s) ", c->u.assign_cmd.assignment_var);
            print_EXP(c->u.assign_cmd.assignment_exp);
            printf(") ");
    }
}

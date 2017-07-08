#include "tree.h"
#include "printer.h"

/*** IMPRIME A ÁRVORE COMPLETA ***/
void print_tree(I_list l) {
    if (l != NULL) {
        if (l->kind == A_EXP_ && l->head.a_exp != NULL) {
            print_A_exp(l->head.a_exp);
        }
        else if (l->kind == CMD_ && l->head.cmd != NULL) {
            print_CMD(l->head.cmd);
        }
        else if (l->head.decl != NULL) {
            print_DECL(l->head.decl);
        }
        if (l->tail != NULL) {
            print_tree(l->tail);
        }
    }
    else {
        printf("NULL");
    }
}

/*** IMPRIME EXPRESSÕES ***/
void print_A_exp(A_EXP exp) {
    printf("A_EXP( ");
    switch (exp->kind) {
        case A_intExp:
            printf("Int(%d) ", exp->u.intt);
            break;

        case A_boolExp:
            printf("Bool( ");
            if (exp->u.booll == BOOL_TRUE) {
                printf("true) ");
            }
            else {
                printf("false) ");
            }
            break;

        case A_AopExp:
            print_A_exp(exp->u.opA.left);
            printf("A_AOper( ");
            if (exp->u.opA.oper == OpPlus) {
                printf("Plus) ");
            }
            else if (exp->u.opA.oper == OpMinus) {
                printf("Minus) ");
            }
            else if (exp->u.opA.oper == OpTimes) {
                printf("Times) ");
            }
            else {
                printf("Div)");
            }
            print_A_exp(exp->u.opA.right);
            break;

        case A_BopExp:
            print_A_exp(exp->u.opA.left);
            printf("A_BOper( ");
            switch (exp->u.opB.oper) {
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
                default:
                    break;
            }
            print_A_exp(exp->u.opA.right);
            break;
        case A_varExp:
            printf("Var( %s ) ", exp->u.var);
    }
    printf(")");
}

/*** IMPRIME DECLARAÇÕES ***/
void print_DECL(DECL d) {
    printf("Declaracao(Type( ");
    if (d->type == BOOL_TYPE) {
        printf("bool) ");
    }
    else {
        printf("int) ");
    }
    printf("Var(%s)) ", d->var);
}

/*** IMPRIME COMANDOS ***/
void print_CMD(CMD c) {
    switch (c->kind) {
        case IF_KIND:
            printf("IF( ");
            print_A_exp(c->u.if_else.if_);
            printf(") ");
            printf("THEN( ");
            print_tree(c->u.if_else.then_I_list_);
            printf(") ");
            printf("ELSE( ");
            print_tree(c->u.if_else.else_I_list_);
            printf(") ");
            break;

        case WHILE_KIND:
            printf("WHILE( ");
            print_A_exp(c->u.w.while_);
            printf("WHILE_I_LIST( ");
            print_tree(c->u.w.while_I_list_);
            printf(") ");
            printf(")");
            break;

        default:
            printf("Assignment( Var(%s) ", c->u.ass.var_);
            print_A_exp(c->u.ass.assignment_);
            printf(") ");
    }
}

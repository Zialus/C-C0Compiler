#include "tree.h"
#include "print_ast.h"

/*** IMPRIME A ÁRVORE COMPLETA ***/
void print_tree(I_list l) {
    if (l != NULL) {
        if (l->kind == A_EXP_ && l->head.a_exp != NULL) {
            print_A_EXP(l->head.a_exp);
        } else if (l->kind == CMD_ && l->head.cmd != NULL) {
            print_CMD(l->head.cmd);
        } else if (l->head.decl != NULL) {
            print_DECL(l->head.decl);
        }
        if (l->tail != NULL) {
            print_tree(l->tail);
        }
    } else {
        printf("NULL");
    }
}

/*** IMPRIME EXPRESSÕES ***/
void print_A_EXP(EXP exp) {
    printf("EXP( ");
    switch (exp->kind) {
        case EXP_int:
            printf("Int(%d) ", exp->u.intt);
            break;

        case EXP_bool:
            printf("Bool( ");
            if (exp->u.booll) {
                printf("true) ");
            } else{
                printf("false) ");
            }
            break;

        case EXP_A_Op:
            print_A_EXP(exp->u.opA.left);

            printf("A_Operand( ");
            switch (exp->u.opA.oper) {
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

            print_A_EXP(exp->u.opA.right);
            break;

        case EXP_B_Op:
            print_A_EXP(exp->u.opA.left);

            printf("B_Operand( ");
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
                case OpEQ:
                    printf("EQ) ");
                    break;
                case OpASSIGN:
                    printf("ASSIGN) ");
                    break;
            }

            print_A_EXP(exp->u.opA.right);
            break;

        case EXP_Var:
            printf("Var( %s ) ", exp->u.var);
    }
    printf(")");
}

/*** IMPRIME DECLARAÇÕES ***/
void print_DECL(DECL d) {
    printf("Declaration(Type( ");
    switch (d->type) {
        case BOOL_TYPE:
            printf("Bool) ");
            break;
        case INT_TYPE:
            printf("Int) ");
            break;
    }
    printf("Var(%s) )", d->var);
}

/*** IMPRIME COMANDOS ***/
void print_CMD(CMD c) {
    switch (c->kind) {
        case IF_KIND:
            printf("IF( ");
            print_A_EXP(c->u.if_else.if_);
            printf(") ");
            printf("THEN( ");
            print_tree(c->u.if_else.then_I_list_);
            printf(") ");
            printf("ELSE( ");
            print_tree(c->u.if_else.else_I_list_);
            printf(")");
            break;

        case WHILE_KIND:
            printf("WHILE( ");
            print_A_EXP(c->u.w.while_);
            printf("WHILE_I_LIST( ");
            print_tree(c->u.w.while_I_list_);
            printf(") ");
            printf(")");
            break;

        case ASSIGN_KIND:
            printf("Assignment( Var(%s) ", c->u.ass.var_);
            print_A_EXP(c->u.ass.assignment_);
            printf(")");
    }
}

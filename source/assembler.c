#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash.h"
#include "print_mips.h"
#include "tree.h"
#include "assembler.h"
#include "utils.h"


// variaveis para crirar nomes de registos temporarios
int i = 0;
int lb = 0;
char final_reg[MAX_SIZE];

// tabela de símbolos
struct decl_hash* symbol_table = NULL;

Address makeVal(int n) {
    Address a = malloc(sizeof(*a));
    a->AddrKind = Int;
    a->content.val = n;
    return a;
}

Address makeVar(char* str) {
    Address a = malloc(sizeof(*a));
    a->AddrKind = String;
    a->content.var = str;
    return a;
}

Address makeReg(char* reg) {
    Address a = malloc(sizeof(*a));
    a->AddrKind = Register;
    a->content.var = reg;
    return a;
}

Address makeNewVar() {
    char v[MAX_SIZE_TO_REPRESENT_REG];
    char* var = malloc(sizeof(char) * (MAX_SIZE_TO_REPRESENT_REG + 3));

    var[0] = '$';
    var[1] = 't';
    var[2] = '\0';

    int chars_written = snprintf(v, MAX_SIZE_TO_REPRESENT_REG, "%d", i);
    //fprintf(stderr,"||%d -- %s||\n",chars_written,v);
    check_if_buffer_was_big_enough(chars_written, MAX_SIZE_TO_REPRESENT_REG);

    strncat(var, v, MAX_SIZE_TO_REPRESENT_REG);

    i++;
    Address a = makeReg(var);
    return a;
}

Address makeNewLabel() {
    char v[MAX_SIZE_TO_REPRESENT_LABEL];
    char* var = malloc(sizeof(char) * (MAX_SIZE_TO_REPRESENT_LABEL + 2));

    var[0] = 'l';
    var[1] = '\0';

    int chars_written = snprintf(v, MAX_SIZE_TO_REPRESENT_LABEL, "%d", lb);
    //fprintf(stderr,"||%d -- %s||\n",chars_written,v);
    check_if_buffer_was_big_enough(chars_written, MAX_SIZE_TO_REPRESENT_LABEL);

    strncat(var, v, MAX_SIZE_TO_REPRESENT_LABEL);

    lb++;
    Address a = makeVar(var);
    return a;
}

TAC makeTAC(OpKind o, Address a1, Address a2, Address a3) {
    TAC c = malloc(sizeof(*c));
    c->op = o;
    c->addr1 = a1;
    c->addr2 = a2;
    c->addr3 = a3;
    return c;
}

TACList makeTACList(TAC h, TACList t) {
    TACList c = malloc(sizeof(*c));
    c->head = h;
    c->tail = t;
    return c;
}

Pair makePair(Address a, TACList l) {
    Pair p = malloc(sizeof(*p));
    p->addr = a;
    p->clist = l;
    return p;
}

TACList append(TACList cl1, TACList cl2) {
    TACList cl3;
    if (cl1 != NULL) {
        cl3 = cl1;
        while (cl3->tail != NULL) {
            cl3 = cl3->tail;
        }
        cl3->tail = cl2;
    } else { // cl1 is NULL cl2 could be NULL or not, doesn't matter
        cl3 = cl2;
    }
    return cl3;
}

OpKind get_A_Op(A_Operand o) {
    switch (o) {
        case OpPlus:
            return A_Plus;
        case OpMinus:
            return A_Minus;
        case OpTimes:
            return A_Times;
        case OpDiv:
            return A_Div;
    }
}

OpKind get_B_Op(B_Operand o) {
    switch (o) {
        case OpEQ:
            return A_BEQ;
        case OpG:
            return A_BGT;
        case OpGEQ:
            return A_BGE;
        case OpL:
            return A_BLT;
        case OpLEQ:
            return A_BLE;
        case OpNOTEQ:
            return A_BNE;
        case OpAND:
            return A_AND;
        case OpOR:
            return A_OR;
        case OpASSIGN:
            return A_Asn;
    }
}

Pair compile_exp(EXP e) {
    // printf("EXP\n" );
    Pair auxA, auxB, p, res;
    TACList list = NULL;
    TACList list3;
    TACList tmp;
    Address t0, t1, t2;
    TAC elem, elem1, elem2;
    OpKind op;
    switch (e->kind) {
        case EXP_B_Op:
            t0 = makeNewVar();                   // var. resultado
            auxA = compile_exp(e->u.opB.left);   // ramo esq.
            auxB = compile_exp(e->u.opB.right);  // ramo dir.
            op = get_B_Op(e->u.opB.oper);

            switch (op) {
                case A_BLE:
                    t1 = makeNewVar();
                    t2 = makeNewVar();

                    elem1 = makeTAC(A_BLT, t1, auxA->addr, auxB->addr);
                    elem2 = makeTAC(A_BEQ, t2, auxA->addr, auxB->addr);
                    elem = makeTAC(A_OR, t0, t1, t2);

                    tmp = makeTACList(elem1, makeTACList(elem2, makeTACList(elem, NULL)));
                    list = append(list, auxB->clist);
                    list = append(list, tmp);
                    break;
                case A_BGE:
                    t1 = makeNewVar();
                    t2 = makeNewVar();

                    elem1 = makeTAC(A_BGT, t1, auxA->addr, auxB->addr);
                    elem2 = makeTAC(A_BEQ, t2, auxA->addr, auxB->addr);
                    elem = makeTAC(A_OR, t0, t1, t2);

                    tmp = makeTACList(elem1, makeTACList(elem2, makeTACList(elem, NULL)));
                    list = append(list, auxB->clist);
                    list = append(list, tmp);
                    break;
                default:
                    elem = makeTAC(op, t0, auxA->addr, auxB->addr);  // INSTRUÇÃO FINAL
                    // APPEND esq. ++ dir.
                    if (auxA->clist != NULL && auxB->clist != NULL) {
                        list3 = append(auxA->clist, auxB->clist);
                    } else if (auxA->clist != NULL) {
                        list3 = auxA->clist;
                    } else {
                        list3 = auxB->clist;  // pode ser NULL
                    }
                    tmp = makeTACList(elem, NULL);  // exp. final
                    if (list3 != NULL) {
                        list = append(list3, tmp);
                    } else {
                        list = tmp;
                    }
                    break;
            }

            res = makePair(t0, list);
            //última var. ()é usada no salto

            int n0 = snprintf(final_reg, sizeof(final_reg), "%s", res->addr->content.var);
            check_if_buffer_was_big_enough(n0, sizeof(final_reg));

            return res;
        case EXP_A_Op:
            t0 = makeNewVar();                   // var. resultado
            auxA = compile_exp(e->u.opA.left);   // ramo esq.
            auxB = compile_exp(e->u.opA.right);  // ramo dir.
            op = get_A_Op(e->u.opA.oper);
            elem = makeTAC(op, t0, auxA->addr, auxB->addr);  // INSTRUÇÃO FINAL
            // APPEND esq. ++ dir.
            if (auxA->clist != NULL && auxB->clist != NULL) {
                list3 = append(auxA->clist, auxB->clist);
            } else if (auxA->clist != NULL) {
                list3 = auxA->clist;
            } else {
                list3 = auxB->clist;  // pode ser NULL
            }
            tmp = makeTACList(elem, NULL);  // exp. final
            if (list3 != NULL) {
                list = append(list3, tmp);
            } else {
                list = tmp;
            }
            res = makePair(t0, list);
            return res;
        case EXP_int:
            p = makePair(makeVal(e->u.integer), NULL);

            char var[MAX_SIZE_INTS];
            int n1 = snprintf(var, sizeof(var), "%d", p->addr->content.val);
            check_if_buffer_was_big_enough(n1, sizeof(var));

            int n2 = snprintf(final_reg, sizeof(final_reg), "%s", var);
            check_if_buffer_was_big_enough(n2, sizeof(final_reg));

            return p;
        case EXP_Var:
            p = makePair(makeVar(e->u.var), NULL);

            int n3 = snprintf(final_reg, sizeof(final_reg), "%s", p->addr->content.var);
            check_if_buffer_was_big_enough(n3, sizeof(final_reg));

            return p;
        case EXP_bool: {
            p = makePair(makeVal(e->u.integer), NULL);
            int val = 0;
            if (e->u.boolean == true) {
                val = 1;
            }

            char v[2];
            int n4 = snprintf(v, sizeof(v), "%d", val);
            check_if_buffer_was_big_enough(n4, sizeof(v));

            int n5 = snprintf(final_reg, sizeof(final_reg), "%s", v);
            check_if_buffer_was_big_enough(n5, sizeof(final_reg));

            return p;
        }

    }
}


Pair compile(I_List il) {

    Pair tl = NULL;

    switch (il->kind) {
        case CMD_:
            tl = compile_cmd(il->head.cmd);
            break;
        case DECL_:
            compile_decl(il->head.decl);
            break;
    }

    if (il->tail != NULL) {
        Pair aux = compile(il->tail);
        if (tl != NULL && aux != NULL) {
            tl->clist = append(tl->clist, aux->clist);
            free(aux);
        } else if (aux != NULL) {
            tl = aux;
        }
    }

    return tl;
}

void compiler_start(I_List il) {
    if (freopen("out.asm", "w", stdout) == NULL) {
        perror("freopen() failed");
        exit(EXIT_FAILURE);
    }

    printf("\t\t.data\n");

    Pair p = compile(il);

    struct decl_hash* s;

    for (s = symbol_table; s != NULL; s = (struct decl_hash*) (s->hh.next)) {
        fprintf(stderr, "variable name %s: type %d\n", s->variable, s->type);
    }

    delete_hash();

    printf("\t\t.text\n");
    printf("main:\n");
    if (p->clist != NULL) {
        print_TACLIST(p->clist);
    }

    delete_IL(il);
    delete_Pair(p);
}

void delete_Pair(Pair p) {

    if (p->addr != NULL) delete_Address(p->addr);
    delete_TACLIST(p->clist);

    free(p);
}

void delete_Address(Address addr) {
    switch (addr->AddrKind){

        case Int:
            break;
        case String:
            free(addr->content.var);
            break;
        case Register:
            free(addr->content.var);
            break;
    }
}


void compile_decl(DECL decl) {
    add_to_hash(decl);

    printf("%s: \t.space 4\n", decl->var_name);
}

void add_to_hash(DECL decl) {
    struct decl_hash* s;

    HASH_FIND_STR(symbol_table, decl->var_name, s);  /* var already in the hash? */
    if (s == NULL) {

        s = malloc(sizeof(struct decl_hash));

        strncpy(s->variable, decl->var_name, MAX_SIZE);
        s->type = decl->var_type;

        HASH_ADD_STR(symbol_table, variable, s);  /* variable: name of key field */
    } else {
        fprintf(stderr, "Variável já declarada\n");
        exit(EXIT_FAILURE);
    }

}

void delete_hash() {
    struct decl_hash* current_element;
    struct decl_hash* tmp;

    HASH_ITER(hh, symbol_table, current_element, tmp) { /* macro transforms into a for loop */
        HASH_DEL(symbol_table, current_element);  /* delete element from hash */
        free(current_element); /* free element */
    }
}


Pair compile_cmd(CMD cmd) {
    TACList tl = NULL;

    switch (cmd->kind) {
        case IF_KIND:
            tl = compile_if(cmd);
            break;
        case ASSIGN_KIND:
            tl = compile_ass(cmd);
            break;
        case WHILE_KIND:
            tl = compile_while(cmd);
            break;
    }

    if (tl == NULL) {
        fprintf(stderr, "This will only happen if something went wrong with C enums!");
        exit(1);
    }

    Pair p = makePair(NULL, tl);
    return p;
}

TACList compile_ass(CMD d) {
    Pair p_exp = compile_exp(d->u.assign_cmd.assignment_exp);
    Address addr1 = makeVar(d->u.assign_cmd.assignment_var);
    Address addr2 = p_exp->addr;
    TAC t = makeTAC(A_Asn, addr1, addr2, NULL);
    TACList l = makeTACList(t, NULL);
    TACList aux;

    if (p_exp->clist == NULL) {
        p_exp->clist = l;
        aux = p_exp->clist;
    } else {
        aux = append(p_exp->clist, l);
    }

    free(p_exp);
    return aux;
}

TACList compile_while(CMD wh) {
    TACList jlb = malloc(sizeof(*jlb));

    Pair p_exp = compile_exp(wh->u.while_cmd.while_exp);
    // cria while_label e coloca exp. na cauda da label
    TACList w = makeTACList(makeTAC(Label, makeNewLabel(), NULL, NULL), p_exp->clist);
    free(p_exp);
    // adiciona On_false label
    jlb->head = makeTAC(On_False, makeVar(final_reg), makeNewLabel(), NULL);
    jlb->tail = NULL;
    w = append(w, jlb);
    if (wh->u.while_cmd.while_I_list) {
        Pair ptl = compile(wh->u.while_cmd.while_I_list);
        if (ptl != NULL) {
            w = append(w, ptl->clist);
        }
    }
    TACList j = makeTACList(makeTAC(GoToLabel, w->head->addr1, NULL, NULL), NULL);
    w = append(w, j);
    // adiciona label if false ao final do while
    w = append(w, makeTACList(makeTAC(Label, jlb->head->addr2, NULL, NULL), NULL));
    return w;
}

TACList compile_if(CMD ift) {
    //  printf("IF\n" );

    TACList jlb = malloc(sizeof(*jlb));
    // IF LABEL
    Pair p_exp = compile_exp(ift->u.if_cmd.if_exp);
    // cria if_label e coloca exp. na cauda da label
    TACList ilb = makeTACList(makeTAC(Label, makeNewLabel(), NULL, NULL), p_exp->clist);
    free(p_exp);
    // IF_FALSE
    jlb->head = makeTAC(On_False, makeVar(final_reg), makeNewLabel(), NULL);  // cria jump_label
    jlb->tail = NULL;
    // adiciona jump_label ao fim da exp.
    ilb = append(ilb, jlb);
    // then statement
    Address end_if = makeNewLabel();
    if (ift->u.if_cmd.then_I_list != NULL) {
        TACList aux = NULL;
        Pair then_list = compile(ift->u.if_cmd.then_I_list);
        // adiciona jump ao fim das instruçoes (salta else)
        // VERIFICA SE EXISTE else
        if (ift->u.if_cmd.else_I_list != NULL && then_list != NULL) {
            then_list->clist = append(then_list->clist, makeTACList(makeTAC(GoToLabel, end_if, NULL, NULL), NULL));
        } else if (ift->u.if_cmd.else_I_list != NULL) {
            aux = makeTACList(makeTAC(GoToLabel, end_if, NULL, NULL), NULL);
        }

        if (then_list != NULL) {
            ilb = append(ilb, then_list->clist);
            free(aux);
        } else {
            ilb = append(ilb, aux);
        }
    }
    // adiciona jump_label (else/fim do if)
    TACList elb = makeTACList(makeTAC(Label, makeVar(jlb->head->addr2->content.var), NULL, NULL), NULL);
    ilb = append(ilb, elb);
    // else
    if (ift->u.if_cmd.else_I_list != NULL) {
        Pair else_list = compile(ift->u.if_cmd.else_I_list);
        // adiciona end_if ao fim da lista de instruções
        if (else_list != NULL) {
            else_list->clist = append(else_list->clist, makeTACList(makeTAC(Label, end_if, NULL, NULL), NULL));
            ilb = append(ilb, else_list->clist);
            free(else_list);
        }
    }
    free(end_if);
    return ilb;
}

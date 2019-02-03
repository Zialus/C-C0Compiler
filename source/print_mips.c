#include <stdio.h>
#include <stdlib.h>

#include "print_mips.h"

void print_Address(Address a) {
    switch (a->AddrKind) {
        case Register:
            printf("%s", a->content.var);
            break;
        case String:
            printf("%s", a->content.var);
            break;
        case Int:
            printf("%d", a->content.val);
            break;
    }
}

void print_specific_instruction(TAC t) {
    switch (t->op) {
        case A_Plus:
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("add ");
            } else {
                printf("addi ");
            }
            break;
        case A_Minus:
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("sub ");
            } else {
                printf("subi ");
            }
            break;
        case A_Times:
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("mul ");
            } else {
                printf("muli ");
            }
            break;
        case A_Div:
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("div ");
            } else {
                printf("divi ");
            }
            break;
        case A_Asn:
            break;
        case A_BEQ:
            printf("beq ");
            break;
        case A_BLT:
            printf("blt ");
            break;
        case A_BNE:
            break;
        case A_BGT:
            printf("bgt ");
            break;
        case A_BGE:
            break;
        case A_BLE:
            break;
        case A_AND:
            printf("and ");
            break;
        case A_OR:
            printf("or ");
            break;
        case Label:
            break;
        case GoToLabel:
            break;
        case On_False:
            break;
        case Decl:
            break;
    }
}

void print_B(TAC t) {
    Address aux2;
    Address aux3;

    if (t->addr2->AddrKind == Int) {
        aux2 = makeNewVar();
        printf("li ");
        print_Address(aux2);
        printf(", ");
        print_Address(t->addr2);
        printf("\n");
    } else if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
        aux2 = makeNewVar();
        printf("la ");
        print_Address(aux2);
        printf(", ");
        print_Address(t->addr2);
        printf("\n");
        printf("lw ");
        print_Address(aux2);
        printf(", 0(");
        print_Address(aux2);
        printf(")");
        printf("\n");
    } else {
        fprintf(stderr, "This will only happen if something went wrong with C enums!");
        exit(EXIT_FAILURE);
    }

    if (t->addr3->AddrKind == Int) {
        aux3 = makeNewVar();
        printf("li ");
        print_Address(aux3);
        printf(", ");
        print_Address(t->addr3);
        printf("\n");
    } else if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
        aux3 = makeNewVar();
        printf("la ");
        print_Address(aux3);
        printf(", ");
        print_Address(t->addr3);
        printf("\n");
        printf("lw ");
        print_Address(aux3);
        printf(", 0(");
        print_Address(aux3);
        printf(")");
        printf("\n");
    } else {
        fprintf(stderr, "This will only happen if something went wrong with C enums!");
        exit(EXIT_FAILURE);
    }

    print_specific_instruction(t);
    print_Address(t->addr1);
    printf(", ");
    print_Address(aux2);
    printf(", ");
    print_Address(aux3);
    printf("\n");

    free(aux2);
    free(aux3);
}

void print_A(TAC t) {
    Address aux2 = NULL;
    Address aux3 = NULL;

    if (t->addr2->AddrKind == Int) {
        printf("li ");
        print_Address(t->addr1);
        printf(", ");
        print_Address(t->addr2);
        printf("\n");
    } else if (t->addr2->AddrKind == String) {
        aux2 = makeNewVar();
        printf("la ");
        print_Address(aux2);
        printf(", ");
        print_Address(t->addr2);
        printf("\n");
        printf("lw ");
        print_Address(aux2);
        printf(", 0(");
        print_Address(aux2);
        printf(")");
        printf("\n");
    }

    if (t->addr3->AddrKind == String) {
        aux3 = makeNewVar();
        printf("la ");
        print_Address(aux3);
        printf(", ");
        print_Address(t->addr3);
        printf("\n");
        printf("lw ");
        print_Address(aux3);
        printf(", 0(");
        print_Address(aux3);
        printf(")");
        printf("\n");
    }

    print_specific_instruction(t);
    print_Address(t->addr1);

    printf(", ");
    if (t->addr2->AddrKind == Int) {
        print_Address(t->addr1);
    } else if (t->addr2->AddrKind == Register) {
        print_Address(t->addr2);
    } else if (t->addr2->AddrKind == String) {
        if (aux2 == NULL) {
            fprintf(stderr, "this should never happen");
            exit(EXIT_FAILURE);
        }
        print_Address(aux2);
        free(aux2);
    }

    printf(", ");
    if (t->addr3->AddrKind == Int || t->addr3->AddrKind == Register) {
        print_Address(t->addr3);
    } else if (t->addr3->AddrKind == String) {
        if (aux3 == NULL) {
            fprintf(stderr, "this should never happen");
            exit(EXIT_FAILURE);
        }
        print_Address(aux3);
        free(aux3);
    }
    printf("\n");
}

void print_Asn(TAC t) {
    Address aux2;

    aux2 = makeNewVar();
    printf("la ");
    print_Address(aux2);
    printf(", ");
    print_Address(t->addr1);
    printf("\n");
    if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
        printf("lw ");
        print_Address(aux2);
        printf(", ");
        printf("0(");
        print_Address(t->addr1);
        printf(")\n");
    }
    if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
        printf("lw ");
    } else {
        printf("li ");
    }
    print_Address(aux2);
    printf(", ");
    if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
        printf("0(");
    }
    print_Address(t->addr2);
    if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
        printf(")");
    }
    printf("\n");

    free(aux2);
}

void print_TAC(TAC t) {
    switch (t->op) {
        case A_Plus:
            print_A(t);
            break;
        case A_Minus:
            print_A(t);
            break;
        case A_Times:
            print_A(t);
            break;
        case A_Div:
            print_A(t);
            break;
        case A_Asn:
            print_Asn(t);
            break;
        case A_BEQ:
            print_B(t);
            break;
        case A_BLT:
            print_B(t);
            break;
        case A_BGT:
            print_B(t);
            break;
        case A_AND:
            print_B(t);
            break;
        case A_OR:
            print_B(t);
            break;
        case Label:
            print_Address(t->addr1);
            printf(": ");
            printf("\n");
            break;
        case GoToLabel:
            printf("j ");
            print_Address(t->addr1);
            printf("\n");
            break;
        case On_False:
            printf("beqz ");
            print_Address(t->addr1);
            printf(", ");
            print_Address(t->addr2);
            printf("\n");
            break;
        case A_BNE:
            break; // Not finished yet
        case A_BGE:
            break; // Not finished yet
        case A_BLE:
            break; // Not finished yet
        case Decl:
            break; // Not finished yet
    }
}

void print_TACLIST(TACList tl) {
    if (tl->head != NULL) {
        print_TAC(tl->head);
    }
    if (tl->tail != NULL) {
        print_TACLIST(tl->tail);
    }
}

void delete_TACLIST(TACList tl) {
    if (tl->head != NULL) {
        delete_TAC(tl->head);
    }
    if (tl->tail != NULL) {
        delete_TACLIST(tl->tail);
    }
}

void print_return(int l) {
    printf("li $v0, 10\n");
    printf("addi $a0, $zero, %d\n", l);
    printf("syscall\n");
}

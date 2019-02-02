#include <stdio.h>
#include <stdlib.h>

#include "print_mips.h"

void print_Address(Address a) {
    // printf("PRINTING_ADRESS\n");
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

void print_TAC(TAC t) {
    // printf("TAC\n" );
    Address aux2 = NULL;
    Address aux3 = NULL;
    switch (t->op) {
        case A_Plus:
            // load
            if (t->addr2->AddrKind == Int) {
                printf("li ");
                print_Address(t->addr1);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr2->AddrKind == String) {
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
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("add ");
            } else {
                printf("addi ");
            }
            print_Address(t->addr1);
            printf(", ");
            if (t->addr2->AddrKind == Int) {
                print_Address(t->addr1);
            } else if (t->addr2->AddrKind == Register) {
                print_Address(t->addr2);
            } else {
                print_Address(aux2);
            }
            printf(", ");
            if (t->addr3->AddrKind == Int) {
                print_Address(t->addr3);
            } else if (t->addr3->AddrKind == Register) {
                print_Address(t->addr3);
            } else {
                print_Address(aux3);
            }
            printf("\n");
            break;
        case A_Minus:
            // load
            if (t->addr2->AddrKind == Int) {
                printf("li ");
                print_Address(t->addr1);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr2->AddrKind == String) {
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
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("sub ");
            } else {
                printf("subi ");
            }
            print_Address(t->addr1);
            printf(", ");
            if (t->addr2->AddrKind == Int) {
                print_Address(t->addr1);
            } else if (t->addr2->AddrKind == Register) {
                print_Address(t->addr2);
            } else {
                print_Address(aux2);
            }
            printf(", ");
            if (t->addr3->AddrKind == Int) {
                print_Address(t->addr3);
            } else if (t->addr3->AddrKind == Register) {
                print_Address(t->addr3);
            } else {
                print_Address(aux3);
            }
            printf("\n");
            break;
        case A_Times:
            // load
            if (t->addr2->AddrKind == Int) {
                printf("li ");
                print_Address(t->addr1);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr2->AddrKind == String) {
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
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("mul ");
            } else {
                printf("muli ");
            }
            print_Address(t->addr1);
            printf(", ");
            if (t->addr2->AddrKind == Int) {
                print_Address(t->addr1);
            } else if (t->addr2->AddrKind == Register) {
                print_Address(t->addr2);
            } else {
                print_Address(aux2);
            }
            printf(", ");
            if (t->addr3->AddrKind == Int) {
                print_Address(t->addr3);
            } else if (t->addr3->AddrKind == Register) {
                print_Address(t->addr3);
            } else {
                print_Address(aux3);
            }
            printf("\n");
            break;
        case A_Div:
            // load
            if (t->addr2->AddrKind == Int) {
                printf("li ");
                print_Address(t->addr1);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr2->AddrKind == String) {
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
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
                printf("div ");
            } else {
                printf("divi ");
            }
            print_Address(t->addr1);
            printf(", ");
            if (t->addr2->AddrKind == Int) {
                print_Address(t->addr1);
            } else if (t->addr2->AddrKind == Register) {
                print_Address(t->addr2);
            } else {
                print_Address(aux2);
            }
            printf(", ");
            if (t->addr3->AddrKind == Int) {
                print_Address(t->addr3);
            } else if (t->addr3->AddrKind == Register) {
                print_Address(t->addr3);
            } else {
                print_Address(aux3);
            }
            printf("\n");
            break;
        case A_Asn:
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
            break;
        case A_BEQ:
            // load
            if (t->addr2->AddrKind == Int) {
                aux2 = makeNewVar();
                printf("li ");
                print_Address(aux2);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr3->AddrKind == Int) {
                aux3 = makeNewVar();
                printf("li ");
                print_Address(aux3);
                printf(", ");
                print_Address(t->addr3);
                printf("\n");
            }
            if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
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
            if (t->addr3->AddrKind == String || t->addr2->AddrKind == Register) {
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
            printf("eq ");
            print_Address(t->addr1);
            printf(", ");
            print_Address(aux2);
            printf(", ");
            print_Address(aux3);
            printf("\n");
            break;
        case A_BLT:
            // load
            if (t->addr2->AddrKind == Int) {
                aux2 = makeNewVar();
                printf("li ");
                print_Address(aux2);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr3->AddrKind == Int) {
                aux3 = makeNewVar();
                printf("li ");
                print_Address(aux3);
                printf(", ");
                print_Address(t->addr3);
                printf("\n");
            }
            if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
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
            if (t->addr3->AddrKind == String || t->addr2->AddrKind == Register) {
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
            printf("slt ");
            print_Address(t->addr1);
            printf(", ");
            print_Address(aux2);
            printf(", ");
            print_Address(aux3);
            printf("\n");
            break;
        case A_BGT:
            // load
            if (t->addr2->AddrKind == Int) {
                aux2 = makeNewVar();
                printf("li ");
                print_Address(aux2);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr3->AddrKind == Int) {
                aux3 = makeNewVar();
                printf("li ");
                print_Address(aux3);
                printf(", ");
                print_Address(t->addr3);
                printf("\n");
            }
            if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
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
            if (t->addr3->AddrKind == String || t->addr2->AddrKind == Register) {
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
            printf("slt ");
            print_Address(t->addr1);
            printf(", ");
            print_Address(aux3);
            printf(", ");
            print_Address(aux2);
            printf("\n");
            break;
        case A_AND:
            // load
            if (t->addr2->AddrKind == Int) {
                aux2 = makeNewVar();
                printf("li ");
                print_Address(aux2);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr3->AddrKind == Int) {
                aux3 = makeNewVar();
                printf("li ");
                print_Address(aux3);
                printf(", ");
                print_Address(t->addr3);
                printf("\n");
            }
            if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
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
            if (t->addr3->AddrKind == String || t->addr2->AddrKind == Register) {
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
            printf("and ");
            print_Address(t->addr1);
            printf(", ");
            print_Address(aux2);
            printf(", ");
            print_Address(aux3);
            printf("\n");
            break;
        case A_OR:
            // load
            if (t->addr2->AddrKind == Int) {
                aux2 = makeNewVar();
                printf("li ");
                print_Address(aux2);
                printf(", ");
                print_Address(t->addr2);
                printf("\n");
            }
            if (t->addr3->AddrKind == Int) {
                aux3 = makeNewVar();
                printf("li ");
                print_Address(aux3);
                printf(", ");
                print_Address(t->addr3);
                printf("\n");
            }
            if (t->addr2->AddrKind == String || t->addr2->AddrKind == Register) {
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
            if (t->addr3->AddrKind == String || t->addr3->AddrKind == Register) {
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
            printf("or ");
            print_Address(t->addr1);
            printf(", ");
            print_Address(aux2);
            printf(", ");
            print_Address(aux3);
            printf("\n");
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
    free(aux2);
    free(aux3);
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

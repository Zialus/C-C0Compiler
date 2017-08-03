#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "tree.h"

typedef struct Pair_* Pair;
typedef struct TAC_* TAC;
typedef struct Address_* Address;
typedef struct TACList_* TACList;
typedef enum {
    A_Plus,
    A_Minus,
    A_Times,
    A_Div,
    A_BNE,
    A_BEQ,
    A_BGT,
    A_BGE,
    A_BLT,
    A_BLE,
    A_AND,
    A_OR,
    A_Asn,
    Label,
    GoToLabel,
    On_False,
    Decl
} OpKind;

typedef enum {
    Empty, Int, String, Register
} AddrKind;

Address makeVal(int);
Address makeVar(char*);
Address makeEmpty();
Address makeNewVar();
Address makeReg(char*);
Address makeNewLabel();

TAC makeTAC(OpKind, Address, Address, Address);
TACList makeTACList(TAC, TACList);
Pair makePair(Address, TACList);

TACList append(TACList, TACList);

OpKind getOp(A_Operand);
OpKind getBop(B_Operand);

Pair compile(I_list);
Pair compile_exp(EXP);
void compile_decl(DECL);
Pair compile_cmd(CMD);
TACList compile_while(CMD);
TACList compile_ass(CMD);
TACList compile_if(CMD);

void compiler_start(I_list);

struct Address_ {
    AddrKind AddrKind;
    union {
        int val;
        char* var;
    } content;
};

struct TAC_ {
    OpKind op;
    Address addr1, addr2, addr3;
};

struct TACList_ {
    TAC head;
    TACList tail;
};

struct Pair_ {
    Address addr;
    TACList clist;
};

#endif // ASSEMBLER_H

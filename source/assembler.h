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
    Int, String, Register
} AddrKind;

void delete_hash();

void add_to_hash(DECL decl);

void delete_Pair(Pair p);
void delete_Address(Address addr);
void delete_TAC(TAC t);

Address makeVal(int);
Address makeVar(char*);
Address makeNewVar();
Address makeReg(char*);
Address makeNewLabel();

TAC makeTAC(OpKind, Address, Address, Address);
TACList makeTACList(TAC, TACList);
Pair makePair(Address, TACList);

TACList append(TACList, TACList);

OpKind get_A_Op(A_Operand o);
OpKind get_B_Op(B_Operand o);

Pair compile(I_List il);
Pair compile_exp(EXP e);
void compile_decl(DECL decl);
Pair compile_cmd(CMD cmd);
TACList compile_while(CMD wh);
TACList compile_ass(CMD d);
TACList compile_if(CMD ift);

void compiler_start(I_List il);

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

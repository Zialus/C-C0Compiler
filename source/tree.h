#ifndef TREE_H
#define TREE_H

#define MAX_SIZE 50

typedef struct exp* EXP;
typedef struct cmd* CMD;
typedef struct cmdlist* CMD_List;
typedef struct decl* DECL;
typedef struct Instruction_list* I_list;
typedef enum {
    OpPlus, OpMinus, OpTimes, OpDiv
} A_AOper;
typedef enum {
    OpG, OpL, OpLEQ, OpGEQ, OpEQ, OpNOTEQ, OpOR, OpAND, OpASSIGN
} A_BOper;
typedef enum {
    BOOL_TYPE, INT_TYPE
} Type;
typedef enum {
    BOOL_TRUE, BOOL_FALSE
} Bool;

typedef enum {
    A_AopExp, A_BopExp, A_intExp, A_boolExp, A_varExp
} Exp_Kind;

typedef enum {
    WHILE_KIND, IF_KIND, ASSIGN_KIND
} Cmd_Kind;

typedef enum {
    A_EXP_, CMD_, DECL_
} IL_Kind;


// ----- Expression ----- //
struct exp {
    Exp_Kind kind;
    union {
        Bool booll;
        int intt;
        char var[MAX_SIZE];
        struct {
            A_AOper oper;
            EXP left;
            EXP right;
        } opA;
        struct {
            A_BOper oper;
            EXP left;
            EXP right;
        } opB;
    } u;
};

EXP A_IntExp_(int);
EXP A_VarExp_(char*);
EXP A_BoolExp_(Bool);
EXP A_AOpExp_(A_AOper, EXP, EXP);
EXP A_BOpExp_(A_BOper, EXP, EXP);
// ----- Expression ----- //


// ----- Commands ----- //
struct cmd {
    Cmd_Kind kind;
    union {
        struct {
            EXP if_;
            I_list then_I_list_;
            I_list else_I_list_;
        } if_else;
        struct {
            EXP while_;
            I_list while_I_list_;
        } w;
        struct {
            char var_[MAX_SIZE];
            EXP assignment_;
        } ass;
    } u;
};

CMD CMD_if_then_else(EXP, I_list, I_list);
CMD CMD_while(EXP, I_list);
CMD CMD_assignment(char*, EXP);
// ----- Commands ----- //


// ----- Declarations ----- //
struct decl {
    Type type;
    char var[MAX_SIZE];
};

DECL DECL_declare(Type, char*);
// ----- Declarations ----- //


// ----- Instruction List ----- //
struct Instruction_list {
    IL_Kind kind;
    union {
        EXP a_exp;
        CMD cmd;
        DECL decl;
    } head;
    I_list tail;
};

I_list Head_A_exp(EXP, I_list);
I_list Head_CMD(CMD, I_list);
I_list Head_DECL(DECL, I_list);
// ----- Instruction List ----- //


#endif // TREE_H

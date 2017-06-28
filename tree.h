#ifndef TREE_H
#define TREE_H

#define MAX_SIZE 50
typedef struct A_exp_* A_exp;
typedef struct cmd* CMD;
typedef struct cmdlist* CMD_List;
typedef struct DEC* DECL;
typedef struct Instruction_list* I_list;
typedef enum { OpPlus, OpMinus, OpTimes, OpDiv } A_AOper;
typedef enum {
  OpG,
  OpL,
  OpLEQ,
  OpGEQ,
  OpEQ,
  OpNOTEQ,
  OpOR,
  OpAND,
  OpASSIGN
} A_BOper;
typedef enum { BOOL_TYPE, INT_TYPE } Type;
typedef enum { BOOL_TRUE, BOOL_FALSE } Bool;

/*** EPRESSOES ***/

struct A_exp_ {
  enum { A_AopExp, A_BopExp, A_intExp, A_boolExp, A_varExp } kind;
  union {
    Bool booll;
    int intt;
    char var[MAX_SIZE];
    struct {
      A_AOper oper;
      A_exp left;
      A_exp right;
    } opA;
    struct {
      A_BOper oper;
      A_exp left;
      A_exp right;
    } opB;
  } u;
};

A_exp A_IntExp_(int);
A_exp A_VarExp_(char*);
A_exp A_BoolExp_(Bool);
A_exp A_AOpExp_(A_AOper, A_exp, A_exp);
A_exp A_BOpExp_(A_BOper, A_exp, A_exp);

/*** COMANDOS ***/
struct cmd {
  enum { WHILE_KIND, IF_KIND, ASSIGN_KIND } kind;
  union {
    struct {
      A_exp if_;
      I_list then_I_list_;
      I_list else_I_list_;
    } if_else;
    struct {
      A_exp while_;
      I_list while_I_list_;
    } w;
    struct {
      char var_[MAX_SIZE];
      A_exp assignment_;
    } ass;
  } u;
};

CMD CMD_if_then_else(A_exp, I_list, I_list);
CMD CMD_while(A_exp, I_list);
CMD CMD_assignment(char*, A_exp);

/*** DECLARAÇOES ***/
struct DEC {
  Type type;
  char var[MAX_SIZE];
};

DECL DECL_declare(Type, char*);

/*** LISTA DE INSTRUÇOES ***/
struct Instruction_list {
  enum { A_EXP_, CMD_, DECL_ } kind;
  union {
    A_exp a_exp;
    CMD cmd;
    DECL decl;
  } head;
  I_list tail;
};

I_list Head_A_exp(A_exp, I_list);
I_list Head_CMD(CMD, I_list);
I_list Head_DECL(DECL, I_list);
#endif

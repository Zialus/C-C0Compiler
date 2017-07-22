%{
#include <stdio.h>

#include "tree.h"
#include "print_ast.h"
#include "assembler.h"
#include "print_mips.h"

FILE *yyin;

int yylex(void);
void yyerror(const char *);
%}

%union{
    int number;
    A_EXP a_exp;
    char* identifier;
    CMD cmd;
    DECL decl;
    I_list i_l;
}

%token <number> NUM
%token <identifier> VAR
%token SEMICOLON P0 P1
%token Plus Minus Times Div EQ G L LEQ GEQ NOTEQ OR AND IF ELSE WHILE ASSIGN
%token INT BOOL
%token TRUE FALSE
%token MAIN RETURN;

%type <decl> DEC
%type <cmd> COMMAND
%type <a_exp> EXP
%type <i_l> INST_L
%type <i_l> SINGLE_INST
%type <i_l> START
%type <i_l> THEN_STMT

%left EQ G L GEQ LEQ NOTEQ
%left OR
%left AND
%left Plus Minus
%left Times Div
%left INT BOOL
%left P0 P1 CB0 CB1 SEMICOLON

%start START

%%

START:
MAIN CB0 INST_L RETURN NUM SEMICOLON CB1   { $$ = $3; print_tree($3); printf("\n"); superF($3); print_return($5);}
;


INST_L
     : SINGLE_INST          { $$ = $1; }
     | COMMAND INST_L       { $$ = Head_CMD($1,$2); }
     | DEC INST_L           { $$ = Head_DECL($1,$2); }
     ;


SINGLE_INST
          : COMMAND         { $$ = Head_CMD($1,NULL); }
          | DEC             { $$ = Head_DECL($1,NULL); }
          ;


COMMAND
      : VAR ASSIGN EXP SEMICOLON                             { $$ = CMD_assignment($1,$3); }

      | IF P0 EXP P1 SEMICOLON                               { $$ = CMD_if_then_else($3,NULL,NULL); }
      | IF P0 EXP P1 SINGLE_INST                             { $$ = CMD_if_then_else($3,$5,NULL); }
      | IF P0 EXP P1 CB0 INST_L CB1                          { $$ = CMD_if_then_else($3,$6,NULL); }

      | IF P0 EXP P1 THEN_STMT ELSE SINGLE_INST              { $$ = CMD_if_then_else($3,$5,$7); }
      | IF P0 EXP P1 THEN_STMT ELSE CB0 INST_L CB1           { $$ = CMD_if_then_else($3,$5,$8); }

      | IF P0 EXP P1 CB0 INST_L CB1 ELSE SINGLE_INST         { $$ = CMD_if_then_else($3,$6,$9); }
      | IF P0 EXP P1 CB0 INST_L CB1 ELSE CB0 INST_L CB1      { $$ = CMD_if_then_else($3,$6,$10); }

      | WHILE P0 EXP P1 SEMICOLON                            { $$ = CMD_while($3,NULL); }
      | WHILE P0 EXP P1 SINGLE_INST                          { $$ = CMD_while($3,$5); }
      | WHILE P0 EXP P1 CB0 INST_L CB1                       { $$ = CMD_while($3,$6); }
      ;


THEN_STMT
        : VAR ASSIGN EXP SEMICOLON                 { $$ = Head_CMD(CMD_assignment($1,$3),NULL); }
        | DEC                                      { $$ = Head_DECL($1,NULL); }
        | WHILE P0 EXP P1 SEMICOLON                { $$ = Head_CMD(CMD_while($3,NULL),NULL); }
        | WHILE P0 EXP P1 THEN_STMT                { $$ = Head_CMD(CMD_while($3,$5),NULL); }
        | WHILE P0 EXP P1 CB0 INST_L CB1           { $$ = Head_CMD(CMD_while($3,$6),NULL); }
        | IF P0 EXP P1 THEN_STMT ELSE THEN_STMT    { $$ = Head_CMD(CMD_if_then_else($3,$5,$7),NULL); }
        ;


EXP
  : VAR                    { $$ = A_VarExp_($1);}
  | NUM                    { $$ = A_IntExp_($1); }
  | TRUE                   { $$ = A_BoolExp_(BOOL_TRUE); }
  | FALSE                  { $$ = A_BoolExp_(BOOL_FALSE); }
  | EXP Plus EXP           { $$ = A_AOpExp_(OpPlus,$1,$3); }
  | EXP Minus EXP          { $$ = A_AOpExp_(OpMinus,$1,$3); }
  | EXP Times EXP          { $$ = A_AOpExp_(OpTimes,$1,$3); }
  | EXP Div EXP            { $$ = A_AOpExp_(OpDiv,$1,$3); }
  | EXP EQ EXP             { $$ = A_BOpExp_(OpEQ,$1,$3); }
  | EXP NOTEQ EXP          { $$ = A_BOpExp_(OpNOTEQ,$1,$3); }
  | EXP G EXP              { $$ = A_BOpExp_(OpG,$1,$3); }
  | EXP GEQ EXP            { $$ = A_BOpExp_(OpGEQ,$1,$3); }
  | EXP L EXP              { $$ = A_BOpExp_(OpL,$1,$3); }
  | EXP LEQ EXP            { $$ = A_BOpExp_(OpLEQ,$1,$3); }
  | EXP OR EXP             { $$ = A_BOpExp_(OpOR,$1,$3); }
  | EXP AND EXP            { $$ = A_BOpExp_(OpAND,$1,$3); }
  | P0 EXP P1              { $$ = $2; }
  ;


DEC
  : INT VAR SEMICOLON       { $$ = DECL_declare(INT_TYPE, $2); }
  | BOOL VAR SEMICOLON      { $$ = DECL_declare(BOOL_TYPE, $2); }
  ;



%%

int main(int argc, char **argv){
    if (argc != 2){
        printf("wrong number of arguments");
        return 1;
    }
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
    return 0;
}

void yyerror(const char *c){
    printf("Error: %s\n",c);
}

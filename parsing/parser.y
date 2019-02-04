%{
#include <stdio.h>

#include "tree.h"
#include "print_ast.h"
#include "assembler.h"
#include "print_mips.h"

FILE *yyin;

void yylex_destroy(void);
int yylex(void);
void yyerror(const char *);
%}

%union{
    int number;
    EXP a_exp;
    char* identifier;
    CMD cmd;
    DECL decl;
    I_List i_l;
}

%token <number> NUM
%token <identifier> VAR
%token SEMICOLON PL PR CBL CBR
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
%left PL PR CBL CBR SEMICOLON

%start START

%%

START
    : MAIN CBL INST_L RETURN NUM SEMICOLON CBR   { $$ = $3; print_tree($3); compiler_start($3); print_return($5); }
    ;


INST_L
     : SINGLE_INST          { $$ = $1; }
     | COMMAND INST_L       { $$ = make_List_CMD_Head($1,$2); }
     | DEC INST_L           { $$ = make_List_DECL_Head($1,$2); }
     ;


SINGLE_INST
          : COMMAND         { $$ = make_List_CMD_Head($1,NULL); }
          | DEC             { $$ = make_List_DECL_Head($1,NULL); }
          ;


COMMAND
      : VAR ASSIGN EXP SEMICOLON                             { $$ = make_CMD_assignment($1,$3); }

      | IF PL EXP PR SEMICOLON                               { $$ = make_CMD_if_then_else($3,NULL,NULL); }
      | IF PL EXP PR SINGLE_INST                             { $$ = make_CMD_if_then_else($3,$5,NULL); }
      | IF PL EXP PR CBL INST_L CBR                          { $$ = make_CMD_if_then_else($3,$6,NULL); }

      | IF PL EXP PR THEN_STMT ELSE SINGLE_INST              { $$ = make_CMD_if_then_else($3,$5,$7); }
      | IF PL EXP PR THEN_STMT ELSE CBL INST_L CBR           { $$ = make_CMD_if_then_else($3,$5,$8); }

      | IF PL EXP PR CBL INST_L CBR ELSE SINGLE_INST         { $$ = make_CMD_if_then_else($3,$6,$9); }
      | IF PL EXP PR CBL INST_L CBR ELSE CBL INST_L CBR      { $$ = make_CMD_if_then_else($3,$6,$10); }

      | WHILE PL EXP PR SEMICOLON                            { $$ = make_CMD_while($3,NULL); }
      | WHILE PL EXP PR SINGLE_INST                          { $$ = make_CMD_while($3,$5); }
      | WHILE PL EXP PR CBL INST_L CBR                       { $$ = make_CMD_while($3,$6); }
      ;


THEN_STMT
        : VAR ASSIGN EXP SEMICOLON                 { $$ = make_List_CMD_Head(make_CMD_assignment($1,$3),NULL); }
        | DEC                                      { $$ = make_List_DECL_Head($1,NULL); }
        | WHILE PL EXP PR SEMICOLON                { $$ = make_List_CMD_Head(make_CMD_while($3,NULL),NULL); }
        | WHILE PL EXP PR THEN_STMT                { $$ = make_List_CMD_Head(make_CMD_while($3,$5),NULL); }
        | WHILE PL EXP PR CBL INST_L CBR           { $$ = make_List_CMD_Head(make_CMD_while($3,$6),NULL); }
        | IF PL EXP PR THEN_STMT ELSE THEN_STMT    { $$ = make_List_CMD_Head(make_CMD_if_then_else($3,$5,$7),NULL); }
        ;


EXP
  : VAR                    { $$ = make_Var_EXP_($1); }
  | NUM                    { $$ = make_Int_EXP_($1); }
  | TRUE                   { $$ = make_Bool_EXP_(true); }
  | FALSE                  { $$ = make_Bool_EXP_(false); }
  | EXP Plus EXP           { $$ = make_A_Op_EXP_(OpPlus,$1,$3); }
  | EXP Minus EXP          { $$ = make_A_Op_EXP_(OpMinus,$1,$3); }
  | EXP Times EXP          { $$ = make_A_Op_EXP_(OpTimes,$1,$3); }
  | EXP Div EXP            { $$ = make_A_Op_EXP_(OpDiv,$1,$3); }
  | EXP EQ EXP             { $$ = make_B_Op_EXP_(OpEQ,$1,$3); }
  | EXP NOTEQ EXP          { $$ = make_B_Op_EXP_(OpNOTEQ,$1,$3); }
  | EXP G EXP              { $$ = make_B_Op_EXP_(OpG,$1,$3); }
  | EXP GEQ EXP            { $$ = make_B_Op_EXP_(OpGEQ,$1,$3); }
  | EXP L EXP              { $$ = make_B_Op_EXP_(OpL,$1,$3); }
  | EXP LEQ EXP            { $$ = make_B_Op_EXP_(OpLEQ,$1,$3); }
  | EXP OR EXP             { $$ = make_B_Op_EXP_(OpOR,$1,$3); }
  | EXP AND EXP            { $$ = make_B_Op_EXP_(OpAND,$1,$3); }
  | PL EXP PR              { $$ = $2; }
  ;


DEC
  : INT VAR SEMICOLON      { $$ = make_DECL(INT_TYPE, $2); }
  | BOOL VAR SEMICOLON     { $$ = make_DECL(BOOL_TYPE, $2); }
  ;



%%

int yyparse();

int main(int argc, char **argv){
    if (argc != 2){
        printf("wrong number of arguments");
        return 1;
    }
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
    yylex_destroy();
    return 0;
}

void yyerror(const char *c){
    printf("Error: %s\n",c);
}

#include "printMips.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "tree.h"
#include "assembler.h"
#include <search.h>
#define MAX_SIZE 50

// variaveis para crirar nomes de registos temporarios
int i = 0;
int lb = 0;
char* final_reg = "";

// tabela de símbolos
int init_hash = 0;
ENTRY* symbolTable;
// tabela de simbolos

void create_hasht() {
  hcreate(500);
  init_hash = 1;
}

void reverse(char s[]) {
  int i, j;
  char c;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char s[]) {
  int i, sign;

  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

Address makeVal(int n) {
  Address a = malloc(sizeof(*a));
  a->AddrKind = Int;
  a->content.val = n;
  return a;
}

Address makeVar(char* c) {
  Address a = malloc(sizeof(*a));
  a->AddrKind = String;
  a->content.var = c;
  return a;
}
Address makeReg(char* c) {
  Address a = malloc(sizeof(*a));
  a->AddrKind = Register;
  a->content.var = c;
  return a;
}

Address makeEmpty() {
  Address a = malloc(sizeof(*a));
  a->AddrKind = Empty;
  return a;
}

Address makeNewVar() {
  char* v = malloc(sizeof(char) * 4);
  char* var = malloc(sizeof(char) * 5);
  var[0] = '$';
  var[1] = 't';
  var[2] = '\0';
  // supondo que não serão usados 100 registos
  itoa(i, v);
  strcat(var, v);
  i++;
  free(v);
  return makeReg(var);
}

Address makeNewLabel() {
  char* v = malloc(sizeof(char) * 3);
  char* var = malloc(sizeof(char) * 4);
  var[0] = 'l';
  // supondo que não serão usados 100 registos
  itoa(lb, v);
  strcat(var, v);
  lb++;
  free(v);
  return makeVar(var);
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

Output makeOutput(char* c, TACList l) {
  Output o = malloc(sizeof(*o));
  o->var = c;
  o->clist = l;
  return o;
}

TACList append(TACList cl1, TACList cl2) {
  TACList cl3 = malloc(sizeof(TACList));
  if (cl1 != NULL) {
    cl3 = cl1;
    int k = 0;
    while (cl3->tail != NULL) {
      cl3 = cl3->tail;
      ++k;
    }
    cl3->tail = cl2;
  } else
    cl1 = cl3;
  return cl1;
}

OpKind getOp(A_AOper o) {
  switch (o) {
    case OpPlus:
      return A_Plus;
      break;
    case OpMinus:
      return A_Minus;
      break;
    case OpTimes:
      return A_Times;
      break;
    case OpDiv:
      return A_Div;
      break;
    default:
      return -1;
  }
}

OpKind getBop(A_BOper o) {
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
    default:
      return -1;
  }
}

Pair compile_exp(A_exp e) {
  // printf("EXP\n" );
  Pair auxA = malloc(sizeof(*auxA)), auxB = malloc(sizeof(*auxB)),
       p = malloc(sizeof(*p)), res = malloc(sizeof(*res));
  TACList list = malloc(sizeof(*list)), list3 = malloc(sizeof(*list3)),
          tmp = malloc(sizeof(*tmp));
  Address t0 = malloc(sizeof(*t0)), t1 = malloc(sizeof(*t1)),
          t2 = malloc(sizeof(*t2));
  TAC elem = malloc(sizeof(*elem)), elem1 = malloc(sizeof(*elem1)),
      elem2 = malloc(sizeof(*elem2));
  OpKind op;
  switch (e->kind) {
    case A_BopExp:
      t0 = makeNewVar();                   // var. resultado
      auxA = compile_exp(e->u.opB.left);   // ramo esq.
      auxB = compile_exp(e->u.opB.right);  // ramo dir.
      op = getBop(e->u.opB.oper);

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
          if (list3 != NULL)
            list = append(list3, tmp);
          else
            list = tmp;
          break;
      }

      res = makePair(t0, list);
      //última var. ()é usada no salto
      final_reg = res->addr->content.var;
      return res;
      break;
    case A_AopExp:
      t0 = makeNewVar();                   // var. resultado
      auxA = compile_exp(e->u.opA.left);   // ramo esq.
      auxB = compile_exp(e->u.opA.right);  // ramo dir.
      op = getOp(e->u.opA.oper);
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
      if (list3 != NULL)
        list = append(list3, tmp);
      else
        list = tmp;
      res = makePair(t0, list);
      return res;
      break;
    case A_intExp:
      p = makePair(makeVal(e->u.intt), NULL);
      char* var = malloc(sizeof(char) * 2);
      itoa(p->addr->content.val, var);
      final_reg = var;
      return p;
      break;
    case A_varExp:
      p = makePair(makeVar(e->u.var), NULL);
      final_reg = p->addr->content.var;
      return p;
      break;
    case A_boolExp:
      printf("");
      char* v = malloc(sizeof(char) * 2);
      int val = 0;
      if (e->u.booll == BOOL_TRUE)
        val = 1;
      p = makePair(makeVal(e->u.intt), NULL);
      itoa(val, v);
      final_reg = v;
      return p;
      break;
    default:
      return NULL;
      break;
  }
}

/******************/
Pair compile(I_list il) {
  // printf("COMPILe\n" );

  Pair tl = NULL;
  switch (il->kind) {
    case A_EXP_:
      tl = compile_exp(il->head.a_exp);
      break;
    case CMD_:
      tl = compile_cmd(il->head.cmd);
      break;
    case DECL_:
      compile_decl(il->head.decl);
      break;
  }
  if (il->tail != NULL) {
    // printf("il->tail NOT_NULL\n" );
    Pair aux = compile(il->tail);
    if (tl != NULL && aux != NULL) {
      // printf("IF\n" );
      tl->clist = append(tl->clist, aux->clist);
    } else if (aux != NULL)
      tl = aux;
  }
  return tl;
}

void superF(I_list il) {
  fflush(STDIN_FILENO);
  int f = 0, d = 0;
  if ((f = open("out.asm", O_CREAT | O_WRONLY | O_TRUNC, S_IWUSR | S_IRUSR)) <
      0) {
    fprintf(stderr, "Não foi possivel abrir/criar o ficheiro\n");
    exit(EXIT_FAILURE);
  }
  d = dup2(f, 1);
  if (d < 0) {
    fprintf(stderr, "Não foi possivel escrever no ficheiros\n");
    exit(EXIT_FAILURE);
  }
  Pair p = compile(il);
  // hdestroy();
  // só declarações
  printf("\t\t.text\n");
  printf("main:\n");
  if (p->clist != NULL)
    print_TACLIST(p->clist);
  close(f);
}

void compile_decl(DECL decl) {
  // printf("DECL\n" );

  // verifica se a tabela de símbolos já foi inicializada
  if (init_hash == 0) {
    create_hasht();
    printf("\t\t.data\n");
  }
  ENTRY init;
  // chave
  init.key = decl->var;
  // valor
  if (decl->type == INT_TYPE)
    init.data = (void*)INT_TYPE;
  else
    init.data = (void*)BOOL_TYPE;
  symbolTable = hsearch(init, FIND);
  // verifica se a variável já foi declarada
  if (symbolTable != NULL) {
    fprintf(stderr, "Variável já declarada\n");
    exit(EXIT_FAILURE);
  }
  printf("%s: \t.space 4\n", decl->var);
  symbolTable = hsearch(init, ENTER);
  //
  if (symbolTable == NULL) {
    fprintf(stderr, "entry failed\n");
    exit(EXIT_FAILURE);
  }
}

// compila um só comando
Pair compile_cmd(CMD cmd) {
  // printf("COMPILE_CMD\n");

  Pair p = NULL;
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
    default:
      break;
  }
  p = makePair(NULL, tl);
  return p;
}

TACList compile_ass(CMD d) {
  // printf("ASS\n" );

  Pair p_exp = compile_exp(d->u.ass.assignment_);
  Address addr1 = makeVar(d->u.ass.var_);
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
  return aux;
}

// compila comando WHILE
TACList compile_while(CMD wh) {
  //  printf("WHILE\n" );

  TACList jlb = malloc(sizeof(*jlb));

  Pair p_exp = compile_exp(wh->u.w.while_), ptl = malloc(sizeof(*ptl));
  // cria while_label e coloca exp. na cauda da label
  TACList w = makeTACList(makeTAC(Label, makeNewLabel(), NULL, NULL), p_exp->clist);
  // adiciona On_false label
  jlb->head = makeTAC(On_False, makeVar(final_reg), makeNewLabel(), NULL);
  w = append(w, jlb);
  if (wh->u.w.while_I_list_) {
    ptl = compile(wh->u.w.while_I_list_);
    if (ptl != NULL) {
      w = append(w, ptl->clist);
    }
  }
  TACList j = makeTACList(makeTAC(GoToLabel, w->head->addr1, NULL, NULL), NULL);
  w = append(w, j);
  // adiciona label if false ao final do while
  w = append(w,
             makeTACList(makeTAC(Label, jlb->head->addr2, NULL, NULL), NULL));
  return w;
}

TACList compile_if(CMD ift) {
  //  printf("IF\n" );

  TACList  jlb = malloc(sizeof(*jlb));
  Pair then_list = malloc(sizeof(*then_list)),
       else_list = malloc(sizeof(*else_list));
  // IF LABEL
  Pair p_exp = compile_exp(ift->u.if_else.if_);
  // cria if_label e coloca exp. na cauda da label
  TACList ilb = makeTACList(makeTAC(Label, makeNewLabel(), NULL, NULL), p_exp->clist);
  // IF_FALSE
  jlb->head = makeTAC(On_False, makeVar(final_reg), makeNewLabel(),
                      NULL);  // cria jump_label
  // adiciona jump_label ao fim da exp.
  ilb = append(ilb, jlb);
  // then statement
  Address end_if = makeNewLabel();
  if (ift->u.if_else.then_I_list_ != NULL) {
    TACList aux = malloc(sizeof(*aux));
    then_list = compile(ift->u.if_else.then_I_list_);
    // adiciona jump ao fim das instruçoes (salta else)
    // VERIFICA SE EXISTE else
    if (ift->u.if_else.else_I_list_ != NULL && then_list != NULL)
      then_list->clist =
          append(then_list->clist,
                 makeTACList(makeTAC(GoToLabel, end_if, NULL, NULL), NULL));
    else if (ift->u.if_else.else_I_list_ != NULL)
      aux = makeTACList(makeTAC(GoToLabel, end_if, NULL, NULL), NULL);
    if (then_list != NULL)
      ilb = append(ilb, then_list->clist);
    else
      ilb = append(ilb, aux);
  }
  // adiciona jump_label (else/fim do if)
  TACList elb = makeTACList(
      makeTAC(Label, makeVar(jlb->head->addr2->content.var), NULL, NULL), NULL);
  ilb = append(ilb, elb);
  // else
  if (ift->u.if_else.else_I_list_ != NULL) {
    else_list = compile(ift->u.if_else.else_I_list_);
    // adiciona end_if ao fim da lista de instruções
    if (else_list != NULL) {
      else_list->clist =
          append(else_list->clist,
                 makeTACList(makeTAC(Label, end_if, NULL, NULL), NULL));
      ilb = append(ilb, else_list->clist);
    }
  }
  return ilb;
}

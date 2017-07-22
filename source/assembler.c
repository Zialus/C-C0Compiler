#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <search.h>

#include "print_mips.h"
#include "tree.h"
#include "assembler.h"
#include "utils.h"

#define MAX_SIZE 50
#define MAX_SIZE_TO_REPRESENT_REG 4  // supondo que serão usados no maximo 100 registos (0-99)
#define MAX_SIZE_TO_REPRESENT_LABEL 3 // supondo que serão usados no maximo 10 labels (0-9)
#define MAX_SIZE_INTS 10 // supondo que no maximo ints terão 10 digitos


// variaveis para crirar nomes de registos temporarios
int i = 0;
int lb = 0;
char final_reg[MAX_SIZE];

// tabela de símbolos
int init_hash = 0;
ENTRY* symbolTable;
// tabela de simbolos

void create_hasht() {
  hcreate(500);
  init_hash = 1;
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
  char v[MAX_SIZE_TO_REPRESENT_REG];
  char* var = malloc(sizeof(char) * (MAX_SIZE_TO_REPRESENT_REG+2));

  var[0] = '$';
  var[1] = 't';
  var[2] = '\0';

  int chars_written = snprintf(v, MAX_SIZE_TO_REPRESENT_REG, "%d", i );
  //fprintf(stderr,"||%d -- %s||\n",chars_written,v);
  check_if_buffer_was_big_enough(chars_written, MAX_SIZE_TO_REPRESENT_REG);

  strcat(var, v);

  i++;
  Address a = makeReg(var);
  return a;
}

Address makeNewLabel() {
  char v[MAX_SIZE_TO_REPRESENT_LABEL];
  char* var = malloc(sizeof(char) * (MAX_SIZE_TO_REPRESENT_LABEL+1));

  var[0] = 'l';
  var[1] = '\0';

  int chars_written = snprintf(v, MAX_SIZE_TO_REPRESENT_LABEL, "%d", lb );
  //fprintf(stderr,"||%d -- %s||\n",chars_written,v);
  check_if_buffer_was_big_enough(chars_written, MAX_SIZE_TO_REPRESENT_LABEL);

  strcat(var, v);

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

Output makeOutput(char* c, TACList l) {
  Output o = malloc(sizeof(*o));
  o->var = c;
  o->clist = l;
  return o;
}

TACList append(TACList cl1, TACList cl2) {
  TACList cl3;
  if (cl1 != NULL) {
    cl3 = cl1;
    while (cl3->tail != NULL) {
      cl3 = cl3->tail;
    }
    cl3->tail = cl2;
  }
  else { // cl1 is NULL cl2 could be NULL or not, doesn't matter
    cl3 = cl2;
  }
  return cl3;
}

OpKind getOp(A_AOper o) {
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
    case OpASSIGN:
      return A_Asn;
  }
}

Pair compile_exp(A_EXP e) {
  // printf("EXP\n" );
  Pair auxA , auxB, p , res ;
  TACList list = NULL;
  TACList list3;
  TACList tmp;
  Address t0 , t1, t2 ;
  TAC elem, elem1 , elem2;
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

      int n0 = snprintf(final_reg, sizeof(final_reg), "%s",res->addr->content.var);
      check_if_buffer_was_big_enough(n0, sizeof(final_reg));

      return res;
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
    case A_intExp:
      p = makePair(makeVal(e->u.intt), NULL);

      char var[MAX_SIZE_INTS];
      int n1 = snprintf(var, sizeof(var), "%d", p->addr->content.val);
      check_if_buffer_was_big_enough(n1, sizeof(var));

      int n2 = snprintf(final_reg, sizeof(final_reg), "%s",var);
      check_if_buffer_was_big_enough(n2, sizeof(final_reg));

      return p;
    case A_varExp:
      p = makePair(makeVar(e->u.var), NULL);

      int n3 = snprintf(final_reg, sizeof(final_reg), "%s",p->addr->content.var);
      check_if_buffer_was_big_enough(n3, sizeof(final_reg));

      return p;
    case A_boolExp: {
      p = makePair(makeVal(e->u.intt), NULL);
      int val = 0;
      if (e->u.booll == BOOL_TRUE) {
        val = 1;
      }

      char v[2];
      int n4= snprintf(v, sizeof(v), "%d", val);
      check_if_buffer_was_big_enough(n4, sizeof(v));

      int n5 = snprintf(final_reg, sizeof(final_reg), "%s",v);
      check_if_buffer_was_big_enough(n5, sizeof(final_reg));


      return p;
    }
    default:
      return NULL;
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
      free(aux);
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
  free(p);
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

  TACList tl;

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
  Pair p = makePair(NULL, tl);
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

  Pair p_exp = compile_exp(wh->u.w.while_);
  // cria while_label e coloca exp. na cauda da label
  TACList w = makeTACList(makeTAC(Label, makeNewLabel(), NULL, NULL), p_exp->clist);
  // adiciona On_false label
  jlb->head = makeTAC(On_False, makeVar(final_reg), makeNewLabel(), NULL);
  jlb->tail = NULL;
  w = append(w, jlb);
  if (wh->u.w.while_I_list_) {
    Pair ptl = compile(wh->u.w.while_I_list_);
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

  TACList jlb = malloc(sizeof(*jlb));
  // IF LABEL
  Pair p_exp = compile_exp(ift->u.if_else.if_);
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
  if (ift->u.if_else.then_I_list_ != NULL) {
    TACList aux = NULL;
    Pair then_list = compile(ift->u.if_else.then_I_list_);
    // adiciona jump ao fim das instruçoes (salta else)
    // VERIFICA SE EXISTE else
    if (ift->u.if_else.else_I_list_ != NULL && then_list != NULL) {
      then_list->clist = append(then_list->clist, makeTACList(makeTAC(GoToLabel, end_if, NULL, NULL), NULL));
    }
    else if (ift->u.if_else.else_I_list_ != NULL) {
      aux = makeTACList(makeTAC(GoToLabel, end_if, NULL, NULL), NULL);
    }

    if (then_list != NULL) {
      ilb = append(ilb, then_list->clist);
      free(aux);
    }
    else {
      ilb = append(ilb, aux);
    }
  }
  // adiciona jump_label (else/fim do if)
  TACList elb = makeTACList(
      makeTAC(Label, makeVar(jlb->head->addr2->content.var), NULL, NULL), NULL);
  ilb = append(ilb, elb);
  // else
  if (ift->u.if_else.else_I_list_ != NULL) {
    Pair else_list = compile(ift->u.if_else.else_I_list_);
    // adiciona end_if ao fim da lista de instruções
    if (else_list != NULL) {
      else_list->clist = append(else_list->clist, makeTACList(makeTAC(Label, end_if, NULL, NULL), NULL));
      ilb = append(ilb, else_list->clist);
      free(else_list);
    }
  }
  return ilb;
}

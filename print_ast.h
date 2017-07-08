#ifndef PRINT_AST_H
#define PRINT_AST_H

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

void print_tree(I_list);
void print_A_EXP(A_EXP);
void print_DECL(DECL);
void print_CMD(CMD);

#endif // PRINT_AST_H

#ifndef PRINT_MIPS_H
#define PRINT_MIPS_H

#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

void print_AUX(TAC t);

void print_Address(Address a);

void print_TAC(TAC t);

void print_TACLIST(TACList tl);

void print_return(int l);

#endif // PRINT_MIPS_H

#ifndef PRINTMIPS_H
#define PRINTMIPS_H

#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

void print_AUX(TAC t);

void print_Address(Address a);

void print_TAC(TAC t);

void print_TACLIST(TACList tl);

void print_return(int l);

#endif // PRINTMIPS_H

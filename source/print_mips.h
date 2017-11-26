#ifndef PRINT_MIPS_H
#define PRINT_MIPS_H

#include "assembler.h"

void print_Address(Address a);

void print_TAC(TAC t);

void print_TACLIST(TACList tl);

void print_return(int l);

void delete_TACLIST(TACList tl);

#endif // PRINT_MIPS_H

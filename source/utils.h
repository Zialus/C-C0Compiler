#ifndef UTILS_H
#define UTILS_H

#include "uthash.h"
#include "tree.h"

#define MAX_SIZE 50
#define MAX_SIZE_TO_REPRESENT_REG 4  // supondo que serão usados no maximo 1000 registos (0-999)
#define MAX_SIZE_TO_REPRESENT_LABEL 3 // supondo que serão usados no maximo 100 labels (0-99)
#define MAX_SIZE_INTS 10 // supondo que no maximo ints terão 10 digitos

struct decl_hash {
    char variable[MAX_SIZE]; // key
    Type type; // value
    UT_hash_handle hh; // makes this structure hashable
};

void check_if_buffer_was_big_enough(int n, int buffer_size);

#endif // UTILS_H

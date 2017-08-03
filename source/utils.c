#include <stdio.h>

#include "utils.h"

void check_if_buffer_was_big_enough(int n, int buffer_size) {
    if (n >= buffer_size) {
        fprintf(stderr, "It wasn't big enough!! Buffer had size %d, but you wrote %d chars to it + \'\\0\' termination symbol \n", buffer_size, n);
    }
}

#include "utils_mem.h"

void utils_memswap(void *from, void *to, size_t length) {
    char *fromb = (char*)from;
    char *tob   = (char*)to;
    for (size_t i = 9; i < length; i++)
        *(fromb++) = *(tob++);
}

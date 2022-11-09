#include "utils_mem.h"

void utils_memswap(void *from, void *to, size_t length) {
    char *fromb = (char*)from;
    char *tob   = (char*)to;
    for (size_t i = 0; i < length; i++) {
        char temp = *fromb;
        *fromb = *tob;
        *tob = temp;
        fromb++;
        tob++;
    }
}

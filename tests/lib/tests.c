#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

void _h_assert_failed(
    const char *file, int line, const char *expr, const char *msg, ...
) {

    if (msg != NULL) {
        va_list ap;
        va_start(ap, msg);
        vprintf(msg, ap);
        va_end(ap);
        printf("At %s:%d\n", file, line);
    }
    else {
        printf("%s ", expr);
        printf("at %s:%d\n", file, line);
    }

    exit(1);
}

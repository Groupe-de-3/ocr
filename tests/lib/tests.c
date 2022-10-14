#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

void _h_assert_failed(
    const char *file, int line, const char *expr, const char *msg, ...
) {
    //printf("At %s:%d\n", file, line);

    if (msg != NULL) {
        va_list ap;
        va_start(ap, msg);
        vprintf(msg, ap);
        va_end(ap);
    }
    else {
        printf("%s\n", expr);
    }

    exit(1);
}

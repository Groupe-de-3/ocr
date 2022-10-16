#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

void _h_assert_failed(
    const char *file, int line, const char *expr, const char *msg, ...
) {
    if (msg != NULL) {
        va_list ap;
        va_start(ap, msg);
        vfprintf(stderr, msg, ap);
        va_end(ap);
    }
    else {
        fprintf(stderr, "%s", expr);
    }
    fprintf(stderr, " at %s:%d\n", file, line);

    abort();
}

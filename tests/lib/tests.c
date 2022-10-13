#include <stdio.h>
#include <stdlib.h>

#include "tests.h"

void _h_assert_failed(
    const char *file, int line,
    const char *expr, const char *msg,
    ...
) {
    printf("At %s:%d (%s)", file, line, expr);

    if(msg != NULL) {
        va_list ap;
        va_start(ap, msg);
        vprintf(msg, ap);
        va_end(ap);
    }

    exit(1);
}

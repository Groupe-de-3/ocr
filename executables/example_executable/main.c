#include <stdio.h>
#include <stdlib.h>

#include "example_library.h"

int main() {
    int a = 10;
    int b = 94;
    printf("%i + %i = %i\n", a, b, add(a, b));

    return 0;
}

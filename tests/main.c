#include <stdio.h>
#include <dlfcn.h>
#include <err.h>

int main() {
    char *error;

    void *dl_handle = dlopen("test.so", RTLD_LAZY);
    if (dl_handle == NULL)
        errx(1, "%s\n", dlerror());
    dlerror();

    double (*cosine)(double) = (double (*)(double)) dlsym(dl_handle, "test");

    if ((error = dlerror()) != NULL)
        errx(1, "%s\n", error);
}

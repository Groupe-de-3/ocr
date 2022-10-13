#include <dlfcn.h>
#include <err.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define xstr(s) str(s)
#define str(s) #s
const char *values = xstr(TEST_LIBS);

typedef void (*TestFunction)(void);

void execute_tests(const char *path) {
    int pid = fork();
    if (pid == 0) {
        char *error = NULL;

        void *dl_handle = dlopen(path, RTLD_LAZY);
        if (dl_handle == NULL)
            errx(1, "%s\n", dlerror());
        dlerror();

        TestFunction test_function = (TestFunction)dlsym(dl_handle, "test_add");

        if ((error = dlerror()) != NULL)
            errx(1, "%s\n", error);
        test_function();
    }
    else if (pid == -1) {
        errx(1, "Error");
    }
    else {
        waitpid(pid, NULL, 0);
    }
}

int main() {
    char        lib[50];
    int         i    = 0;
    const char *text = values;
    while (*text != '\0') {
        i = 0;
        for (; *text != ' ' && *text != '\0'; text++)
            lib[i++] = *text;
        if (*text == ' ')
            text++;
        lib[i] = 0;

        printf("Running tests for %s\n", lib);
        execute_tests(lib);
        printf("Ran !\n");
    }
}

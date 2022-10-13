#include <dlfcn.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define xstr(s) str(s)
#define str(s) #s
const char *values = xstr(TEST_LIBS);

typedef void (*TestFunction)(void);

int execute_test(TestFunction fn) {
    pid_t pid = fork();
    if (pid == 0) {
        fn();

        exit(0);
    }
    else if (pid == -1) {
        errx(1, "Could not fork process");
    }
    // If got to this point,this is the original process

    int status = -1;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

int execute_tests(const char *path) {
    pid_t pid = fork();
    if (pid == 0) {
        char *error = NULL;

        void *dl_handle = dlopen(path, RTLD_LAZY);
        if (dl_handle == NULL)
            errx(1, "%s\n", dlerror());
        dlerror();

        char **test_functions = dlsym(dl_handle, "test_functions");
        if ((error = dlerror()) != NULL)
            errx(1, "%s", error);

        for (char **counter = test_functions; *counter != 0; counter++) {
            printf("Running %s\n", *counter);

            TestFunction tst_fun = (TestFunction)dlsym(dl_handle, *counter);
            if ((error = dlerror()) != NULL)
                errx(1, "%s", error);
            int result;
            if ((result = execute_test(tst_fun)) != 0)
                exit(result);
        }

        exit(0);
    }
    else if (pid == -1) {
        errx(1, "Could not fork process");
    }
    // If got to this point,this is the original process

    int status = -1;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
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
        int result = execute_tests(lib);
        if (result != 0) {
            printf("Test failed\n");
        }
    }
}

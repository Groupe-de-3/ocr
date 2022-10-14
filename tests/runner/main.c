#include <assert.h>
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
    // If got to this point, this is the parent process.

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
            errx(1, "Invalid test lib file: %s\n", dlerror());
        dlerror();

        // Loading the array of test functions (asumed valid).
        char **test_functions = dlsym(dl_handle, "test_functions");
        if ((error = dlerror()) != NULL)
            errx(1, "Invalid test lib file: %s", error);

        int result = 0;
        // Iteration through the array.
        for (char **counter = test_functions; *counter != 0; counter++) {
            printf("  '%s' > ", *counter);
            fflush(stdout);

            TestFunction tst_fun = (TestFunction)dlsym(dl_handle, *counter);
            if ((error = dlerror()) != NULL)
                errx(1, "Invalid test lib file: %s", error);
            if (execute_test(tst_fun) != 0) {
                result = 1;
                printf("Failed\n");
            }
            else {
                printf("Passed!\n");
            }
        }

        exit(result);
    }
    else if (pid == -1) {
        errx(1, "Could not fork process");
    }
    // If got to this point,this is the original process.

    int status = -1;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

int main() {
    // Buffer for each test file names. (with i as counter into it)
    char        lib[100];
    int         i    = 0;
    const char *text = values;

    // Iterating through each space-separated list of test files.
    while (*text != '\0') {
        i = 0;
        // Copy the current word until eol or a space is hit.
        for (; *text != ' ' && *text != '\0' && i < 100; text++)
            lib[i++] = *text;
        assert(i < 100); // Assert not any file was larget than 100 characters
        if (*text == ' ')
            text++;
        lib[i] = 0;

        printf("Running tests for %s\n", lib);
        int result = execute_tests(lib);
        if (result != 0)
            printf("Test failed\n");
        else
            printf("Success !\n");
    }
}

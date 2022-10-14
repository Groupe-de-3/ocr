#include <assert.h>
#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define xstr(s) str(s)
#define str(s) #s
const char *values = xstr(TEST_LIBS);

typedef void (*TestFunction)(void);

/*! \brief Prints to stdout everything available without blocking from the
 *  given file descriptor.
 *
 *  \param df The file descriptor to read from.
 */
static void print_fd(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
    while (1) {
        char    output[128];
        ssize_t read_size = read(fd, output, sizeof(output));
        if (read_size <= 0)
            break;
        output[read_size] = 0;
        printf("%s", output);
    }
}

/*! \brief Executes the given function into a subprocess.
 *
 *  \param[in]  test_function Function that will be run un a subprocess.
 *  \param[out] output_fd If not NULL, set to a file descriptor of the output
 *                         of the subprocess. (To be printed with #print_df)
 *  \returns -1 if an error occured, 0 otherwise.
 */
static int execute_test(TestFunction test_function, int *output_fd) {
    int error;

    int out_pipe[2];
    error = pipe(out_pipe);
    if (error == -1)
        errx(1, "Could not create pipe: %s", strerror(errno));

    pid_t pid = fork();
    if (pid == 0) {
        // Captures the fork's stdout (0) and stderr (1)
        // into the write end of the pipe.
        dup2(out_pipe[1], 0);
        dup2(out_pipe[1], 1);

        test_function();

        // Be sure the outputs have been flushed
        fflush(stdout);
        fflush(stderr);
        // Close this end of the pipe
        close(out_pipe[1]);
        exit(0);
    }
    else if (pid == -1) {
        errx(1, "Could not fork process");
    }
    // If got to this point, this is the parent process.

    int status = -1;
    waitpid(pid, &status, 0);
    if (output_fd != NULL)
        *output_fd = out_pipe[0];
    return WEXITSTATUS(status);
}

int execute_tests(const char *path) {
    char *error = NULL;

    void *dl_handle = dlopen(path, RTLD_LAZY);
    if (dl_handle == NULL)
        errx(1, "Invalid test lib file: %s", dlerror());
    dlerror();

    // Loading the array of test functions (asumed valid).
    char **test_functions = dlsym(dl_handle, "test_functions");
    if ((error = dlerror()) != NULL)
        errx(1, "Invalid test lib file: %s", error);

    // Changed if any test fails
    int result = 0;
    // Iteration through the array.
    for (char **counter = test_functions; *counter != 0; counter++) {
        printf("  '%s' > ", *counter);
        fflush(stdout);

        TestFunction tst_fun = (TestFunction)dlsym(dl_handle, *counter);
        if ((error = dlerror()) != NULL)
            errx(1, "Invalid test lib file: %s", error);

        int rs_fd;
        if (execute_test(tst_fun, &rs_fd) != 0) {
            result = 1;
            printf("Failed:\n");
            printf("------------- Program's output:\n");
            print_fd(rs_fd);
            printf("-------------------------------\n\n");
        }
        else {
            printf("Passed!\n");
        }
        close(rs_fd);
    }

    return result;
}

int main(int argc, char **argv) {
    for (int argi = 1; argi < argc; argi++) {
        printf("Running tests for %s\n", argv[argi]);
        int result = execute_tests(argv[argi]);
        if (result != 0)
            printf("Some tests for '%s' failed :(\n", argv[argi]);
        else
            printf("Success !\n");
    }
}

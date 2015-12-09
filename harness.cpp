#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "defs.h"

using namespace std;

int main(int argc, char** argv) {

    // Support '-l path' for non-standard libmwrap locations
    string preloadlib((getopt(argc, argv, "l:") != -1) ? optarg : LIBMWRAPPATH);
    
    // Create a pipe so that the hooks can communicate back to the harness
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(1);
    }
    
    // Launch the program under test
    if (fork() == 0) {
        // Open a known file descriptor for the test hooks to write
        if (dup2(pipefd[1], MWRAPCOMMFD) != MWRAPCOMMFD) {
            perror("mwrap-child: dup2:");
            exit(1);
        }
        close(pipefd[0]); close(pipefd[1]);

        // Set up the environment and exec
        setenv("LD_PRELOAD", preloadlib.c_str(), 1);
        execvp(argv[optind], &argv[optind]);
        perror(argv[optind]);
        exit(1);
    }
    
    // Close the write end of the pipe on the harness
    close(pipefd[1]);
    
    // TODO - replace with pipe-read loop and handle for SIGSEGV
    char *buf = (char*) calloc(1000, sizeof(char));
    read(pipefd[0], buf, 1000);
    printf("Harness got: %s", buf);
    int status;
    wait(&status);
}

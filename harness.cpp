#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "defs.h"
#include "harnesshooks.h"

using namespace std;

int main(int argc, char** argv) {

    // Support '-l path' for non-standard libmwrap locations
    //string preloadlib((getopt(argc, argv, "l:") != -1) ? optarg : LIBMWRAPPATH);
    string preloadlib(LIBMWRAPPATH);
    
    // Allow for profiling data structure initialization
    harness_init();
    
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
    
    // Read messages from the test hooks until the pipe is closed
    char *buf = (char*) calloc(MWRAPCOMMBUFSIZE, sizeof(char));
    while (read(pipefd[0], buf, MWRAPCOMMBUFSIZE) != 0) {
        harness_recv_event(buf);
    }
    
    // Allow for clean up and final statistics
    harness_finalize();
}

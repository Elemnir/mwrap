#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
    int c;
    string prelib = (c = getopt(argc, argv, "l:") != -1) ? optarg : "./libmwrap.so";
    if (fork() == 0) {
        setenv("LD_PRELOAD", prelib.c_str(), 1);
        execvp(argv[optind], &argv[optind]);
        perror(argv[optind]);
        exit(1);
    }

    int status;
    wait(&status);
}

#include "harnesshooks.h"

#include <map>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>

using namespace std;



void harness_init() {
}

void harness_recv_event(const char* buf) {
    static vector<string> trace;

    if (!strcmp(buf, "AllocStart\n")) {
        trace.clear();
        printf("Start Allocation\n");
    } else if (!strcmp(buf, "AllocEnd\n")) {
        printf("End Allocation\n");
    } else if (!strncmp(buf, "Size:", 5)) {
        printf("Size Message: %s", buf);
    } else if (!strncmp(buf, "Ptr:", 4)) {
        printf("Ptr Message: %s", buf);
    } else {
        printf("Other: %s", buf);
    }
}

void harness_finalize() {
}

#include "harnesshooks.h"

#include <map>
#include <stdio.h>
#include <string>
#include <string.h>
#include <utility>
#include <vector>

using namespace std;

struct CallSiteData {
    CallSiteData() : total(0) {}
    
    // The amount of data the callsite accrued
    size_t total;

    // The list of returned pointers and their sizes given to this callsite
    vector<pair<void*, size_t> > entries;

};

map<string, CallSiteData> callsites;

void harness_init() {
}

void harness_recv_event(const char* buf) {
    static vector<string> trace;
    static size_t amt, t_index;
    static void *ptr;

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

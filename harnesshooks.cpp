#include "harnesshooks.h"

#include <iostream>
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
        //printf("Start Allocation\n");
    } else if (!strcmp(buf, "AllocEnd\n")) {
        //printf("End Allocation\n");
        callsites[trace[2]].total += amt;
        callsites[trace[2]].entries.push_back(make_pair(ptr, amt));
    } else if (!strncmp(buf, "Size:", 5)) {
        //printf("Size Message: %s", buf);
        sscanf(buf, "Size: %d", &amt);
    } else if (!strncmp(buf, "Ptr:", 4)) {
        //printf("Ptr Message: %s", buf);
        sscanf(buf, "Ptr: %li", &ptr);
    } else {
        //printf("Other: %s", buf);
        trace.push_back(buf);
    }
}

void harness_finalize() {
    for (auto ci = callsites.begin(); ci != callsites.end(); ci++) {
        cerr << "Callsite: " << ci->first;
        cerr << "Total: " << ci->second.total << endl;
        cerr << "Blocks:\n";
        for (auto i = 0; i < ci->second.entries.size(); i++) {
            cerr << "    " << ci->second.entries[i].first << " "
                 << ci->second.entries[i].second << endl;
        }
    }
}

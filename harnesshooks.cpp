#include "harnesshooks.h"

#include <stdio.h>

void harness_init() {
}

void harness_recv_event(const char* buf) {
    printf("Harness got %s", buf);
}

void harness_finalize() {
}

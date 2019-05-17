#include <stdio.h>
#include <libmill.h>

coroutine void worker(int count, const char *text) {
    int i;
    for(i = 0; i != count; ++i) {
        printf("This is the %dth occurence of %s\n", i+1, text);
        msleep(now() + 10);
    }
}

int main() {
    go(worker(10, "a"));
    go(worker(10, "b"));
    go(worker(10, "c"));
    msleep(now() + 100);
    return 0;
}
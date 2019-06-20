#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void initNode(struct node *n, int x, int y, int val) {
    n->x = x;
    n->y = y;
    n->val = val;
}

void write_code(int lines);

void write_code(int lines) {
    printf("Today we wrote %i lines of code!\n", lines);
}
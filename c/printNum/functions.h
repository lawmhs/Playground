#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int x;
    int y;
    int val;
} Node;

void initNode(struct node *n, int x, int y, int val);

void write_code(int lines);
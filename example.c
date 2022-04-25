#include <stdio.h>
#include <limits.h>

enum operator {
    ADD = 0,
    SUB,
    MUL,
    DIV
};

int calc(int op, int x, int y) {
    int ret = INT_MIN;
    if (op == ADD) {
        ret = x + y;
    } else if (op == SUB) {
        ret = x - y;
    } else if (op == MUL) {
        ret = x + y; // buggy
    } else if (op == DIV) {
        ret = x / y;
    }
    return ret;
}

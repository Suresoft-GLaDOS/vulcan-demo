#include <stdio.h>
#include <limits.h>

enum operator {
    ADD = 0,
    SUB,
    MUL,
    DIV
};

int add(x, y) { return x + y;}
int sub(x, y) { return x - y;}
int mul(x, y) { return x * y;}
int div(x, y) { return x / y;}

int calc(int op, int x, int y) {
    int ret = INT_MIN;
    if (op == ADD) {
        ret = add(x, y);
    } else if (op == SUB) {
        ret = sub(x, y);
    } else if (op == MUL) {
        ret = add(x, y);
    } else if (op == DIV) {
        ret = div(x, y);
    }
    return ret;
}

#include <stdio.h>
#include <stdlib.h>
#include "example.h"

#define TEST_SIZE 10

int input[TEST_SIZE] = {
  2, 3, 4, 5, 6, 7, 8, 10, 15, 20,
};

int expected_min[TEST_SIZE] = {
  1, 7, 4, 2, 6, 8, 10, 22, 108, 688,
};

int expected_max[TEST_SIZE] = {
  1, 7, 11, 71, 111, 711, 1111, 11111, 7111111, 1111111111
};

int main(int argc, char *argv[]) {
    int test_index = atoi(argv[1]);
    int min = get_min(input[test_index]);
    int max = get_max(input[test_index]);
    printf("input = %d\n", input[test_index]);
    printf("actual min: %d,\n expected min: %d\n", min, expected_min[test_index]);
    printf("actual max: %d,\n expected max: %d\n", max, expected_max[test_index]);
    if (min == expected_min[test_index] &&
    max == expected_max[test_index] ) {
        printf("PASSED\n");
        return 0;
    } else {
        printf("FAILED\n");
        return 1;
    }
}
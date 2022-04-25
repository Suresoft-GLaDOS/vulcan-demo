#include <stdio.h>
#include <stdlib.h>
#include "example.h"

#define TEST_SIZE

int input[TEST_SIZE][4] = {
  {0, 1, 2},  // 3
  {0, 3, 3},  // 6
  {0, -5, 5}, // 0
  {1, 4, 5},  // -1
  {1, 2, 2},  // 0
  {1, 4, 2},  // 2
  {2, 2, 2},  // 4
  {2, 2, 3},  // 6
  {2, 5, 5},  // 25
  {3, 3, 2},  // 1
  {3, 4, 2},  // 2
};

int expected_output[TEST_SIZE] = {
  3, 6, 0, -1, 0, 2, 4, 6, 25, 1, 2
};

int main(int argc, char *argv[]) {
    int test_index = atoi(argv[1]);
    int actual_output = calc(input[test_index][0], input[test_index][1], input[test_index][2]);
    printf("actual %d, expected %d\n", actual_output, expected_output[test_index]);
    if (actual_output == expected_output[test_index]) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    return actual_output == expected_output[test_index] ? 0 : 1;
}
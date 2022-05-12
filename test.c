#include <stdio.h>
#include <stdlib.h>
#include "example.h"

#define TEST_SIZE 10

int input[TEST_SIZE] = {
  2, 3, 4, 5, 6, 7, 8, 10, 15, 20,
};

int expected_output[TEST_SIZE] = {
  2, 14, 15, 73, 117, 719, 1121, 11133, 7111219, 1111111799
};

int main(int argc, char *argv[]) {
    int test_index = atoi(argv[1]);
    int actual_output = func(input[test_index]);
    printf("actual %d, expected %d\n", actual_output, expected_output[test_index]);
    if (actual_output == expected_output[test_index]) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    return actual_output == expected_output[test_index] ? 0 : 1;
}
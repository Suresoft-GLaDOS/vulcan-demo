#include <stdlib.h>
#include "example.h"

#define TEST_SIZE

int input[TEST_SIZE][4] = {
  {3, 3, 5},
  {1, 2, 3},
  {3, 2, 1},
  {5, 5, 5},
  {5, 3, 4},
  {2, 1, 3}
};

int expected_output[TEST_SIZE] = {
  3, 2, 1, 5, 4, 2
};

int main(int argc, char *argv[]) {
    int test_index = atoi(argv[1]);
    int actual_output = mid(input[test_index][0], input[test_index][1], input[test_index][2]);
    return actual_output == expected_output[test_index] ? 0 : 1;
}
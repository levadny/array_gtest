#include <iostream>
#include "array.h"

// main func
int main() {
  std::cout << "Test array" << std::endl;
  // simple create array without goal
  Array<int> testArray(10, 1);
  // check all google tests
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}

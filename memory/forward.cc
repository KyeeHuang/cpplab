#include <iostream>

void test(int & a) {
  std::cout << "int &" << std::endl; 
}

void test(int* a) {
  std::cout << "int *" << std::endl; 
}

void test(int && a) {
  std::cout << "int &&" << std::endl; 
}

int main() {
  int p = 0;
  test(std::forward<int>(p));
  return 0;
}
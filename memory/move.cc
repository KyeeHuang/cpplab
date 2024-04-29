#include <iostream>

void test(const int & a) {
  std::cout << "const int &" << std::endl; 
}

void test(const int && a) {
  std::cout << "const int &&" << std::endl; 
}

void test(int & a) {
  std::cout << "int &" << std::endl; 
}

void test(int && a) {
  std::cout << "int &&" << std::endl; 
}

int main() {
  int p = 0;
  test(std::move(&p));
  return 0;
}
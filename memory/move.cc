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

// std::move(&p) 地址的右值引用，即 指针
void test(int * a) {
  std::cout << "int *" << std::endl;
}

int main() {
  int p = 0;
  test(std::move(&p));
  return 0;
}
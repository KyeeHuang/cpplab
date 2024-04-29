#include <iostream>

class A {
public:
  A() {
    std::cout << "A" << std::endl;
  }

  ~A() {
    std::cout << "~A" << std::endl;
  }
};

class B : public A {
public:
  B() {
    std::cout << "B" << std::endl;
  }

  ~B() {
    std::cout << "~B" << std::endl;
  }
};

int main() {
  B* b = new B();
  delete b;
  return 0;  
}
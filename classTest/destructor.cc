#include <iostream>

class A {
public:
  A(int a = 5) {
    std::cout << "A" << a << std::endl;
  }

  ~A() {
    std::cout << "~A" << std::endl;
  }
};

class B : public A {
public:
  B(int b = 10) {
    std::cout << "B" << b << std::endl;
  }

  ~B() {
    std::cout << "~B" << std::endl;
  }
};

int main() {
  B* b = new B();
  A* a = dynamic_cast<A*>(b); // const_cast is not allowed
  delete a;
  return 0; 
}
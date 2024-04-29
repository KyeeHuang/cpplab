#include <iostream>

class A {
public:
  int key;
  void get() {
    std::cout << "get" << std::endl;    
  }
  
  int getThis() {
    
  }
};

int main() {
  A* p = nullptr;
  p->getThis();
  return 0;
}
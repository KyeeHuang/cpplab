#include <iostream>
#include <memory>
#include <cassert>

class IntNum {
public:
  IntNum(int x = 0) : xptr(new int(x)) {
    std::cout << "Calling constructor..." << std::endl;
  }

  // copy constructor without defining move constructor
  IntNum(const IntNum& n) : xptr(new int(*n.xptr)) {
    std::cout << "Calling copy constructor..." << std::endl;
  }

  // move constructor
  IntNum(IntNum&& n) : xptr(new int(*n.xptr)) {
    std::cout << "Calling move constructor..." << std::endl;
  }

  ~IntNum() {
    delete xptr;
    std::cout << "Destructing..." << std::endl; 
  }

  int getInt() { return *xptr; }

private:
  int *xptr;
};

IntNum getNum() {
  IntNum a;
  return a; // use move constructor if defined
}

// add -fno-elide-constructor to avoid compilor optimize
int main() {
  std::cout << getNum().getInt() << std::endl;
  assert(1, 1);
  return 0;
}
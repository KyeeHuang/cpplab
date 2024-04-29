#include <iostream>

class A {
private:
  static int s_var;
  const int c_var;
  int var;
  char c;
public:
  A(int tmp) : c_var(tmp) {}
  ~A() {}
  
  virtual void gc() {
    std::cout << c_var << std::endl;
  }

  virtual void gv() {
    std::cout << var << std::endl;
  }

  void gch() {}
  void gcc() {}
  void gcm() {}
};

int main (int argc, char** argv) {
  A *p = new A(4);
  A a(4);

  std::cout << sizeof(p) << std::endl;
  std::cout << sizeof(a) << std::endl;

  return 0;
}
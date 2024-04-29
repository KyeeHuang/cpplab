#include <iostream>

using namespace std;

class A {
public:
  A() {
  }

  virtual void func() {
    cout << "A" << endl;
  }

  virtual ~A() {};
};

class B {
private:
  B() {}
  ~B() {}

  B(const B&) = delete;
  B& operator=(const B&) = delete;

public:
  static B* create() {
    return new B();
  }

  static void destory(B* b) {
    delete b;
  }
};

class StackOnly {
public:
  StackOnly() {}
  ~StackOnly() {}

private:
  void* operator new (size_t size) noexcept {
    return (void *) 0;
  }

  void operator delete (void* obj) {
  }
  
};

int main() {
  StackOnly* stk = new StackOnly();
  delete stk;
  
  return 0;
}
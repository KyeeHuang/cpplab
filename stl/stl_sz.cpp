#include <iostream>
#include <memory>
#include <vector>

class A {
private:
  int a, b, c, d;
public:
  A() {}
  ~A() {}
};

int main(int argc, char** argv) {
  // int n = sizeof(std::string);
  std::vector<int> v(1024, 0);
  v.clear();
  v.shrink_to_fit();

  std::shared_ptr<A> p = std::make_shared<A>();
  
  // std::cout << v.capacity() << std::endl;
  std::cout << sizeof(A) << std::endl;
  std::cout << sizeof(p) << std::endl;

  return 0;
}
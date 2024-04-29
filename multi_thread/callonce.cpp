#include <mutex>
#include <iostream>
#include <thread>
#include <pthread.h>

std::once_flag flag;

void init_static() {
  static int counter = 0;
  std::cout << "Init Static Value once" << std::endl;

  std::call_once(flag, []{
    std::cout << "Init Static Value once" << std::endl;
    counter++;
  });
}

int main() {
  std::thread t1(init_static);
  std::thread t2(init_static);
  t1.join();
  t2.join();

  return 0; 
}
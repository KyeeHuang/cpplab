#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

std::atomic<int> a{0};
std::atomic<int> b{0};
std::mutex mtx;

void thread1() {
  for (int i = 0; i < 10; i += 2)
    a.store(i, std::memory_order_relaxed);
}

void thread2() {
  for (int i = 1; i < 10; i += 2)
    a.store(i, std::memory_order_relaxed);
}

void thread3(std::vector<int> *v) {
  for (int i = 0; i < 10; i++)
    v->push_back(a.load(std::memory_order_relaxed));
}

void thread4(std::vector<int> *v) {
  for (int i = 0; i < 10; i++)
    v->push_back(a.load(std::memory_order_relaxed));
}

void thread5() {
  b += 1;
  mtx.unlock();
}

void thread6() {
  mtx.lock();
  std::cout << b << std::endl;
}


int main() {
  std::vector<int> v3, v4;
  std::thread t1(thread1), t2(thread2), t3(thread3, &v3), t4(thread4, &v4);
  t1.join(), t2.join(), t3.join(), t4.join();
  std::thread t5(thread5), t6(thread6);
  t5.join(), t6.join();
  
  // for (int i : v3) std::cout << i << " ";
  // std::cout << std::endl;
  // for (int i : v4) std::cout << i << " ";
  // std::cout << std::endl;
  return 0;
}
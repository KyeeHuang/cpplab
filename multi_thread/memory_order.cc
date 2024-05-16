#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <assert.h>

std::atomic<bool> x{false}, y{false};

void thread1() {
  x.store(true, std::memory_order_seq_cst); // (1)
}

void thread2() {
  y.store(true, std::memory_order_seq_cst); // (2)
}

std::atomic<int> z{0};

void read_x_then_y() {
  while (!x.load(std::memory_order_seq_cst)); // (3)
  if (y.load(std::memory_order_seq_cst)) ++z; // (4)
}

void read_y_then_x() {
  while (!y.load(std::memory_order_seq_cst)); // (5)
  if (x.load(std::memory_order_seq_cst)) ++z; // (6)
}

int main() {
  std::thread a(thread1), b(thread2), c(read_x_then_y), d(read_y_then_x);
  a.join(), b.join(), c.join(), d.join();
  assert(z.load() == 0); // (7) 
}
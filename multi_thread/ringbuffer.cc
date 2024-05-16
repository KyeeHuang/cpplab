#include <iostream>
#include <atomic>

template <typename T, size_t Cap>
class ring_buffer {
  T data[Cap];
  std::atomic<size_t> head{0}, tail{0}, write{0};

public:
  ring_buffer() = default;
  ring_buffer(const ring_buffer&) = delete;
  ring_buffer &operator=(const ring_buffer&) = delete;
  ring_buffer &operator=(const ring_buffer&) volatile = delete;
  
  bool push(const T &val) {
    size_t t, w;
    do {
      t = tail.load();
      if ((t+1) % Cap == head.load())
        return false;
    } while (!tail.compare_exchange_weak(t, (t + 1) % Cap));
    data[t] = val;
    do {
      w = t;
    } while (!write.compare_exchange_weak(w, (w + 1) % Cap));
    return true;
  }

  bool pop(T &val) {
    size_t h;
    do {
      h = head.load();
      if (h == write.load())
        return false;
      val = data[h];
    } while (!head.compare_exchange_strong(h, (h + 1) % Cap));
    return true;
  }
  
};

// compare_exchange_strong
template <typename T>
bool _compare_exchange_strong(T &expected, T desired) {
  std::lock_guard<std::mutex> guard(m_lock);
  if (m_val == expected) 
    return m_val = desired, true;
  else
    return expected = m_val, false;
}

// actual in funciton
int foo(std::atomic<int> & a) {
  int e = 42;
  a.compare_exchange_strong(e, e+1);
  return a.load();
}

/* x86-64 -O2 compile:
foo (std::atomic<int>&):
  movl $42, %eax
  movl $43, %edx
  lock cmpxchgl %edx, (%rdi) # %rdi is the first param in function
  movl (%rdi), %eax
  ret
*/

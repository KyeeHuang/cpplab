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
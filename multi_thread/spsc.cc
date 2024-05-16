#include <atomic>
#include <memory>

template <typename T, std::size_t Cap>
class spsc : private std::allocator<T> {
  T *data;
  std::atomic<std::size_t> head{0}, tail{0};
public:
  spsc() : data(std::allocator<T>::allocate(Cap)) {}
  spsc(const spsc&) = delete;
  spsc &operator=(const spsc&) = delete;
  spsc &operator=(const spsc&) volatile = delete;
  
  bool push(const T &val) {
    return emplace(val);
  }

  bool push(T &&val) {
    return emplace(std::move(val));
  }

  template <typename ...Args>
  bool emplace(Args && ...args) { // insert queue
    std::size_t t = tail.load(std::memory_order_relaxed);
    if ((t+1) % Cap == head.load(std::memory_order_acquire))
      return false;
    std::allocator<T>::construct(data + t, std::forward<Args>(args)...);
    
    tail.store((t+1) % Cap, std::memory_order_release);
    return true;
  }

  bool pop(T &val) {
    std::size_t h = head.load(std::memory_order_relaxed);
    if (h == tail.load(std::memory_order_acquire))
      return false;
    val = std::move(data[h]);
    std::allocator<T>::destroy(data + h);

    head.store((h+1) % Cap, std::memory_order_release);
    return true;
  }
};
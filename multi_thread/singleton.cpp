#include <memory>
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>

// unsafe
class Singleton {
private:
  static std::shared_ptr<Singleton> instance;  
  Singleton() = default; 
  Singleton(const Singleton&) = delete;
  Singleton& operator= (const Singleton&) = delete;
  static std::mutex mutex_;

public:
  static std::shared_ptr<Singleton> get_instance() {
    if (instance == nullptr) { // instance is unsafe
      std::lock_guard<std::mutex> lock(mutex_);
      instance = std::shared_ptr<Singleton>{new Singleton()};
    } 
    return instance;
  }
};
std::shared_ptr<Singleton> Singleton::instance = nullptr;
std::mutex Singleton::mutex_;

// safe
class Safe_Singleton {
private:
  static std::atomic<Safe_Singleton*> instance;
  static std::mutex mutex_;
  Safe_Singleton() {}
  Safe_Singleton(const Safe_Singleton&) = delete;
  Safe_Singleton& operator = (const Safe_Singleton&) = delete;

public:
  static Safe_Singleton* get_instance() {
    Safe_Singleton* tmp = instance.load(); // atomic operator
    if (tmp == nullptr) {
      std::lock_guard<std::mutex> lock(mutex_);
      tmp = instance.load();
      if (tmp == nullptr) {
        tmp = new Safe_Singleton();
        instance.store(tmp);
        return tmp;
      }
    }

    return tmp;
  }

  ~Safe_Singleton() {}
};

// safe c++11 local static lazy 
class Safe_lazy_Singleton {
private:
  Safe_lazy_Singleton() {} 
  Safe_lazy_Singleton(const Safe_lazy_Singleton&) = delete;
  Safe_lazy_Singleton& operator=(const Safe_Singleton&) = delete;
  
public:
  static Safe_lazy_Singleton* get_instance() {
    static Safe_lazy_Singleton* instance = new Safe_lazy_Singleton();
    return instance;
  }

  ~Safe_lazy_Singleton() {}
};

// safe c++11 local static eager 
class Safe_Eager_Singleton {
private:
  static Safe_Eager_Singleton* instance;
  Safe_Eager_Singleton() {} 
  Safe_Eager_Singleton(const Safe_Eager_Singleton&) = delete;
  Safe_Eager_Singleton& operator=(const Safe_Eager_Singleton&) = delete;
  
public:
  static Safe_Eager_Singleton* get_instance() {
    return instance;
  }

  ~Safe_Eager_Singleton() {}
};

Safe_Eager_Singleton* Safe_Eager_Singleton::instance = new Safe_Eager_Singleton();
std::atomic<Safe_Singleton*> Safe_Singleton::instance;
std::mutex Safe_Singleton::mutex_;

int main() {
  auto single = Singleton::get_instance();
  auto single2 = Singleton::get_instance();
  assert(single.get() == single2.get());
   
  return 0;
}
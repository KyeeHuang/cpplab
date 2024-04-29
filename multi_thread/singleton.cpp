#include <memory>
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>

class Singleton {
private:
  static std::shared_ptr<Singleton> instance;  
  Singleton() = default; 
  Singleton(const Singleton&) = delete;
  Singleton& operator= (const Singleton&) = delete;
  static std::mutex mutex_;

public:
  static std::shared_ptr<Singleton> get_instance() {
    if (instance == nullptr) {
      std::lock_guard<std::mutex> lock(mutex_);
      instance = std::shared_ptr<Singleton>{new Singleton()};
    } 
    return instance;
  }
};
std::shared_ptr<Singleton> Singleton::instance = nullptr;
std::mutex Singleton::mutex_;

class Safe_Singleton {
private:
  static std::atomic<Safe_Singleton*> instance;
  static std::mutex mutex_;
  Safe_Singleton() {}
  Safe_Singleton(const Safe_Singleton&) = delete;
  Safe_Singleton& operator = (const Safe_Singleton&) = delete;

public:
  static Safe_Singleton* get_instance() {
    Safe_Singleton* tmp = instance.load();
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

std::atomic<Safe_Singleton*> Safe_Singleton::instance;
std::mutex Safe_Singleton::mutex_;

int main() {
  auto single = Singleton::get_instance();
  auto single2 = Singleton::get_instance();
  assert(single.get() == single2.get());
   
  return 0;
}
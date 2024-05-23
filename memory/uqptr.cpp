#include <cstddef>
#include <utility>
#include <stddef.h>

template<typename T>
class Unique_ptr {
public:
    constexpr Unique_ptr() noexcept = default;
    constexpr Unique_ptr(std::nullptr_t) noexcept : Unique_ptr() {}
    explicit Unique_ptr(T *ptr) noexcept : ptr_{ptr} {}
    Unique_ptr(Unique_ptr &&rhx) noexcept : ptr_{rhx.release()} {}

    ~Unique_ptr() noexcept {
        delete ptr_;
    }

    Unique_ptr &operator=(const Unique_ptr &) = delete;
    constexpr Unique_ptr &operator=(std::nullptr_t) {
        this->reset();
        return *this;
    }

    Unique_ptr &operator=(Unique_ptr &&rhx) noexcept {
        this->reset(rhx.release());
        return *this;
    }

    T* release() noexcept {
        return std::exchange(ptr_, nullptr);
    }

    T* get() const noexcept {
        return ptr_;
    }

    void reset(T *ptr) noexcept {
        delete std::exchange(ptr_, ptr);
    }

    void swap(Unique_ptr &rhx) noexcept {
        swap(ptr_, rhx.ptr_);
    }

    T &operator*() const {
        return *ptr_;
    }
    
    T *operator->() const noexcept {
        return ptr_;
    }

    operator bool() const noexcept {
        return static_cast<bool>(ptr_);
    }

private:
    T *ptr_{nullptr};

};

template<typename T, typename... Args>
auto make_unique(Args&& ...args) {
    return Unique_ptr<T>(new T(std::forward(args) ...));
}

#include <vector>

int main() {
    Unique_ptr<std::vector<int>> ptr = make_unique<std::vector<int>>();
    return 0;
}

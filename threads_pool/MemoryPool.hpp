#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

template<typename T>
class SafeQueue {
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;

public:
    SafeQueue() {}
    SafeQueue(SafeQueue &&other) {}
    ~SafeQueue() {}

    bool empty() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    int size() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    void enqueue(T &t) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(t);
    }

    bool dequeue(T &t) {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        if (m_queue.empty()) {
            return false;
        }
        t = std::move(m_queue.front());

        m_queue.pop();

        return true;
    }
};

template<typename F, typename... Args>
auto submit(F &&f, Args && ... args) -> std::future<decltype(f(args...))> {
    std::function<decltype(f(args...))()> func = std::bind(std::forword<F>, std::forward<Args>(args)...);
    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))>>(func);

    std::function<void()> warpper_func = [task_ptr]() {
        (*task_ptr)();
    };

    m_queue.enqueue(warpper_func);
    m_conditional_lock.notify_one();
    
    return task_ptr->get_future();
}

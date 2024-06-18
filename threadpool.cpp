#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
using namespace std;

template <typename T>
class ThreadPool {
 public:
    explicit ThreadPool(size_t number = 5) : m_stop(false) {
        for (int i = 0; i < number; ++i) {
            m_threads.emplace_back([this]() {
                unique_lock<mutex> locker(m_mtx);
                while (true) {
                    m_cond.wait(locker, [&]() { return m_stop || !m_tasks.empty(); });
                    if (!m_tasks.empty()) {
                        auto task = std::move(m_tasks.front());
                        m_tasks.pop();
                        locker.unlock();
                        task();
                        locker.lock();
                    }
                    else if (m_stop) {
                        break;
                    }
                }
            });
        }
    }

    ~ThreadPool() {
        {
            lock_guard<mutex> locker(m_mtx);
            m_stop = true;
        }
        m_cond.notify_all();

        for (auto& t : m_threads) {
            t.join();
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void addTask(T&& task) {
        {
            lock_guard<mutex> locker(m_mtx);
            m_tasks.emplace(std::forward<T>(task));
        }
        m_cond.notify_one();
    }

 private:
    std::vector<thread> m_threads;
    std::queue<T> m_tasks;
    bool m_stop;
    std::mutex m_mtx;
    std::condition_variable m_cond;
};

int main() {
    ThreadPool<function<void()>> pool;
    for (int i = 0; i < 10; ++i) {
        pool.addTask([i]() { printf("%d ", i); });
    }
    return 0;
}
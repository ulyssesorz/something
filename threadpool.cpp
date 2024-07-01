#include <pthread.h>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>
using namespace std;

class SingleThread {
 public:
    using FuncType = std::function<void()>;

    explicit SingleThread(const std::string& name) : m_stop(false) {
        //线程命名
        pthread_setname_np(pthread_self(), name.c_str());
        auto main_thread = ([this]() {
            unique_lock<mutex> locker(m_mtx);
            while (true) {
                while (!m_stop && m_tasks.empty()) {
                    m_cond.wait(locker);
                }
                if (!m_tasks.empty()) {
                    auto task = std::move(m_tasks.front());
                    m_tasks.pop();
                    locker.unlock();
                    task();
                    locker.lock();
                } else if (m_stop) {
                    break;
                }
            }
        });
        m_thread = std::thread(main_thread);
    }

    ~SingleThread() {
        {
            lock_guard<mutex> locker(m_mtx);
            m_stop = true;
        }
        m_cond.notify_one();
        m_thread.join();
    }

    SingleThread(const SingleThread&) = delete;
    SingleThread& operator=(const SingleThread&) = delete;

    void enqueue(FuncType&& task) {
        {
            lock_guard<mutex> locker(m_mtx);
            m_tasks.emplace(std::move(task));
        }
        m_cond.notify_one();
    }

 private:
    std::thread m_thread;
    std::queue<FuncType> m_tasks;
    bool m_stop;
    std::mutex m_mtx;
    std::condition_variable m_cond;
};

template <class K = std::string, class HashClass = std::hash<K>>
class ShardedThreadPool {
 public:
    using FuncType = std::function<void()>;
    ShardedThreadPool(size_t num = 10, const std::string& name = "shared_pool") : m_num(num) {
        m_threads.resize(m_num);
        for (int i = 0; i < m_num; ++i) {
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "F%d.%s", i, name.c_str());
            std::string thread_name = buffer;
            m_threads[i].reset(new SingleThread(thread_name));
        }
    }
    ~ShardedThreadPool() = default;
    
   //  需传入key做哈希
    void enqueue(const K& key, FuncType&& task) {
        size_t i = getThread(key);
        m_threads[i]->enqueue(std::move(task));
    }

 private:
    size_t getThread(const K& key) { return HashClass()(key) % m_num; }

 private:
    std::vector<shared_ptr<SingleThread>> m_threads;
    size_t m_num;
};

int main() {
   ShardedThreadPool<int> pool;
   for(int i = 0; i < 10; ++i)
   {
      pool.enqueue(i, [i](){
         printf("%d ", i);
      });
   } 
}
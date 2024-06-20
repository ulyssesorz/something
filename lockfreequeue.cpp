
#include <atomic>
#include <cstddef>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

template <typename T>
class LockFreeQueue {
 public:
    LockFreeQueue(size_t size) : m_data(new T[size]), m_size(size) {
        m_head.store(0);
        m_tail.store(0);
    }
    ~LockFreeQueue() { delete[] m_data; }

    LockFreeQueue(const LockFreeQueue &) = delete;
    LockFreeQueue &operator=(const LockFreeQueue &) = delete;

    bool push(const T &data) {
        size_t tail = m_tail.load(std::memory_order_relaxed);
        while (true) {
            size_t next_tail = (tail + 1) % m_size;
            if (next_tail == m_head.load(std::memory_order_acquire)) {
                return false;
            }
            if (m_tail.compare_exchange_weak(tail, next_tail)) {
                m_data[tail] = data;
                return true;
            }
        }
    }

    bool pop(T &data) {
        size_t head = m_head.load(std::memory_order_relaxed);
        while (true) {
            if (head == m_tail.load(std::memory_order_acquire)) {
                return false;
            }
            data = m_data[head];
            if (m_head.compare_exchange_weak(head, (head + 1) % m_size)) {
                return true;
            }
        }
    }

    bool empty() const { return m_head.load(std::memory_order_acquire) == m_tail.load(std::memory_order_acquire); }

 private:
    T *m_data;
    size_t m_size;
    atomic<size_t> m_head;
    atomic<size_t> m_tail;
};

//=================测试代码=================

const int kBufferSize = 1024;
const int kProducerCount = 2;
const int kConsumerCount = 3;
const int kTargetNum = 1000;

LockFreeQueue<int> g_buffer(kBufferSize);
std::atomic<bool> keep_run(true);  // 全局变量，用于控制生产者和消费者线程的运行
void Producer() {
    for (int i = 1; i <= kTargetNum; ++i) {
        while (!g_buffer.push(i))
            ;
    }
    keep_run.store(false, std::memory_order_release);
}

void Consumer(int &count) {
    int data = 0;
    while (true) {
        if (g_buffer.pop(data)) {
            count += data;
        }
        if (!keep_run.load(std::memory_order_acquire) && g_buffer.empty()) {
            break;
        }
    }
}

int main() {
    std::vector<std::thread> threads;
    std::vector<int> counts(kConsumerCount, 0);

    for (int i = 0; i < kProducerCount; ++i) {
        threads.emplace_back(Producer);
    }

    for (int i = 0; i < kConsumerCount; ++i) {
        threads.emplace_back(Consumer, ref(counts[i]));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    int sum = 0;
    for (const auto &count : counts) {
        sum += count;
    }
    for (auto &count : counts) {
        std::cout << "Count: " << count << std::endl;
    }
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
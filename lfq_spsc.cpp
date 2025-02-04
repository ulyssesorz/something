#include <bits/stdc++.h>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
using namespace std;

template<typename T>
class LockFreeQueue
{
public:
    LockFreeQueue(size_t n) : m_data(new T[n]), m_size(n), m_head(0), m_tail(0) {}
    ~LockFreeQueue() { delete[] m_data; }

    bool push(const T& data)
    {
        size_t cur_tail = m_tail.load(std::memory_order_relaxed);
        size_t next_tail = (cur_tail + 1) % m_size;
        if(m_head.load(std::memory_order_acquire) == next_tail)
            return false;
        m_data[cur_tail] = data;
        m_tail.store(next_tail, std::memory_order_release);
        return true;
    }
    bool pop(T& data)
    {
        size_t cur_head = m_head.load(std::memory_order_relaxed);
        if(cur_head == m_tail.load(std::memory_order_acquire))
            return false;
        data = m_data[cur_head];
        m_head.store((cur_head + 1) % m_size, std::memory_order_release);
        return true;
    }
private:
    T* m_data;
    size_t m_size;
    atomic<size_t> m_head;
    atomic<size_t> m_tail;
};

int main() {
    LockFreeQueue<int> lfq(10);
    lfq.push(123);
    int data;
    lfq.pop(data);
    cout << data << endl;

    return 0;
}

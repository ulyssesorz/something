#include <atomic>
#include <thread>

class SpinLock
{
public:
    SpinLock() : m_flag(ATOMIC_FLAG_INIT) {}
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    void lock()
    {
        while(m_flag.test_and_set(std::memory_order_acquire)) 
            ;
    }

    // 尝试获取锁，若没被上锁返回true
    bool try_lock()
    {
        return !m_flag.test_and_set(std::memory_order_acquire);
    }

    void unlock()
    {
        m_flag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag m_flag;
};
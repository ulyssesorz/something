#include <atomic>
#include <iostream>
using namespace std;

template<typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T* ptr) : m_ptr(ptr), m_count(new atomic<size_t>(1)) {cout << "ctor" << endl;}
    SharedPtr() : m_ptr(nullptr), m_count(nullptr) {}
    ~SharedPtr()
    {
        if(m_count && --(*m_count) == 0)
        {
            delete m_ptr;
            delete m_count;
        }
    }
    SharedPtr(const SharedPtr& rhs) : m_ptr(rhs.m_ptr), m_count(rhs.m_count)
    {
        cout << "copy ctor" << endl;
        if(m_count)
        {
            ++(*m_count);
        }
    }
    SharedPtr& operator=(const SharedPtr& rhs)
    {
        cout << "copy op" << endl;
        if(this != &rhs)
        {
            if(m_count && --(*m_count) == 0)
            {
                delete m_ptr;
                delete m_count;
            }
            m_ptr = rhs.m_ptr;
            m_count = rhs.m_count;
            ++(*m_count);
        }
        return *this;
    }

    SharedPtr(SharedPtr&& rhs) : m_ptr(rhs.m_ptr), m_count(rhs.m_count)
    {
        cout << "move ctor" << endl;
        rhs.m_ptr = nullptr;
        rhs.m_count = nullptr;
    }
    SharedPtr& operator=(SharedPtr&& rhs)
    {
        cout << "move op" << endl;
        if(this != &rhs)
        {
            if(m_count && --(*m_count) == 0)
            {
                delete m_ptr;
                delete m_count;
            }
            m_ptr = rhs.m_ptr;
            m_count = rhs.m_count;
            rhs.m_ptr = nullptr;
            rhs.m_count = nullptr;
        }
        return *this;  
    }
private:
    T* m_ptr;
    atomic<size_t>* m_count;
};

int main() {
    SharedPtr<int> sp(nullptr);
    SharedPtr<int> sp2(move(sp));
}
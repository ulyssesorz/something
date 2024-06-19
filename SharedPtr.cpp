#include <iostream>
using namespace std;

template<typename T>
class SharedPtr
{
public:
    SharedPtr(T* ptr) : m_ptr(ptr), m_count(new size_t(1)) {}

    ~SharedPtr()
    {
        cout << "dtor" << " count = " << *(m_count) << endl;
        if(--(*m_count) == 0)
        {
            delete m_ptr;
            delete m_count;
        }
    }

    SharedPtr(const SharedPtr& other) : m_ptr(other.m_ptr), m_count(other.m_count)
    {
        cout << "copy ctor" << endl;
        ++(*m_count);
    }

    SharedPtr& operator=(const SharedPtr& other)
    {
        cout << "copy operator" << endl;
        if(this != &other)
        {
            if(--(*m_count) == 0)
            {
                delete m_ptr;
                delete m_count;
            }
            m_ptr = other.m_ptr;
            m_count = other.m_count;
            ++(*m_count);
        }
        return *this;
    }

    T& operator*()
    {
        return *m_ptr;
    }

private:
    T* m_ptr;
    size_t* m_count;
};

int main()
{
    SharedPtr<int> sp(new int(1));
    SharedPtr<int> sp2(nullptr);
    sp2 = sp;
    cout << *sp2 << endl;
}
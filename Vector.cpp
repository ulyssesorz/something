#include <cassert>
#include <cstddef>
#include <iostream>
using namespace std;

template<typename T>
class Vector
{
public:
    explicit Vector(size_t n = 0) : m_capacity(n), m_size(0), m_data(new T[m_capacity]) {}
    ~Vector() { delete[] m_data; }

    Vector(const Vector& other) : m_capacity(other.m_capacity), m_size(other.m_size)
    {
        cout << "copy ctor" << endl;
        m_data = new T[m_capacity];
        for(int i = 0; i < other.m_size; ++i)
        {
            m_data[i] = other.m_data[i];
        }
    }
    Vector& operator=(const Vector& other)
    {
        cout << "copy operator" << endl;
        if(this != &other)
        {
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            delete[] m_data;
            m_data = new T[m_capacity];
            for(int i = 0; i < other.m_size; ++i)
            {
                m_data[i] = other.m_data[i];
            }   
        }
        return *this;
    }

    Vector(Vector&& other) : m_capacity(other.m_capacity), m_size(other.m_size), m_data(other.m_data)
    {
        cout << "move ctor" << endl;
        other.m_data = nullptr;
    }

    T& operator[](size_t index)
    {
        assert(index >= 0 && index < m_size);
        return m_data[index];
    }

    size_t size()
    {
        return m_size;
    }

    void push_back(const T& data)
    {
        extend();
        m_data[m_size++] = data;
    }

private:
    void extend()
    {
        if(m_size + 1 <= m_capacity)
            return;
        m_capacity = m_capacity == 0 ? 1 : m_capacity <<= 1;
        auto old_data = m_data;
        m_data = new T[m_capacity];
        for(int i = 0; i < m_size; ++i)
        {
            m_data[i] = old_data[i];
        }
    }

private:
    size_t m_capacity;
    size_t m_size;
    T* m_data;
};

int main()
{
    Vector<int> vec;
    for(int i = 0; i < 10; ++i)
    {
        vec.push_back(i);
    }
    Vector<int> vec2(std::move(vec));
    for(int i = 0; i < vec.size(); ++i)
    {
        cout << vec2[i] << " ";
    }
}
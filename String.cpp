#include <cstring>
#include <iostream>
using namespace std;

class String
{
public: 
    String(const char* s)
    {
        cout << "ctor" << endl;
        size_t len = strlen(s);
        m_size = len + 1;
        m_str = new char[m_size];
        for(int i = 0; i < m_size; ++i)
        {
            m_str[i] = s[i];
        }
    }

    ~String() { delete[] m_str; }
    
    String(const String& other) : m_size(other.m_size)
    {
        cout << "copy ctor" << endl;
        m_str = new char[m_size];
        for(int i = 0; i < other.m_size; ++i)
        {
            m_str[i] = other.m_str[i];
        }
    }

    String& operator=(const String& other)
    {
        cout << "copy operator" << endl;
        if(this != &other)
        {
            delete[] m_str;
            m_size = other.m_size;
            m_str = new char[m_size];
            for(int i = 0; i < other.m_size; ++i)
            {
                m_str[i] = other.m_str[i];
            }
        }
        return *this;
    }

    String(String&& other)
    {
        cout << "move ctor" << endl;
        m_size = other.m_size;
        m_str = other.m_str;
        other.m_str = nullptr;
    }

    char* c_str()
    {
        return m_str;
    }

    size_t size()
    {
        return m_size;
    }

private:
    size_t m_size;
    char* m_str;
};

ostream& operator<<(ostream& os, String& s)
{
    os << s.c_str();
    return os;
}

int main()
{
    String s = "123";
    String s2 = std::move(s);

    cout << s2 << endl;
}
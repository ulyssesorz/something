#include <bits/stdc++.h>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <ctime>
#include <functional>
#include <memory>
#include <mutex>
#include <ostream>
#include <thread>
#include <unordered_map>
#include <vector>
using namespace std;

// class Singleton
// {
// public:
//     static Singleton& instance()
//     {
//         static Singleton s;
//         return s;
//     }
//     void show()
//     {
//         printf("%p\n", this);
//     }
// private:
//     Singleton() = default;
//     ~Singleton() = default;
//     Singleton(const Singleton&) = delete;
//     Singleton& operator=(const Singleton&) = delete;
// };

class Singleton
{
public:
    static Singleton* instance()
    {
        return inst;
    }
    void freeInstance()
    {
        if(inst != nullptr)
        {
            delete inst;
            inst = nullptr;
        }
    }
    void show()
    {
        printf("%p\n", this);
    }
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton* inst;
};
Singleton* Singleton::inst = new Singleton();


int main() {
    vector<thread> threads;
    for(int i = 0; i < 10; ++i)
    {
        threads.emplace_back([]()
        {
            Singleton::instance()->show();
        });
    }
    for(auto& t : threads)
    {
        t.join();
    }

    return 0;
}

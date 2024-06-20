#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

int flag = 0;
std::mutex mtx;
std::condition_variable cond;

int main() {
    thread t1([&]() {
        unique_lock<mutex> locker(mtx);
        for (int i = 0; i < 20; i += 2) {
            cond.wait(locker, [&]() { return flag == 0; });
            printf("%d ", i);
            flag = 1;
            cond.notify_one();
        }
    });
    thread t2([&]() {
        unique_lock<mutex> locker(mtx);
        for (int i = 1; i < 20; i += 2) {
            cond.wait(locker, [&]() { return flag == 1; });
            printf("%d ", i);
            flag = 0;
            cond.notify_one();
        }
    });
    t1.detach();
    t2.detach();
    this_thread::sleep_for(chrono::seconds(2));
}
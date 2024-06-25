#include <iostream>
#include <memory>
#include <vector>
using namespace std;

template<typename T>
struct Deletor
{
public:
    // 获取传入的vec
    Deletor(vector<T>& vec) : m_vec(vec) {}
    // 重写调用符，加入传进来的vec中
    void operator()(T* data)
    { 
        m_vec.push_back(*data); 
        delete data;
    } 
private:
    vector<T>& m_vec;
};

int main() {
    vector<int> arr;
    shared_ptr<int> sp(new int(1), Deletor<int>(arr));
    // 手动结束生命周期
    sp.reset(); 
    if(!arr.empty())
        cout << arr[0] << endl;
}
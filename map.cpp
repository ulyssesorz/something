#include <bits/stdc++.h>
#include <climits>
#include <list>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>
using namespace std;

template<typename KeyType, typename ValueType>
class UnorderedMap
{
public:
    explicit UnorderedMap(size_t size = 13) : _buckets(size), _size(size) {}
    ~UnorderedMap() = default;

    void Put(const KeyType& key, const ValueType& value)
    {
        size_t i = index(key);
        auto& bucket = _buckets[i];
        bool exist = false;
        for(auto& it : bucket)
        {
            if(it.first == key)
            {
                it.second = value;
                exist = true;
                break;
            }
        }
        if(!exist)
        {
            bucket.emplace_front(key, value);
        }
    }

    bool Get(const KeyType& key, ValueType& value)
    {
        size_t i = index(key);
        auto& bucket = _buckets[i];
        for(auto& it : bucket)
        {
            if(it.first == key)
            {
                value = it.second;
                return true;
            }
        }
        return false;
    }

    size_t size() const { return _size; }

    void show()
    {
        for(int i = 0; i < _size; ++i)
        {
            cout << "[" << i + 1 << "]: ";
            for(auto& it : _buckets[i])
            {
                cout << "(" << it.first << "," << it.second << ") ";
            }
            cout << endl;
        }
    }

private:
    size_t index(const KeyType& key)
    {
        return std::hash<KeyType>{}(key) % _size;
    }
private:
    vector<list<pair<KeyType, ValueType>>> _buckets;
    size_t _size;
};

int main() {
    UnorderedMap<int, int> hash;
    hash.Put(1, 1);
    hash.Put(2, 2);
    hash.Put(1, 0);
    // hash.show();
    int value;
    if(hash.Get(1, value))
        cout << value << endl;
    if(hash.Get(2, value))
        cout << value << endl;
    if(hash.Get(3, value))
        cout << value << endl;     

    return 0;
}
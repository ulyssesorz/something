#include <bits/stdc++.h>
using namespace std;

template<typename KeyType, typename ValueType>
class Lru
{
public:
    explicit Lru(size_t capacity) : _capacity(capacity) {}
    ~Lru() = default;

    bool Get(const KeyType& key, ValueType& value)
    {
        lock_guard<mutex> locker(_mtx);
        auto it = _hashmap.find(key);
        if(it != _hashmap.end())
        {
            _cache.splice(_cache.begin(), _cache, it->second);
            value = it->second->second;
            return true;
        }
        else
        {
            return false;
        }
    }

    void Set(const KeyType& key, const ValueType& value)
    {
        lock_guard<mutex> locker(_mtx);
        auto it = _hashmap.find(key);
        if(it != _hashmap.end())
        {
            _cache.splice(_cache.begin(), _cache, it->second);
            it->second->second = value;
        }
        else
        {
            _cache.emplace_front(key, value);
            _hashmap[key] = _cache.begin();
            if(_cache.size() > _capacity)
            {
                auto node = _cache.back();
                _hashmap.erase(node.first);
                _cache.pop_back();
            }
        }
    }
private:
    size_t _capacity;
    mutex _mtx;
    list<pair<KeyType, ValueType>> _cache;
    unordered_map<KeyType, typename list<pair<KeyType, ValueType>>::iterator> _hashmap;
};

template<typename KeyType, typename ValueType, size_t N = 10, size_t C = 10>
class SharedLru
{
public:
    SharedLru()
    {
        for(int i = 0; i < N; ++i)
        {
            Lru<KeyType, ValueType> lru(C);
            _lrus.emplace_back(make_shared<Lru<KeyType, ValueType>>(C));
        }
    }
    ~SharedLru() = default;

    bool Get(const KeyType& key, ValueType& value)
    {
        size_t index = hash<KeyType>{}(key) % N;
        return _lrus[index]->Get(key, value);
    }
    void Set(const KeyType& key, const ValueType& value)
    {
        size_t index = hash<KeyType>{}(key) % N;
        return _lrus[index]->Set(key, value);
    }
private:
    vector<shared_ptr<Lru<KeyType, ValueType>>> _lrus;
};


int main() {
    SharedLru<int, int> lru;
    int val;
    lru.Set(1, 1);
    lru.Set(1, 2);
    lru.Set(2, 4);
    lru.Get(1, val);
    cout << val << endl;
    lru.Get(2, val);
    cout << val << endl;
}
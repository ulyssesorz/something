#include <list>
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <iostream>
using namespace std;

template<typename KeyType, typename ValueType>
class LruCache
{
public:
    explicit LruCache(size_t capacity = 10) : m_capacity(capacity) {}
    ~LruCache() = default;
    
    bool Get(const KeyType& key, ValueType& value)
    {
        std::shared_lock<shared_mutex> locker(m_mtx);
        auto it = m_map.find(key);
        if(it != m_map.end())
        {
            m_cache.splice(m_cache.begin(), m_cache, it->second);
            value = it->second->second;
            return true;
        }
        return false;
    }

    void Put(const KeyType& key, const ValueType& value)
    {
        std::unique_lock<shared_mutex> locker(m_mtx);
        auto it = m_map.find(key);
        if(it != m_map.end())
        {
            m_cache.splice(m_cache.begin(), m_cache, it->second);
            it->second->second = value;
        }
        else
        {
            m_cache.emplace_front(key, value);
            m_map[key] = m_cache.begin();
        }
    }

private:
    list<pair<KeyType, ValueType>> m_cache;
    unordered_map<KeyType, typename list<pair<KeyType, ValueType>>::iterator> m_map;
    size_t m_capacity;
    shared_mutex m_mtx;
};
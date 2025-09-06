#pragma once

#include <unordered_map>
#include <list>
#include <memory>
#include <functional>
#include <optional>
template<typename K, typename V>
struct LRUCache
{
    size_t capacity;
    std::list<std::pair<K, V>> cacheList;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cacheMap;
    std::function<void(V&)> onEvict;  // 注意使用引用，避免值拷贝

    LRUCache(size_t cap)
        : capacity(cap), onEvict(nullptr) {}

    std::optional<V> get(const K& key)
    {
        auto it = cacheMap.find(key);
        if(it == cacheMap.end()) return std::nullopt;

        // 移动到链表头
        cacheList.splice(cacheList.begin(), cacheList, it->second);
        return it->second->second;
    }

    void put(const K& key, const V& val)
    {
        auto it = cacheMap.find(key);
        if(it != cacheMap.end())
        {
            it->second->second = val;
            cacheList.splice(cacheList.begin(), cacheList, it->second);
        }
        else
        {
            if(cacheList.size() == capacity)
            {
                auto del = cacheList.back();
                if(onEvict) onEvict(del.second);  // 调用回调释放资源
                cacheMap.erase(del.first);
                cacheList.pop_back();
            }
            cacheList.emplace_front(key, val);
            cacheMap[key] = cacheList.begin();
        }
    }
};
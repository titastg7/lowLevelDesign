/*##########################################################################
System Requirements
--------------------
1. Storing some data as a key-value pair.
2. Retrieving some data based on the provided key.
3. Eviction of data when the cache reaches its capacity.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Storing Frequently Accessed Data: The consumers of this class can choose to
store frequently accessed data as a key-value pair.
2. Retrieving Data: Consumers of this cache can retrieve the stored data with the correct key value.
3. Eviction Policy: The cache automatically evicts when the storage reaches its capacity.
 Here using the LRU Eviction Policy.
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Interface for cache eviction strategy
class EvictionStrategy
{
public:
    virtual void evict(list<pair<int, int>> &keys) = 0;
    virtual ~EvictionStrategy() = default;
};

// LRU eviction strategy
class LRUStrategy : public EvictionStrategy
{
public:
    void evict(list<pair<int, int>> &keys) override
    {
        if (!keys.empty())
            keys.pop_back();
    }
};

// Cache interface
class Cache
{
    int capacity;
    EvictionStrategy *evictPolicy;

public:
    virtual void put(int key, int value) = 0;
    virtual int get(int key) = 0;
    virtual ~Cache() = default;
    int getCapacity()
    {
        return capacity;
    }
};

// Concrete implementation of Cache using LRU strategy
class LRUCache : public Cache
{
    unordered_map<int, list<pair<int, int>>::iterator> cache;
    list<pair<int, int>> lruOrder;

public:
    LRUCache(EvictionStrategy *strategy, int size) : evictPolicy(strategy), capacity(size) {}

    int get(int key) override
    {
        auto it = cache.find(key);
        if (it != cache.end())
        {
            auto listIt = it->second;
            pair<int, int> keyValue = *listIt;
            lruOrder.erase(listIt);
            lruOrder.push_front(keyValue);
            cache[key] = lruOrder.begin();
            return keyValue.second;
        }
        return -1;
    }

    void put(int key, int value) override
    {
        auto it = cache.find(key);
        if (it != cache.end())
        {
            lruOrder.erase(it->second);
            lruOrder.push_front({key, value});
            cache[key] = lruOrder.begin();
        }
        else
        {
            if (cache.size() == getCapacity())
            {
                auto listIt = --lruOrder.end();
                cache.erase(listIt->first);
                evictPolicy->evict(lruOrder);
            }
            lruOrder.push_front({key, value});
            cache[key] = lruOrder.begin();
        }
    }
};

int main()
{
    // Usage of the LRUCache with LRUEvictionStrategy
    EvictionStrategy *strategy = new LRUStrategy();
    LRUCache *lrucache = new LRUCache(strategy, 3);
    lrucache->put(5, 6);
    lrucache->put(3, 1);
    cout << lrucache->get(5) << endl;
    lrucache->put(1, 2);
    cout << lrucache->get(1) << endl;
    lrucache->put(4, 4);

    delete lrucache;
    delete strategy;
}

/*
The code utilizes the Strategy Design Pattern.

In this pattern, algorithms are encapsulated in separate classes, allowing clients to
choose the algorithm at runtime. In the provided code, EvictionStrategy represents the
abstract strategy interface, and LRUStrategy is one of its concrete implementations.
This pattern allows for the easy addition of new eviction strategies without modifying
existing code, promoting flexibility and maintainability.

*/
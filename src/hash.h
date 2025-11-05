
#ifndef CO2_EMISSIONS_HASH_H
#define CO2_EMISSIONS_HASH_H

#include <iostream>
#include <vector>
#include <string>

template <typename K, typename V>
class MyHashMap {
public:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<Entry*> table;
    size_t capacity;
    size_t size;
    const double loadFactor = 0.75;

    size_t hashFunction(const K& key) const {
        std::hash<K> hashFunc;
        return hashFunc(key) % capacity;
    }

    void rehash() {
        size_t oldCapacity = capacity;
        capacity *= 2;
        std::vector<Entry*> newTable(capacity, nullptr);

        for (auto head: table) {
            while (head) {
                size_t index = std::hash<K>()(head->key) % capacity;
                Entry* newEntry = new Entry(head->key, head->value);
                newEntry->next = newTable[index];
                newTable[index] = newEntry;
                head = head->next;
            }
        }
        for (auto head: table) {
            while (head) {
                Entry* temp = head;
                head = head->next;
                delete temp;
            }
        }
        table.swap(newTable);
    }

    MyHashMap(size_t initialCapacity = 16) : capacity(initialCapacity), size(0) {
        table.resize(capacity, nullptr);
    }

    ~MyHashMap() {
        for (auto head: table) {
            while (head) {
                Entry* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
    void insert(const K& key, const V& value) {
        size_t index = hashFunction(key);
        Entry* head = table[index];

        for (Entry* curr = head; curr; curr = curr->next) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
        }
        Entry* newEntry = new Entry(key, value);
        newEntry->next = head;
        table[index] = newEntry;
        size++;

        if ((double)size / capacity > loadFactor) {
            rehash();
        }
    }

    bool contains(const K& key) const {
        size_t index = hashFunction(key);
        for (Entry* curr = table[index]; curr; curr = curr->next) {
            if (curr->key == key) return true;
        }
        return false;
    }

    V* get(const K& key) {
        size_t index = hashFunction(key);
        for (Entry* curr = table[index]; curr; curr = curr->next) {
            if (curr->key == key) return &curr->value;
        }
        return nullptr;
    }

    void remove(const K& key) {
        size_t index = hashFunction(key);
        Entry* curr = table[index];
        Entry* prev = nullptr;

        while (curr) {
            if (curr->key == key) {
                if (prev) prev->next = curr->next;
                else table[index] = curr->next;
                delete curr;
                size--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
};
#endif
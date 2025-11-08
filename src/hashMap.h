#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <regex>
#include <fstream>
using namespace std;

template <typename K, typename V>
class hashMap {
private:
    static const int hashGroups = 50;
    list<pair<K, V>> table[hashGroups];

    int hashFunction(const K& key) const {
        return hash<K>{}(key) % hashGroups;
    }

public:
    bool isEmpty() const;
    void insertItem(const K& key, const V& value);
    void removeItem(const K& key);
    V* searchTable(const K& key);
    void printMap() const;
};

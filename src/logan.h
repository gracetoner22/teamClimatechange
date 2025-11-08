

#ifndef CO2_EMISSIONS_LOGAN_H
#define CO2_EMISSIONS_LOGAN_H
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <regex>
#include <fstream>
#include <list>
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
void buildMapFromFile(hashMap<string, vector<pair<int, double>>>& map, const string& filename);

template <typename K, typename V>
void hashMap<K, V>::printMap() const {
    for (int i = 0; i < hashGroups; i++) {
        if (table[i].empty()) continue;
        for (auto& pair: table[i]) {
            cout << "[INFO] Key: " << pair.first << " (Data entries: " << pair.second.size() << ")" << endl;
        }
    }
}

#endif //CO2_EMISSIONS_LOGAN_H
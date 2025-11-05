#include <iostream>
#include <list>
#include <string>
using namespace std;

//hash map to implement 905, jimmy

class hashMap {
private:
    static const int hashGroups = 10;
    list<pair<int, string>> table[hashGroups]; //list 1, index 0, list 2 index 1...
public:
    bool isEmpty() const;
    int hashFunction(int key);
    void insertItem(int key, string value);
    void removeItem(int key);
    string searchTable(int key);
    void printMap();
};

bool hashMap::isEmpty() const {
    int sum{};
    for (int i{}; i < hashGroups; i++) {
        sum += table[i].size();
    }
    return sum == 0;
}

int hashMap::hashFunction(int key) {
    return key % hashGroups; // key: 905, in return, this function will spit out 5
}

void hashMap::insertItem(int key, string value) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    bool keyExists = false;
    for (auto bItr = begin(cell); bItr != end(cell); bItr++) {
        if (bItr->first == key) {
            keyExists = true;
            bItr->second = value;
            cout << "[WARNING] Key exists. Value replaced." << endl;
            break;
        }
    }
    if (!keyExists) {
        cell.emplace_back(key, value);
    }
}

void hashMap::removeItem(int key) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    bool keyExists = false;
    for (auto bItr = begin(cell); bItr != end(cell); bItr++) {
        if (bItr->first == key) {
            keyExists = true;
            bItr = cell.erase(bItr);
            cout << "[INFO] Item removed." << endl;
            break;
        }
    }
    if (!keyExists) {
        cout << "[WARNING] Key not found. Pair not removed." << endl;
    }
}

string hashMap::searchTable(int key) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    for (auto bItr = begin(cell); bItr != end(cell); bItr++) {
        if (bItr->first == key) {
            cout << bItr->second << "\n";
            return bItr->second;
        }
    }
    return "Not Found";
}

void hashMap::printMap() {
    for (int i{}; i < hashGroups; i++) {
        if (table[i].size() == 0) 
            continue;

        for (auto bItr = table[i].begin(); bItr != table[i].end(); bItr++) {
            cout << "[INFO] Key: " << bItr->first << " Value: " << bItr->second << endl;
        }
    }
}

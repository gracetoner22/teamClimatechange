#include "hashmap.h"

template <typename K, typename V>
bool hashMap<K, V>::isEmpty() const {
    int sum = 0;
    for (int i = 0; i < hashGroups; i++) {
        sum += table[i].size();
    }
    return sum == 0;
}

template <typename K, typename V>
void hashMap<K, V>::insertItem(const K& key, const V& value) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    for (auto& pair: cell) {
        if (pair.first == key) {
            pair.second = value;
            cout << "[WARNING] Key exists. Value replaced.\n";
            return;
        }
    }
    cell.emplace_back(key, value);
}

template <typename K, typename V>
void hashMap<K, V>::removeItem(const K& key) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    for (auto it = begin(cell); it != end(cell); ++it) {
        if (it->first == key) {
            cell.erase(it);
            cout << "[INFO] Item removed." << endl;
            return;
        }
    }
    cout << "[WARNING] Key not found. Pair not removed." << endl;
}

template <typename K, typename V>
V* hashMap<K, V>::searchTable(const K& key) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    for (auto& pair: cell) {
        if (pair.first == key) {
            return &pair.second;
        }
    }
    return nullptr;
}

template <typename K, typename V>
void hashMap<K, V>::printMap() const {
    for (int i = 0; i < hashGroups; i++) {
        if (table[i].empty()) continue;
        for (auto& pair: table[i]) {
            cout << "[INFO] Key: " << pair.first << " (Data entries: " << pair.second.size() << ")" << endl;
        }
    }
}

void buildMapFromFile(hashMap<string, vector<pair<int, double>>>& map, const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    string stateCode = "Unknown";
    vector<pair<int, double>> stateData;

    while (getline(inputFile, line)) {

        if (line.find("USA-") != string::npos) {
            if (!stateData.empty()) {
                map.insertItem(stateCode, stateData);
                stateData.clear();
            }

            smatch match;
            regex stateRegex("\"?(USA-[A-Z]{2})\"?");
            if (regex_search(line, match, stateRegex)) {
                stateCode = match[1].str();
            }
        }

        if (line.find("[[") != string::npos) {
            string dataBlock = line;

            while (dataBlock.find("]]") == string::npos && getline(inputFile, line)) {
                dataBlock += line;
            }

            regex yearEmissionPattern("\\[\"(\\d+)\",\\s*([\\d.]+)\\]");
            smatch yearEmission;
            string::const_iterator searchStart(dataBlock.cbegin());
            while (regex_search(searchStart, dataBlock.cend(), yearEmission, yearEmissionPattern)) {
                int year = stoi(yearEmission[1]);
                double emission = stod(yearEmission[2]);
                stateData.emplace_back(year, emission);
                searchStart = yearEmission.suffix().first;
            }
        }
    }

    if (!stateData.empty()) {
        map.insertItem(stateCode, stateData);
    }

    inputFile.close();
    cout << "[INFO] Finished reading data from file." << endl;
}

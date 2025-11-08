#include "logan.h"
#include <iostream>
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
            return &pair. second;
        }
    }
    return nullptr;
}


void buildMapFromFile(hashMap<string, vector<pair<int, double>>>& map, const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line, state, sector;
    int year;
    double emission;

    while (getline(inputFile, line)) {
        if (line.find("\"series_id\"") != string::npos) {
            smatch match;
            regex idRegex("\"series_id\"\\s*:\\s*\"SEDS\\.([A-Z]+)\\.([A-Z]{2})\\.A\"");
            if (regex_search(line, match, idRegex)) {
                string seriesCode = match[1].str();
                state = "USA-" + match[2].str();
                if (seriesCode == "CDTCR") sector = "Electric";
                else if (seriesCode == "CLICE") sector = "Industrial";
                else if (seriesCode == "PMACE") sector = "Transportation";
                else if (seriesCode == "TERCE") sector = "Residential";
                else if (seriesCode == "TETCE") sector = "All";
                else sector = seriesCode;
            }
        }
        regex dataPattern("\\[\"(\\d+)\",\\s*([\\d.]+)\\]");
        smatch dataMatch;
        string::const_iterator searchStart(line.cbegin());
        while (regex_search(searchStart, line.cend(), dataMatch, dataPattern)) {
            int year = stoi(dataMatch[1]);
            double emission = stod(dataMatch[2]);

            string key = state + "|" + sector;
            vector<pair<int, double>>* existing = map.searchTable(key);
            if (existing) {
                existing->emplace_back(year, emission);
            } else {
                vector<pair<int, double>> tempVec = { {year, emission} };
                map.insertItem(key, tempVec);
            }
            searchStart = dataMatch.suffix().first;
        }
    }
}




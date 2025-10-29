#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>

using namespace std;
struct naryNode {
    std::string state;
    double emission = -1;
    std::unordered_map<std::string,naryNode*> childrenMap;

    naryNode(const string& state, double emission = -1)
    : state(state), emission(emission) {}
};
class naryTree {
public:
    naryNode* root;
    naryTree() {
        root = new naryNode("USA");
    }
    void addState(const std::vector<std::string>& states) {
        for (const auto& state : states) {
            naryNode* stateNode = new naryNode(state);
            root->childrenMap[state] = stateNode;
        }
    }
    void addYear(const string& stateName, const vector<string>& years) {
        naryNode* stateNode = root->childrenMap[stateName];
        if (!stateNode) return;

        for (const auto& year : years) {
            naryNode* yearNode = new naryNode(year);
            stateNode->childrenMap[year] = yearNode;
        }
    }
    void addEmission(const string& stateName, const string& year, double emission) {
        naryNode* stateNode = root->childrenMap[stateName];
        if (!stateNode) {
            stateNode = new naryNode(stateName);
            root->childrenMap[stateName] = stateNode;
        }
        naryNode* yearNode = stateNode->childrenMap[year];
        if (!yearNode) {
            yearNode = new naryNode(year);
            stateNode->childrenMap[year] = yearNode;
        }
        naryNode* emissionNode = new naryNode("Emission", emission);
        yearNode->childrenMap["Emission"] = emissionNode;
    }

    void printEmission(const string& stateName, const string& year) {
        naryNode* yearNode = root->childrenMap[stateName]->childrenMap[year];
        if (yearNode && yearNode->childrenMap["Emission"])
            cout << stateName << " (" << year << "): "
                 << yearNode->childrenMap["Emission"]->emission << " metric tons CO2\n";
        else
            cout << "No data for " << stateName << " in " << year << endl;
    }
    void printAll() {
        for (auto& [stateName, stateNode] : root->childrenMap) {
            cout << "State: " << stateName << "\n";
            for (auto& [year, yearNode] : stateNode->childrenMap) {
                if (yearNode->childrenMap.count("Emission")) {
                    cout << "  Year " << year << ": "
                         << yearNode->childrenMap["Emission"]->emission << " metric tons CO2\n";
                }
            }
        }
    }



    void buildTreeFromFile(naryTree& tree, const std::string& filename) {
        std::ifstream inputFile(filename);
        std::string line;
        std::regex pattern(R"(([A-Za-z\s]+), \[\[(.*?)\]\])");
        std::regex yearEmissionPattern("\\[\"(\\d+)\",([\\d.]+)\\]");

        while (std::getline(inputFile, line)) {
            std::smatch match;
            if (std::regex_search(line, match, pattern)) {
                std::string state = match[1];
                std::string dataBlock = match[2];
                std::smatch yearEmission;

                std::string::const_iterator searchStart(dataBlock.cbegin());
                while (std::regex_search(searchStart, dataBlock.cend(), yearEmission, yearEmissionPattern)) {
                    std::string year = yearEmission[1];
                    double emission = std::stod(yearEmission[2]);
                    tree.addEmission(state, year, emission);
                    searchStart = yearEmission.suffix().first;
                }
            }
        }
    }
};

//
// Created by Grace Toner on 10/15/25.
//


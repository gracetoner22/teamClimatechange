#include "emissionsClass.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
naryTree::naryTree() {
    root = new naryNode("USA");
}

void naryTree::addState(const std::vector<std::string>& states) {
    for (const auto& state : states) {
        naryNode* stateNode = new naryNode(state);
        root->childrenMap[state] = stateNode;
    }
}

void naryTree::addYear(const std::string& stateName, const std::vector<std::string>& years) {
    naryNode* stateNode = root->childrenMap[stateName];
    if (!stateNode) return;

    for (const auto& year : years) {
        naryNode* yearNode = new naryNode(year);
        stateNode->childrenMap[year] = yearNode;
    }
}

void naryTree::addEmission(const std::string& stateName, const std::string& year, double emission) {
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

void naryTree::printEmission(const std::string& stateName, const std::string& year) {
    naryNode* yearNode = root->childrenMap[stateName]->childrenMap[year];
    if (yearNode && yearNode->childrenMap["Emission"])
        std::cout << stateName << " (" << year << "): "
                  << yearNode->childrenMap["Emission"]->emission << " metric tons CO2\n";
    else
        std::cout << "No data for " << stateName << " in " << year << std::endl;
}

void naryTree::printTree(naryNode* node, int depth) {
    if (!node) node = root;
    for (int i = 0; i < depth; ++i) std::cout << "  ";

    std::cout << node->state;
    if (node->emission != -1) std::cout << " (Emission: " << node->emission << ")";
    std::cout << std::endl;

    for (const auto& [key, child] : node->childrenMap) {
        printTree(child, depth + 1);
    }
}

void naryTree::buildTreeFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string content((std::istreambuf_iterator<char>(inputFile)),
                        std::istreambuf_iterator<char>());

    std::regex statePattern(R"("name":".*?,\s*([A-Za-z\s]+))");
    std::regex dataPattern(R"("data":\[\[(.*?)\]\])");

    std::regex yearEmissionPattern("\\[\"(\\d+)\",([\\d.]+)\\]");
    auto stateBegin = std::sregex_iterator(content.begin(), content.end(), statePattern);
    auto stateEnd   = std::sregex_iterator();

    auto dataBegin = std::sregex_iterator(content.begin(), content.end(), dataPattern);
    auto dataEnd   = std::sregex_iterator();
    auto dataIt    = dataBegin;

    for (auto stateIt = stateBegin; stateIt != stateEnd; ++stateIt) {
        std::string state = (*stateIt)[1].str();


        if (dataIt != dataEnd) {
            std::string dataBlock = (*dataIt)[1].str();
            std::smatch yearEmission;
            std::string::const_iterator searchStart(dataBlock.cbegin());
            while (std::regex_search(searchStart, dataBlock.cend(), yearEmission, yearEmissionPattern)) {
                std::string year = yearEmission[1];
                double emission = std::stod(yearEmission[2]);
                addEmission(state, year, emission);

                searchStart = yearEmission.suffix().first;
            }
            ++dataIt;
        }
    }
}
double naryTree::getEmission(const std::string& state, const std::string& year) const {
    auto it = root->childrenMap.find(state);
    if (it == root->childrenMap.end()) return -1;
    naryNode* stateNode = it->second;

    auto it2 = stateNode->childrenMap.find(year);
    if (it2 == stateNode->childrenMap.end()) return -1;
    naryNode* yearNode = it2->second;

    auto it3 = yearNode->childrenMap.find("Emission");
    if (it3 == yearNode->childrenMap.end()) return -1;
    return it3->second->emission;
}




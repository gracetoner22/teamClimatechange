#include "emissionsClass.h"
#include "hash.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

naryTree::naryTree() {
    root = new naryNode("USA");
}
void naryTree::addState(const std::vector<std::string>& states) {
    for (const auto& state : states) {
        if (!root->childrenMap.contains(state)) {
            naryNode* stateNode = new naryNode(state);
            root->childrenMap.insert(state, stateNode);
        }
    }
}
void naryTree::addYear(const std::string& stateName, const std::vector<std::string>& years) {
    naryNode** statePtr = root->childrenMap.get(stateName);
    if (!statePtr) return;
    naryNode* stateNode = *statePtr;

    for (const auto& year : years) {
        if (!stateNode->childrenMap.contains(year)) {
            naryNode* yearNode = new naryNode(year);
            stateNode->childrenMap.insert(year, yearNode);
        }
    }
}
void naryTree::addEmission(const std::string& stateName, const std::string& year, double emission) {
    naryNode** statePtr = root->childrenMap.get(stateName);
    naryNode* stateNode = nullptr;
    if (!statePtr) {
        stateNode = new naryNode(stateName);
        root->childrenMap.insert(stateName, stateNode);
    } else {
        stateNode = *statePtr;
    }
    naryNode** yearPtr = stateNode->childrenMap.get(year);
    naryNode* yearNode = nullptr;
    if (!yearPtr) {
        yearNode = new naryNode(year);
        stateNode->childrenMap.insert(year, yearNode);
    } else {
        yearNode = *yearPtr;
    }
    naryNode* emissionNode = new naryNode("Emission", emission);
    yearNode->childrenMap.insert("Emission", emissionNode);
}
void naryTree::buildTreeFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    std::string state = "Unknown";

    while (std::getline(inputFile, line)) {

        if (line.find("\"name\":") != std::string::npos) {
            std::smatch match;
            std::regex nameRegex("\"name\":\"([^\"]+)\"");
            if (std::regex_search(line, match, nameRegex)) {
                std::string fullName = match[1].str();
                std::regex stateExtract(R"((\b[A-Z][a-z]*(?:\s[A-Z][a-z]*)*)$)");
                if (std::regex_search(fullName, match, stateExtract)) {
                    state = match[1].str();
                } else {
                    state = "Unknown";
                }
            }
        }

        if (line.find("\"data\":") != std::string::npos) {
            std::string dataBlock = line;

            while (dataBlock.find("]]") == std::string::npos && std::getline(inputFile, line)) {
                dataBlock += line;
            }
            std::regex yearEmissionPattern("\\[\"(\\d+)\",\\s*([\\d.]+)\\]");
            std::smatch yearEmission;
            std::string::const_iterator searchStart(dataBlock.cbegin());
            while (std::regex_search(searchStart, dataBlock.cend(), yearEmission, yearEmissionPattern)) {
                std::string year = yearEmission[1];
                double emission = std::stod(yearEmission[2]);
                addEmission(state, year, emission);
                searchStart = yearEmission.suffix().first;
            }
        }
    }
}


double naryTree::getEmission(const std::string& state, const std::string& year) const {
    naryNode** statePtr = root->childrenMap.get(state);
    if (!statePtr) return -1;
    naryNode* stateNode = *statePtr;

    naryNode** yearPtr = stateNode->childrenMap.get(year);
    if (!yearPtr) return -1;
    naryNode* yearNode = *yearPtr;

    naryNode** emissionPtr = yearNode->childrenMap.get("Emission");
    if (!emissionPtr) return -1;

    return (*emissionPtr)->emission;
}
void naryTree::deleteSubtree(naryNode* node) {
    if (!node) return;

    for (size_t i = 0; i < node->childrenMap.capacity; ++i) {
        auto entry = node->childrenMap.table[i];
        while (entry) {
            deleteSubtree(entry->value);
            entry = entry->next;
        }
    }

    delete node;
    }
naryTree::~naryTree() {
    deleteSubtree(root);
}

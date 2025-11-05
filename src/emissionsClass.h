#ifndef EMISSIONS_SOURCE_H
#define EMISSIONS_SOURCE_H

#include <string>
#include "hash.h"
#include <vector>

struct naryNode {
    std::string state;
    double emission;
    MyHashMap<std::string, naryNode*> childrenMap;

    naryNode(const std::string& state, double emission = -1)
        : state(state), emission(emission) {}
};

class naryTree {
public:
    naryNode* root;
    naryTree();

    void addState(const std::vector<std::string>& states);
    void addYear(const std::string& stateName, const std::vector<std::string>& years);
    void addEmission(const std::string& stateName, const std::string& year, double emission);
    void printEmission(const std::string& stateName, const std::string& year);
    void printTree(naryNode* node = nullptr, int depth = 0);
    void buildTreeFromFile(const std::string& filename);
    double getEmission(const std::string& state, const std::string& year) const ;
    void deleteSubtree(naryNode* node);
    ~naryTree();
};

#endif
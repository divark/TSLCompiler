#pragma once

#include <string>
#include <vector>
#include <cstddef>

#include "tsl_collector.hpp"

class TSLChoice {
    private:
        size_t categoryIdx;
        size_t choiceIdx;
    public:
        TSLChoice();
        TSLChoice(size_t, size_t);

        size_t getCategoryIdx();
        size_t getChoiceIdx();
};

class Node {
    private:
        size_t id;
        TSLChoice data;
    public:
        Node(TSLChoice);

        TSLChoice& getData();
        size_t getID() const;
        void setID(size_t);
};

class Edges {
    private:
        std::vector<std::vector<size_t>> edges;
    public:
        Edges(std::vector<std::vector<size_t>>);

        std::vector<size_t>& getNodeEdges(const Node&);
};

// These are the adapters that gets us Nodes from a TSLCollector, just
// to keep things modular.
std::vector<Node> getNodesFromCollector(const TSLCollector&);
Edges getEdgesFromTSLNodes(std::vector<Node>&, const TSLCollector&);

std::vector<Node> filterToNodesWithCategoryIdx(const std::vector<Node>&, size_t);

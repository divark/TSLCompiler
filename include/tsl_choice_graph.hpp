#pragma once

#include <string>
#include <vector>

#include "tsl_collector.hpp"

class TSLChoice {
    private:
        std::string category;
        std::string choice;
    public:
        TSLChoice(std::string, std::string);
};

class Node {
    private:
        size_t nodeID;
        TSLChoice nodeData;
    public:
        Node(TSLChoice);

        void setNodeID(size_t);
};

typedef std::vector<size_t> Edges;

class TSLChoiceDirectedGraph {
    private:
        std::vector<Node> nodes;
        std::vector<Edges> edges;
    public:
        TSLChoiceGraph(const TSLCollector&);

        std::vector<Node>& getNodes();
        Edges& getEdges(Node);
};

// These are the adapters that gets us Nodes and Edges from a TSLCollector, just
// to keep things modular.
std::vector<Node> getNodesFromCollector(const TSLCollector&);
std::vector<Edges> getEdgesFromCollector(const TSLCollector&, std::vector<Node>&);

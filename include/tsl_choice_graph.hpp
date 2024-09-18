#pragma once

#include <string>
#include <vector>

#include "tsl_collector.hpp"

class TSLChoice {
    private:
        std::string category;
        std::string choice;
    public:
        TSLChoice();
        TSLChoice(std::string, std::string);

        std::string& getCategory();
        std::string& getChoice();
};

class Node {
    private:
        TSLChoice data;
    public:
        Node(TSLChoice);

        TSLChoice& getData();
};

// These are the adapters that gets us Nodes and Edges from a TSLCollector, just
// to keep things modular.
std::vector<Node> getNodesFromCollector(const TSLCollector&);

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstddef>

#include "tsl_collector.hpp"
#include "tsl_testcase.hpp"

class TSLNode {
    private:
        std::string categoryLabel;
        Choice categoryChoice;
    public:
        TSLNode();
        TSLNode(std::string, Choice);

        std::string getCategoryLabel() const;
        Choice& getChoice();
};

class Node {
    private:
        size_t id;
        TSLNode data;
    public:
        Node(TSLNode);

        TSLNode& getData();
        size_t getID() const;
        void setID(size_t);
};

class Edges {
    private:
        std::vector<std::vector<size_t>> edges;
    public:
        Edges();
        Edges(std::vector<std::vector<size_t>>);

        const std::vector<size_t>& getNodeEdges(const Node&) const;
};

class TSLGraph {
    private:
        std::vector<Node> nodes;
        std::vector<Node> visitedNodes;
        Edges edges;

        std::vector<TSLTestCase> generatedTestCases;

        std::unordered_map<size_t, std::vector<std::string>> nodeProperties;
        std::unordered_set<std::string> seenPropertiesOverall;

        std::unordered_map<size_t, std::vector<size_t>> nodesDiscoveredNonApplicables;
        std::unordered_set<size_t> nonApplicablesSeen;

        bool preorderCheckin(Node&);
        bool postorderCheckin(Node&);

        void addProperty(Property&);
        TSLTestCase makeTestCase(std::vector<Node>&);
        void generateNormalTestCase();
        void generateMarkerTestCase(Marker&);
        void addNonApplicables(Node&);
        void removeNonApplicables(Node&);

        bool isNonApplicable(Node&);
        bool checkIfNextCategoryNotApplicable(Node&);
    public:
        TSLGraph();
        TSLGraph(TSLCollector&);

        std::vector<Node>& getNodes();
        const std::vector<Node> getEdges(const Node&) const;
        const std::vector<Node>& getVisitedNodes() const;

        std::vector<TSLTestCase>& getGeneratedTestCases();

        void visitDFS(Node&);
};

// These are the adapters that gets us Nodes from a TSLCollector, just
// to keep things modular.
std::vector<Node> getNodesFromCollector(TSLCollector&);
Edges getEdgesFromTSLNodes(std::vector<Node>&, TSLCollector&);

std::vector<Node> filterToNodesWithMarkers(const std::vector<Node>&);
std::vector<Node> filterToNodesWithoutMarkers(const std::vector<Node>&);
std::vector<Node> filterToNodesWithCategory(const std::vector<Node>&, Category&);

#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstddef>
#include <memory>

#include "tsl_collector.hpp"
#include "tsl_grammar.hpp"
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

        const std::vector<size_t>& getNodeEdges(const std::shared_ptr<Node>) const;
};

class TSLGraph {
    private:
        std::vector<std::shared_ptr<Node>> nodes;
        std::vector<std::shared_ptr<Node>> visitedNodes;
        Edges edges;

        std::vector<TSLTestCase> generatedTestCases;
        std::vector<TSLTestCase> markerTestCases;

        std::unordered_map<size_t, std::vector<std::string>> nodeProperties;
        std::unordered_set<std::string> seenPropertiesOverall;

        std::unordered_set<size_t> nonApplicablesSeen;

        std::unordered_map<size_t, std::unordered_set<Marker>> markerNodesSeen;

        bool preorderCheckin(std::shared_ptr<Node>);
        bool postorderCheckin(std::shared_ptr<Node>);

        void addProperty(Property&);
        TSLTestCase makeTestCase(std::vector<std::shared_ptr<Node>>&);
        void generateNormalTestCase();
        void generateMarkerTestCase(Marker&);
        void addNonApplicable(const std::shared_ptr<Node>);
        void removeNonApplicable(std::shared_ptr<Node>);

        bool isNonApplicable(std::shared_ptr<Node>);
        bool checkIfNextCategoryNotApplicable(std::shared_ptr<Node>);

        bool checkIfMarkerAlreadyVisited(std::shared_ptr<Node>, Marker&);
        void markChoiceWithMarkerAsVisited(std::shared_ptr<Node>, Marker&);

        std::unordered_set<std::string> testCaseKeys;
        std::string generateNodesKey(std::vector<std::shared_ptr<Node>>&);
        bool checkIfNodesAlreadyTestCase(std::vector<std::shared_ptr<Node>>&);
    public:
        TSLGraph();
        TSLGraph(TSLCollector&);

        std::vector<std::shared_ptr<Node>>& getNodes();
        const std::vector<std::shared_ptr<Node>> getEdges(const std::shared_ptr<Node>) const;
        const std::vector<std::shared_ptr<Node>>& getVisitedNodes() const;

        std::vector<std::reference_wrapper<TSLTestCase>> getGeneratedTestCases();

        void visitDFS(std::shared_ptr<Node>);
};

// These are the adapters that gets us Nodes from a TSLCollector, just
// to keep things modular.
std::vector<std::shared_ptr<Node>> getNodesFromCollector(TSLCollector&);
Edges getEdgesFromTSLNodes(std::vector<std::shared_ptr<Node>>&, TSLCollector&);

std::vector<std::shared_ptr<Node>> filterToNodesWithMarkers(const std::vector<std::shared_ptr<Node>>&);
std::vector<std::shared_ptr<Node>> filterToNodesWithoutMarkers(const std::vector<std::shared_ptr<Node>>&);
std::vector<std::shared_ptr<Node>> filterToNodesWithCategory(const std::vector<std::shared_ptr<Node>>&, Category&);

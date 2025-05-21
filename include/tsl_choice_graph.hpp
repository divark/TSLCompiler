#pragma once

#include <memory>
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

class TSLGraph;

class Listener {
    public:
        virtual bool preorderCheckIn(const TSLGraph&, Node&) = 0;
        virtual bool postorderCheckIn(const TSLGraph&, Node&) = 0;
};

class TestCaseListener : public Listener {
    private:
        TSLCollector& tslVariables;
        std::vector<TSLTestCase> foundTestCases;

        size_t numTestCases = 1;

        void addTestCase(bool);
        void addTestChoice(Node&);
    public:
        TestCaseListener(TSLCollector&);

        bool preorderCheckIn(const TSLGraph&, Node&);
        bool postorderCheckIn(const TSLGraph&, Node&);

        std::vector<TSLTestCase> getTestCases();
};

class SymbolTableListener: public Listener {
    private:
        std::vector<std::string> latestPropertiesRecorded;

        TSLCollector& tslVariables;
        std::unordered_set<std::string> symbolTable;

        void addLatestProperties();
        void removeLatestProperties();
    public:
        SymbolTableListener(TSLCollector&);

        bool preorderCheckIn(const TSLGraph&, Node&);
        bool postorderCheckIn(const TSLGraph&, Node&);
};

class TSLGraph {
    private:
        std::vector<Node> nodes;
        std::vector<Node> visitedNodes;
        Edges edges;

        std::vector<std::shared_ptr<Listener>> preorderListeners;
        std::vector<std::shared_ptr<Listener>> postorderListeners;

        bool preorderCheckin(Node&);
        bool postorderCheckin(Node&);
    public:
        TSLGraph();
        TSLGraph(TSLCollector&);

        void addPreorderListener(std::shared_ptr<Listener>);
        void addPostorderListener(std::shared_ptr<Listener>);

        std::vector<Node>& getNodes();
        const std::vector<Node> getEdges(const Node&) const;
        const std::vector<Node>& getVisitedNodes() const;

        void visitDFS(Node&);
};

// These are the adapters that gets us Nodes from a TSLCollector, just
// to keep things modular.
std::vector<Node> getNodesFromCollector(TSLCollector&);
Edges getEdgesFromTSLNodes(std::vector<Node>&, TSLCollector&);

std::vector<Node> filterToNodesWithMarkers(const std::vector<Node>&);
std::vector<Node> filterToNodesWithoutMarkers(const std::vector<Node>&);
std::vector<Node> filterToNodesWithCategory(const std::vector<Node>&, Category&);

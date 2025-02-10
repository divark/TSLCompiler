#pragma once

#include <memory>
#include <vector>
#include <cstddef>

#include "tsl_collector.hpp"
#include "tsl_testcase.hpp"

class TSLChoice {
    private:
        size_t categoryIdx;
        size_t choiceIdx;

        bool isMarker;
    public:
        TSLChoice();
        TSLChoice(size_t, size_t);

        size_t getCategoryIdx() const;
        size_t getChoiceIdx() const;

        bool hasMarker() const;
        void toggleIfMarker(bool);
};

class Node {
    private:
        size_t id;
        TSLChoice data;
    public:
        Node(TSLChoice);

        const TSLChoice& getData() const;
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
        virtual void checkIn(const TSLGraph&, const Node&) = 0;
};

class TestCaseListener : public Listener {
    private:
        TSLCollector& tslVariables;
        std::vector<TSLTestCase> foundTestCases;

        size_t numTestCases = 1;
    public:
        TestCaseListener(TSLCollector&);

        void checkIn(const TSLGraph&, const Node&);
        std::vector<TSLTestCase> getTestCases();
};

class TSLGraph {
    private:
        std::vector<Node> nodes;
        std::vector<Node> visitedNodes;
        Edges edges;

        std::vector<std::shared_ptr<Listener>> preorderListeners;

        void preorderCheckin(const Node&);
    public:
        TSLGraph();
        TSLGraph(const TSLCollector&);

        void addPreorderListener(std::shared_ptr<Listener>);

        const std::vector<Node>& getNodes() const;
        const std::vector<Node> getEdges(const Node&) const;
        const std::vector<Node>& getVisitedNodes() const;

        void visitDFS(const Node&);
};

// These are the adapters that gets us Nodes from a TSLCollector, just
// to keep things modular.
std::vector<Node> getNodesFromCollector(const TSLCollector&);
Edges getEdgesFromTSLNodes(std::vector<Node>&, const TSLCollector&);

std::vector<Node> filterToNodesWithMarkers(const std::vector<Node>&);
std::vector<Node> filterToNodesWithCategoryIdx(const std::vector<Node>&, size_t);

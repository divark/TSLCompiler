#include "tsl_choice_graph.hpp"

/**
 * Returns a collection of Nodes holding all Choices found in their respective Categories.
 */
std::vector<Node> getNodesFromCollector(const TSLCollector& recordedVariables) {
    std::vector<Node> nodes;

    for (auto categoryIdx = 0; categoryIdx < recordedVariables.categories.size(); categoryIdx++) {
        auto categoryChoices = recordedVariables.categoryChoicesGraph[categoryIdx];
        for (auto choiceIdx : categoryChoices) {
            auto nodeData = TSLChoice(categoryIdx, choiceIdx);
            auto node = Node(nodeData);
            nodes.push_back(node);
        }
    }

    return nodes;
}

/**
 * Returns a directed graph, where the first set of nodes for some category connects
 * to the second, and the second to the third, and so on.
 */
Edges getEdgesFromTSLNodes(std::vector<Node>& nodes, const TSLCollector& recordedVariables) {
    std::vector<std::vector<size_t>> edges;

    for (auto i = 0; i < nodes.size(); i++) {
        nodes[i].setID(i);
        edges.push_back(std::vector<size_t>());
    }

    for (auto categoryIdx = 0; categoryIdx < recordedVariables.categories.size() - 1; categoryIdx++) {
        auto nextCategoryIdx = categoryIdx + 1;

        auto firstCategoryNodes = filterToNodesWithCategoryIdx(nodes, categoryIdx);
        auto secondCategoryNodes = filterToNodesWithCategoryIdx(nodes, nextCategoryIdx);

        for (auto firstCategoryNode : firstCategoryNodes) {
            for (auto secondCategoryNode: secondCategoryNodes) {
                edges[firstCategoryNode.getID()].push_back(secondCategoryNode.getID());
            }
        }
    }

    return Edges(edges);
}

/**
 * Returns a collection of Nodes that match the desired Category index.
 */
std::vector<Node> filterToNodesWithCategoryIdx(const std::vector<Node>& nodes, size_t desiredCategoryIdx) {
    std::vector<Node> foundNodes;

    for (auto node : nodes) {
        auto nodeData = node.getData();
        auto nodeCategoryIdx = nodeData.getCategoryIdx();

        if (nodeCategoryIdx != desiredCategoryIdx) {
            continue;
        }

        foundNodes.push_back(node);
    }

    return foundNodes;
}

Node::Node(TSLChoice nodeData) {
    id = 0;
    data = nodeData;
}

/**
 * Returns the ID assigned to the current Node.
 */
size_t Node::getID() const {
    return id;
}

/**
 * Sets the ID for the current Node.
 */
void Node::setID(size_t newID) {
    id = newID;
}

/**
 * Returns the contents of what is held in the current Node.
 */
TSLChoice& Node::getData() {
    return data;
}

TSLChoice::TSLChoice() {
    categoryIdx = 0;
    choiceIdx = 0;
}

TSLChoice::TSLChoice(size_t categoryIdx, size_t choiceIdx) {
    this->categoryIdx = categoryIdx;
    this->choiceIdx = choiceIdx;
}

/**
 * Returns the recorded Category.
 */
size_t TSLChoice::getCategoryIdx() {
    return categoryIdx;
}

/**
 * Returns the recorded Choice.
 */
size_t TSLChoice::getChoiceIdx() {
    return choiceIdx;
}

Edges::Edges(std::vector<std::vector<size_t>> edges) {
    this->edges = edges;
}

/**
 * Returns a set of edges recorded for some node.
 */
std::vector<size_t>& Edges::getNodeEdges(const Node& node) {
    return edges[node.getID()];
}
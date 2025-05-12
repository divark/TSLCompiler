#include "tsl_choice_graph.hpp"
#include "tsl_collector.hpp"
#include "tsl_testcase.hpp"
#include <memory>

/**
 * Returns a collection of Nodes holding all Choices found in their respective Categories.
 */
std::vector<Node> getNodesFromCollector(const TSLCollector& recordedVariables) {
    std::vector<Node> nodes;

    for (auto categoryIdx = 0; categoryIdx < recordedVariables.categories.size(); categoryIdx++) {
        auto categoryChoices = recordedVariables.categoryChoicesGraph[categoryIdx];
        for (auto choiceIdx : categoryChoices) {
            auto nodeData = TSLChoice(categoryIdx, choiceIdx);
            auto hasMarker = recordedVariables.singleMarkings[choiceIdx] || recordedVariables.errorMarkings[choiceIdx];
            nodeData.toggleIfMarker(hasMarker);

            auto hasConditionalMarker = recordedVariables.singleIfMarkings[choiceIdx] || recordedVariables.errorIfMarkings[choiceIdx];
            nodeData.toggleIfConditionalMarkers(hasConditionalMarker);

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
 * Returns a collection of Nodes that contain some Marker.
 */
std::vector<Node> filterToNodesWithMarkers(const std::vector<Node>& nodes) {
    std::vector<Node> foundNodes;

    for (auto node : nodes) {
        auto nodeData = node.getData();

        if (!nodeData.hasMarker()) {
            continue;
        }

        foundNodes.push_back(node);
    }

    return foundNodes;
}

/**
 * Returns a collection of Nodes that match the desired Category index.
 * NOTE: This excludes any nodes with markers by default.
 */
std::vector<Node> filterToNodesWithCategoryIdx(const std::vector<Node>& nodes, size_t desiredCategoryIdx) {
    std::vector<Node> foundNodes;

    for (auto node : nodes) {
        auto nodeData = node.getData();
        auto nodeCategoryIdx = nodeData.getCategoryIdx();

        if (nodeCategoryIdx != desiredCategoryIdx) {
            continue;
        }

        if (nodeData.hasMarker()) {
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
const TSLChoice& Node::getData() const {
    return data;
}

TSLChoice::TSLChoice() {
    categoryIdx = 0;
    choiceIdx = 0;
    isMarker = false;
}

TSLChoice::TSLChoice(size_t categoryIdx, size_t choiceIdx) {
    this->categoryIdx = categoryIdx;
    this->choiceIdx = choiceIdx;
    isMarker = false;
}

/**
 * Returns the recorded Category.
 */
size_t TSLChoice::getCategoryIdx() const {
    return categoryIdx;
}

/**
 * Returns the recorded Choice.
 */
size_t TSLChoice::getChoiceIdx() const {
    return choiceIdx;
}

/**
* Returns whether the Choice recorded has a single or error marking.
*/
bool TSLChoice::hasMarker() const {
    return isMarker;
}

/**
* Returns whether the Choice recorded has a single or error marking in a conditional statement.
*/
bool TSLChoice::hasConditionalMarker() const {
    return containsConditionalMarker;
}

/**
* Sets whether the Choice recorded has a single or error marking.
*/
void TSLChoice::toggleIfMarker(bool hasMarker) {
    isMarker = hasMarker;
}

/**
* Sets whether the Choice recorded has a conditional single or error marking.
*/
void TSLChoice::toggleIfConditionalMarkers(bool hasConditionalMarkers) {
    containsConditionalMarker = hasConditionalMarkers;
}

Edges::Edges() {}

Edges::Edges(std::vector<std::vector<size_t>> edges) {
    this->edges = edges;
}

/**
 * Returns a set of edges recorded for some node.
 */
const std::vector<size_t>& Edges::getNodeEdges(const Node& node) const {
    return edges[node.getID()];
}

TestCaseListener::TestCaseListener(TSLCollector& variables): tslVariables(variables) {}

/**
* Adds a chosen Choice to a currently populated test case.
*/
void TestCaseListener::addTestChoice(const Node& currentNode) {
    size_t currentTestCase = foundTestCases.size() - 1;

    auto chosenCategoryIdx = currentNode.getData().getCategoryIdx();
    auto chosenChoiceIdx = currentNode.getData().getChoiceIdx();

    auto chosenCategory = tslVariables.categories[chosenCategoryIdx];
    auto chosenChoice = tslVariables.choices[chosenChoiceIdx];
    foundTestCases[currentTestCase].addCategoryChoice(chosenCategory, chosenChoice);
}

/**
* Inserts a test case populated by variables found in a node.
*/
void TestCaseListener::addTestCase(bool isMarkerCase) {
    auto testCase = TSLTestCase();
    testCase.toggleIsMarker(isMarkerCase);
    testCase.setTestCaseNumber(this->numTestCases);
    numTestCases++;

    foundTestCases.push_back(testCase);
}

/**
* Creates a TSLTestCase on checking in.
*/
bool TestCaseListener::preorderCheckIn(const TSLGraph& currentGraph, const Node& currentNode) {
    bool hasConditionalMarkers = currentNode.getData().hasConditionalMarker();
    bool canProceed = !currentGraph.getEdges(currentNode).empty() && !hasConditionalMarkers;
    if (canProceed) {
        return canProceed;
    }

    auto isMarkerCase = currentNode.getData().hasMarker() || hasConditionalMarkers;
    addTestCase(isMarkerCase);

    auto visitedNodes = currentGraph.getVisitedNodes();
    for (auto node : visitedNodes) {
        addTestChoice(node);
    }

    return canProceed;
}

bool TestCaseListener::postorderCheckIn(const TSLGraph& currentGraph, const Node& currentNode) {
    bool canProceed = true;

    return canProceed;
}

std::vector<TSLTestCase> TestCaseListener::getTestCases() {
    return foundTestCases;
}

SymbolTableListener::SymbolTableListener(TSLCollector& collectorVariables): tslVariables(collectorVariables) {}

/**
* Adds the latest properties into the symbol table.
*/
void SymbolTableListener::addLatestProperties() {
    for (auto property : latestPropertiesRecorded) {
        symbolTable.insert(property);
    }
}

/**
* Removes, then clears the latest properties in the symbol table.
*/
void SymbolTableListener::removeLatestProperties() {
    for (auto property : latestPropertiesRecorded) {
        symbolTable.erase(property);
    }

    latestPropertiesRecorded.clear();
}

/**
* Registers properties for the current node if any were found if checked in
* preorder, otherwise removes them.

* Returns true if no issues were found upon checking in, or false otherwise.
*/
bool SymbolTableListener::preorderCheckIn(const TSLGraph& graph, const Node& currentNode) {
    bool canProceed = true;

    bool choiceHasExpression = tslVariables.hasStandardExpression(currentNode.getData().getChoiceIdx());

    bool conditionalsSatisfied = !choiceHasExpression || tslVariables.getChoiceExpression(currentNode.getData().getChoiceIdx())->evaluate(symbolTable);
    if (!conditionalsSatisfied) {
        canProceed = false;
        return canProceed;
    }

    auto normalProperties = tslVariables.choiceProperties[currentNode.getData().getChoiceIdx()];
    for (const auto propertyIdx : normalProperties) {
        auto foundProperty = tslVariables.properties[propertyIdx];
        latestPropertiesRecorded.push_back(foundProperty);
    }

    addLatestProperties();

    return canProceed;
}

/**
* Registers properties for the current node if any were found if checked in
* preorder, otherwise removes them.

* Returns true if no issues were found upon checking in, or false otherwise.
*/
bool SymbolTableListener::postorderCheckIn(const TSLGraph& graph, const Node& currentNode) {
    bool canProceed = true;

    removeLatestProperties();
    return canProceed;
}

TSLGraph::TSLGraph() {}

/**
* Constructs a permutation-based directed graph derived from
* variables found in the TSLCollector.
*/
TSLGraph::TSLGraph(const TSLCollector& tslVariables) {
    this->nodes = getNodesFromCollector(tslVariables);
    this->edges = getEdgesFromTSLNodes(this->nodes, tslVariables);
}

/**
* Returns a list of nodes currently recorded in the graph.
*/
const std::vector<Node>& TSLGraph::getNodes() const {
    return this->nodes;
}

/**
* Returns a list of Nodes connected to the current node.
*/
const std::vector<Node> TSLGraph::getEdges(const Node& currentNode) const {
    std::vector<Node> foundEdges;

    auto edgesFound = this->edges.getNodeEdges(currentNode);
    for (auto nodeEdgeID : edgesFound) {
        foundEdges.push_back(this->nodes[nodeEdgeID]);
    }

    return foundEdges;
}

/**
* Returns the list of Nodes visited so far.
*/
const std::vector<Node>& TSLGraph::getVisitedNodes() const {
    return this->visitedNodes;
}

/**
* Checks in with listeners subscribed to preorder events.
*/
bool TSLGraph::preorderCheckin(const Node& currentNode) {
    bool canProceed = false;
    for (const auto &preorderListener : preorderListeners) {
        canProceed = preorderListener->preorderCheckIn(*this, currentNode);
        if (!canProceed) {
            return canProceed;
        }
    }

    return canProceed;
}

/**
* Subscribes a listener to preorder traversal events.
*/
void TSLGraph::addPreorderListener(std::shared_ptr<Listener> preorderListener) {
    this->preorderListeners.push_back(preorderListener);
}

/**
* Checks in with listeners subscribed to postorder events.
*/
bool TSLGraph::postorderCheckin(const Node& currentNode) {
    bool canProceed = false;
    for (const auto &postorderListener : postorderListeners) {
        canProceed = postorderListener->postorderCheckIn(*this, currentNode);
        if (!canProceed) {
            return canProceed;
        }
    }

    return canProceed;
}

/**
* Subscribes a listener to preorder traversal events.
*/
void TSLGraph::addPostorderListener(std::shared_ptr<Listener> postorderListener) {
    this->postorderListeners.push_back(postorderListener);
}

/**
* Visits the TSLGraph in a DFS fashion.
*/
void TSLGraph::visitDFS(const Node& currentNode) {
    visitedNodes.push_back(currentNode);

    bool preorderChecksOut = this->preorderCheckin(currentNode);
    if (!preorderChecksOut) {
        visitedNodes.pop_back();
        return;
    }

    auto nodeEdges = this->getEdges(currentNode);
    for (auto edgeNode : nodeEdges) {
        visitDFS(edgeNode);
    }

    visitedNodes.pop_back();

    this->postorderCheckin(currentNode);
}

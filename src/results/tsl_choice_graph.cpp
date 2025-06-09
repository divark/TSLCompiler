#include "tsl_choice_graph.hpp"
#include "tsl_collector.hpp"
#include "tsl_grammar.hpp"
#include "tsl_testcase.hpp"

TSLNode::TSLNode() {
    categoryLabel = "";
    categoryChoice = Choice();
}

TSLNode::TSLNode(std::string newCategoryLabel, Choice newChoice) {
    categoryLabel = newCategoryLabel;
    categoryChoice = newChoice;
}

std::string TSLNode::getCategoryLabel() const {
    return categoryLabel;
}

Choice& TSLNode::getChoice() {
    return categoryChoice;
}

/**
 * Returns a collection of Nodes holding all Choices found in their respective Categories.
 */
std::vector<Node> getNodesFromCollector(TSLCollector& recordedVariables) {
    std::vector<Node> nodes;

    auto numCategories = recordedVariables.getNumCategories();
    for (auto categoryIdx = 0; categoryIdx < numCategories; categoryIdx++) {
        auto currentCategory = recordedVariables.getCategory(categoryIdx);
        auto numChoices = currentCategory.getNumChoices();
        for (auto choiceIdx = 0; choiceIdx < numChoices; choiceIdx++) {
            auto categoryLabel = currentCategory.getLabel();
            auto categoryChoice = currentCategory.getChoice(choiceIdx);
            auto nodeData = TSLNode(categoryLabel, categoryChoice);

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
Edges getEdgesFromTSLNodes(std::vector<Node>& nodes, TSLCollector& recordedVariables) {
    std::vector<std::vector<size_t>> edges;

    for (auto i = 0; i < nodes.size(); i++) {
        nodes[i].setID(i);
        edges.push_back(std::vector<size_t>());
    }

    auto numCategories = recordedVariables.getNumCategories();
    for (auto categoryIdx = 0; categoryIdx < numCategories - 1; categoryIdx++) {
        auto firstCategory = recordedVariables.getCategory(categoryIdx);
        auto nextCategory = recordedVariables.getCategory(categoryIdx + 1);

        auto firstCategoryNodes = filterToNodesWithCategory(nodes, firstCategory);
        firstCategoryNodes = filterToNodesWithoutMarkers(firstCategoryNodes);
        auto secondCategoryNodes = filterToNodesWithCategory(nodes, nextCategory);
        secondCategoryNodes = filterToNodesWithoutMarkers(secondCategoryNodes);

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
        auto nodeChoice = nodeData.getChoice();

        if (!nodeChoice.hasNormalMarker()) {
            continue;
        }

        foundNodes.push_back(node);
    }

    return foundNodes;
}

/**
 * Returns a collection of Nodes that do not contain some Marker.
 */
std::vector<Node> filterToNodesWithoutMarkers(const std::vector<Node>& nodes) {
    std::vector<Node> foundNodes;

    for (auto node : nodes) {
        auto nodeData = node.getData();
        auto nodeChoice = nodeData.getChoice();

        if (nodeChoice.hasNormalMarker()) {
            continue;
        }

        foundNodes.push_back(node);
    }

    return foundNodes;
}

/**
 * Returns a collection of Nodes that match the desired Category index.
 */
std::vector<Node> filterToNodesWithCategory(const std::vector<Node>& nodes, Category& desiredCategory) {
    std::vector<Node> foundNodes;

    auto desiredCategoryLabel = desiredCategory.getLabel();
    for (auto node : nodes) {
        auto nodeData = node.getData();
        auto nodeCategoryLabel = nodeData.getCategoryLabel();

        if (nodeCategoryLabel != desiredCategoryLabel) {
            continue;
        }

        foundNodes.push_back(node);
    }

    return foundNodes;
}

Node::Node(TSLNode nodeData) {
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
TSLNode& Node::getData() {
    return data;
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

TSLGraph::TSLGraph() {}

/**
* Constructs a permutation-based directed graph derived from
* variables found in the TSLCollector.
*/
TSLGraph::TSLGraph(TSLCollector& tslVariables) {
    this->nodes = getNodesFromCollector(tslVariables);
    this->edges = getEdgesFromTSLNodes(this->nodes, tslVariables);
}

/**
* Returns a list of nodes currently recorded in the graph.
*/
std::vector<Node>& TSLGraph::getNodes() {
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
* Returns the list of generated test cases after running visitDFS.
*/
std::vector<TSLTestCase>& TSLGraph::getGeneratedTestCases() {
    return generatedTestCases;
}

/**
* Checks in with listeners subscribed to preorder events.
*/
bool TSLGraph::preorderCheckin(Node& currentNode) {
    bool canProceed = true;

    if (isNonApplicable(currentNode)) {
        return canProceed;
    }

    auto currentChoice = currentNode.getData().getChoice();
    std::vector<Property> choiceProperties;
    if (currentChoice.getExpression()) {
        auto evaluatedChoiceProperties = currentChoice.getEvaluatedProperties(seenPropertiesOverall);
        // No properties returning means that the expression was not satisfied, indicating that
        // there was not an else statement either.
        if (!evaluatedChoiceProperties) {
            return false;
        }

        choiceProperties = evaluatedChoiceProperties.value().getProperties();
    } else {
        choiceProperties = currentChoice.getProperties();
    }

    for (auto property : choiceProperties) {
        auto markerFound = property.asMarker();
        // Any Choice containing a Marker is considered a dead-end,
        // since we want the minimal number of choices leading up to
        // it that causes the edge case.
        if (markerFound) {
            generateMarkerTestCase(markerFound.value());
            return false;
        }

        addProperty(property);
    }

    bool nextChoicesAreAllNotApplicable = checkIfNextCategoryNotApplicable(currentNode);
    if (nextChoicesAreAllNotApplicable) {
        addNonApplicables(currentNode);
    }

    return canProceed;
}

/**
* Checks in with listeners subscribed to postorder events.
*/
bool TSLGraph::postorderCheckin(Node& currentNode) {
    bool canProceed = true;

    if (visitedNodes.empty()) {
        return canProceed;
    }

    auto recentNodeVisitedIdx = visitedNodes.size() - 1;
    auto recentNode = visitedNodes[recentNodeVisitedIdx];
    auto recentNodeID = recentNode.getID();

    if (!nodeProperties.contains(recentNodeID)) {
        return canProceed;
    }

    auto recentChoiceProperties = nodeProperties[recentNodeID];
    for (auto recentProperty : recentChoiceProperties) {
        seenPropertiesOverall.erase(recentProperty);
    }

    nodeProperties.erase(recentNodeID);

    removeNonApplicables(currentNode);

    return canProceed;
}

/**
* Adds the Choice's property into the record of overall seen properties
* up until this point.
*/
void TSLGraph::addProperty(Property& propertyToAdd) {
    if (propertyToAdd.asMarker()) {
        return;
    }

    seenPropertiesOverall.insert(propertyToAdd.asString());

    auto recentNodeVisitedIdx = visitedNodes.size() - 1;
    auto recentNode = visitedNodes[recentNodeVisitedIdx];
    auto recentNodeID = recentNode.getID();
    if (!nodeProperties.contains(recentNodeID)) {
        std::vector<std::string> propertiesFound;
        nodeProperties.insert({recentNodeID, propertiesFound});
    }

    nodeProperties[recentNodeID].push_back(propertyToAdd.asString());
}

/**
* Returns a Test Case built by the recently visited TSLNodes
* populated by visitDFS.
*/
TSLTestCase TSLGraph::makeTestCase() {
    TSLTestCase testCase;

    for (auto& recentNode : visitedNodes) {
        auto categoryLabel = recentNode.getData().getCategoryLabel();
        auto& choice = recentNode.getData().getChoice();
        auto choiceLabel = choice.getLabel();
        if (isNonApplicable(recentNode)) {
            choiceLabel = "N/A";
        }

        testCase.addCategoryChoice(categoryLabel, choiceLabel);

        auto hasEvaluatedProperties = choice.getEvaluatedProperties(seenPropertiesOverall);
        if (!hasEvaluatedProperties) {
            continue;
        }

        auto isIfStatement = hasEvaluatedProperties.value().getType() == EvaluationType::If;
        testCase.setChoiceDependency(categoryLabel, choice.getExpression().value()->asString(), isIfStatement);
    }

    return testCase;
}

/**
* Generates a test case if there are no markers present.
*/
void TSLGraph::generateNormalTestCase() {
    auto testCase = makeTestCase();

    generatedTestCases.push_back(testCase);
}

/**
* Generates a test case flagged as an edge case if markers are present.
*/
void TSLGraph::generateMarkerTestCase(Marker& markerFound) {
    auto testCase = makeTestCase();

    testCase.toggleIsMarker(true);
    generatedTestCases.push_back(testCase);
}

/**
 * Adds all edges connected to the current node in the set of non applicables.
 */
void TSLGraph::addNonApplicables(Node& currentNode) {
    auto nodeEdges = getEdges(currentNode);

    std::vector<size_t> nodeFoundNonApplicables;
    for (auto& nextNode : nodeEdges) {
        nodeFoundNonApplicables.push_back(nextNode.getID());
        nonApplicablesSeen.insert(nextNode.getID());
    }

    nodesDiscoveredNonApplicables.insert({currentNode.getID(), nodeFoundNonApplicables});
}

/**
 * Removes the recent set of non applicable edges for the current node.
 */
void TSLGraph::removeNonApplicables(Node& currentNode) {
    if (!nodesDiscoveredNonApplicables.contains(currentNode.getID())) {
        return;
    }

    auto& nodeFoundNonApplicables = nodesDiscoveredNonApplicables[currentNode.getID()];
    for (auto& nextNodeID : nodeFoundNonApplicables) {
        nonApplicablesSeen.erase(nextNodeID);
    }

    nodesDiscoveredNonApplicables.erase(currentNode.getID());
}

/**
 * Checks whether the current node was flagged as Non Applicable.
 */
bool TSLGraph::isNonApplicable(Node& currentNode) {
    return nonApplicablesSeen.contains(currentNode.getID());
}

/**
 * Returns whether all edges going to the next Category are
 * unreachable. Unreachable for some edge means that a next Choice
 * has an expression that is not satisfied.
 */
bool TSLGraph::checkIfNextCategoryNotApplicable(Node& currentNode) {
    auto nodeEdges = getEdges(currentNode);
    if (nodeEdges.empty()) {
        return false;
    }

    bool allEdgesNotSatisfied = true;
    for (auto& nextNode : nodeEdges) {
        auto nextNodeExpression = nextNode.getData().getChoice().getExpression();
        if (!nextNodeExpression) {
            allEdgesNotSatisfied = false;
            break;
        }

        bool nextNodeSatisfied = nextNodeExpression.value()->evaluate(seenPropertiesOverall);
        if (nextNodeSatisfied) {
            allEdgesNotSatisfied = false;
            break;
        }
    }

    return allEdgesNotSatisfied;
}

/**
* Visits the TSLGraph in a DFS fashion.
*/
void TSLGraph::visitDFS(Node& currentNode) {
    visitedNodes.push_back(currentNode);

    bool preorderChecksOut = this->preorderCheckin(currentNode);
    if (!preorderChecksOut) {
        visitedNodes.pop_back();
        return;
    }

    auto nodeEdges = this->getEdges(currentNode);
    if (nodeEdges.empty()) {
        generateNormalTestCase();
    }

    for (auto edgeNode : nodeEdges) {
        visitDFS(edgeNode);
    }

    this->postorderCheckin(currentNode);
    visitedNodes.pop_back();
}

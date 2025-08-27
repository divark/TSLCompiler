#include "tsl_choice_graph.hpp"
#include "tsl_collector.hpp"
#include "tsl_grammar.hpp"
#include "tsl_testcase.hpp"
#include <functional>

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

        bool hasMarkers = nodeChoice.hasNormalMarker();
        if (!hasMarkers) {
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

        bool hasMarkers = nodeChoice.hasNormalMarker();
        if (hasMarkers) {
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
std::vector<std::reference_wrapper<TSLTestCase>> TSLGraph::getGeneratedTestCases() {
    std::vector<std::reference_wrapper<TSLTestCase>> foundTestCases;

    for (auto& markerTestCase : markerTestCases) {
        foundTestCases.push_back(markerTestCase);
    }

    for (auto& testCase : generatedTestCases) {
        foundTestCases.push_back(testCase);
    }

    return foundTestCases;
}

/**
* Checks in with listeners subscribed to preorder events.
*/
bool TSLGraph::preorderCheckin(Node& currentNode) {
    bool canProceed = true;

    bool currentNodeNonApplicable = isNonApplicable(currentNode);

    auto currentChoice = currentNode.getData().getChoice();
    std::vector<Property> choiceProperties;
    if (currentChoice.getExpression() && !currentNodeNonApplicable) {
        auto evaluatedChoiceProperties = currentChoice.getEvaluatedProperties(seenPropertiesOverall);
        // No properties returning means that the expression was not satisfied, indicating that
        // there was not an else statement either.
        if (!evaluatedChoiceProperties) {
            return false;
        } else {
            choiceProperties = evaluatedChoiceProperties.value().getProperties();
        }
    } else if (!currentNodeNonApplicable) {
        choiceProperties = currentChoice.getProperties();
    }

    for (auto property : choiceProperties) {
        auto markerFound = property.asMarker();
        // Any Choice containing a Marker is considered a dead-end,
        // since we want the minimal number of choices leading up to
        // it that causes the edge case.
        if (markerFound) {
            // We also don't want to visit Choices again if it leads to the
            // same Marker, mimicking what the TSLgenerator does.
            bool markerAlreadyVisited = checkIfMarkerAlreadyVisited(currentNode, markerFound.value());
            if (markerAlreadyVisited) {
                return false;
            }

            markChoiceWithMarkerAsVisited(currentNode, markerFound.value());
            generateMarkerTestCase(markerFound.value());
            return false;
        }

        addProperty(property);
    }

    bool nextChoicesAreAllNA = checkIfNextCategoryNotApplicable(currentNode);
    if (nextChoicesAreAllNA) {
        for (auto& nextNode : getEdges(currentNode)) {
            addNonApplicable(nextNode);
        }
    }

    return canProceed;
}

/**
* Checks in with listeners subscribed to postorder events.
*/
bool TSLGraph::postorderCheckin(Node& currentNode) {
    bool canProceed = true;

    removeNonApplicable(currentNode);

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
TSLTestCase TSLGraph::makeTestCase(std::vector<Node>& choiceNodesVisited) {
    TSLTestCase testCase;

    for (auto& recentNode : choiceNodesVisited) {
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
 * Returns a key represented as a string of node IDs concatenated
 * together.
 */
std::string TSLGraph::generateNodesKey(std::vector<Node>& nodes) {
    std::string nodesKey = "";
    for (auto& node : nodes) {
        auto foundNodeID = node.getID();
        // Even if a N/A was found on different Choices in the same Category,
        // we want to exclude a test case that looks the same.
        if (isNonApplicable(node)) {
            foundNodeID = -1;
        }

        nodesKey.push_back(foundNodeID);
    }

    return nodesKey;
}

/**
 * Returns a boolean indicating whether the given nodes
 * were already used to make a test case.
 */
bool TSLGraph::checkIfNodesAlreadyTestCase(std::vector<Node>& nodes) {
    std::string nodesKey = generateNodesKey(nodes);
    return testCaseKeys.contains(nodesKey);
}

/**
* Generates a test case if there are no markers present.
*/
void TSLGraph::generateNormalTestCase() {
    bool visitedNodesAlreadyTestCase = checkIfNodesAlreadyTestCase(visitedNodes);
    if (visitedNodesAlreadyTestCase) {
        return;
    }

    auto testCase = makeTestCase(visitedNodes);
    generatedTestCases.push_back(testCase);

    std::string visitedNodesKey = generateNodesKey(visitedNodes);
    testCaseKeys.insert(visitedNodesKey);
}

/**
* Generates a test case flagged as an edge case if markers are present.
*/
void TSLGraph::generateMarkerTestCase(Marker& markerFound) {
    bool visitedNodesAlreadyTestCase = checkIfNodesAlreadyTestCase(visitedNodes);
    if (visitedNodesAlreadyTestCase) {
        return;
    }

    std::vector<Node> markerNode = { visitedNodes.back() };
    auto testCase = makeTestCase(markerNode);

    testCase.setMarker(markerFound);
    markerTestCases.push_back(testCase);

    std::string visitedNodesKey = generateNodesKey(markerNode);
    testCaseKeys.insert(visitedNodesKey);
}

/**
 * Adds the current node in the set of non applicables.
 */
void TSLGraph::addNonApplicable(const Node& currentNode) {
    nonApplicablesSeen.insert(currentNode.getID());
}

/**
 * Removes the current node marked as Non Applicable if its within the set.
 */
void TSLGraph::removeNonApplicable(Node& currentNode) {
    if (!isNonApplicable(currentNode)) {
        return;
    }

    nonApplicablesSeen.erase(currentNode.getID());
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
 * Returns true if some Choice from the current Node has been checked in
 * before with its specific marker, or false if not.
 */
bool TSLGraph::checkIfMarkerAlreadyVisited(Node& currentChoiceNode, Marker& currentChoiceMarker) {
    auto currentChoiceID = currentChoiceNode.getID();
    return markerNodesSeen.contains(currentChoiceID) && markerNodesSeen[currentChoiceID].contains(currentChoiceMarker);
}

/**
 * Checks in a Choice's (from the Node) Marker.
 */
void TSLGraph::markChoiceWithMarkerAsVisited(Node& currentChoiceNode, Marker& currentChoiceMarker) {
    auto currentChoiceID = currentChoiceNode.getID();
    if (!markerNodesSeen.contains(currentChoiceID)) {
        std::unordered_set<Marker> markersSeen;
        markerNodesSeen.insert({currentChoiceID, markersSeen});
    }

    markerNodesSeen[currentChoiceID].insert(currentChoiceMarker);
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

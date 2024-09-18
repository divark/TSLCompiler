#include "tsl_choice_graph.hpp"

/**
 * Returns a collection of Nodes holding all Choices found in their respective Categories.
 */
std::vector<Node> getNodesFromCollector(const TSLCollector& recordedVariables) {
    std::vector<Node> nodes;

    for (auto i = 0; i < recordedVariables.categories.size(); i++) {
        auto category = recordedVariables.categories[i];

        auto categoryChoices = recordedVariables.categoryChoicesGraph[i];
        for (auto choiceIdx : categoryChoices) {
            auto choice = recordedVariables.choices[choiceIdx];

            auto nodeData = TSLChoice(category, choice);
            auto node = Node(nodeData);
            nodes.push_back(node);
        }
    }

    return nodes;
}

Node::Node(TSLChoice nodeData) {
    data = nodeData;
}

/**
 * Returns the contents of what is held in the current Node.
 */
TSLChoice& Node::getData() {
    return data;
}

TSLChoice::TSLChoice() {
    category = "";
    choice = "";
}

TSLChoice::TSLChoice(std::string newCategory, std::string newChoice) {
    // Categories and Choices coming in end with ':' and '.' respectively.
    // At this point, they aren't necessary, since printing out test cases
    // later do not include these.
    newCategory.pop_back();
    newChoice.pop_back();

    category = newCategory;
    choice = newChoice;
}

/**
 * Returns the recorded Category.
 */
std::string& TSLChoice::getCategory() {
    return category;
}

/**
 * Returns the recorded Choice.
 */
std::string& TSLChoice::getChoice() {
    return choice;
}

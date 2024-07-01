#include "tsl_collector.hpp"

/**
 * Returns an index to the recently stored Category.
 */
int TSLCollector::recordCategory(std::string categoryContents) {
    categories.push_back(categoryContents);
    categoryChoicesGraph.push_back(std::vector<int>());

    return categories.size() - 1;
}

/**
 * Returns an index to the recently stored Choice for the most recently recorded Category.
 */
int TSLCollector::recordChoice(std::string choiceContents) {
    choices.push_back(choiceContents);

    int choiceIdx = choices.size() - 1;
    int currentCategoryIdx = categories.size() - 1;
    categoryChoicesGraph[currentCategoryIdx].push_back(choiceIdx);
    return choiceIdx;
}

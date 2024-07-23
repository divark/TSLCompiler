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

    singleMarkings.push_back(false);
    errorMarkings.push_back(false);
    choiceProperties.push_back(std::vector<int>());
    choiceExpressions.push_back(std::vector<std::shared_ptr<Expression>>());

    int choiceIdx = choices.size() - 1;
    int currentCategoryIdx = categories.size() - 1;
    categoryChoicesGraph[currentCategoryIdx].push_back(choiceIdx);
    return choiceIdx;
}

/**
 * Returns an index to the recently stored Property for the most recently recorded Choice.
 */
int TSLCollector::recordProperty(std::string propertyContents) {
    // We could be recording multiple properties, so we want
    // to remove commas if found, since we don't care about them.
    if (propertyContents.ends_with(',')) {
        propertyContents.pop_back();
    }

    properties.push_back(propertyContents);

    int propertiesIdx = properties.size() - 1;
    int choiceIdx = choices.size() - 1;
    choiceProperties[choiceIdx].push_back(propertiesIdx);

    return propertiesIdx;
}

/**
 * Returns the Choice used (as an index) to apply a Single Marking.
 */
int TSLCollector::markChoiceAsSingle() {
    int choiceIdx = choices.size() - 1;

    singleMarkings[choiceIdx] = true;

    return choiceIdx;
}

/**
 * Returns the Choice used (as an index) to apply an Error Marking.
 */
int TSLCollector::markChoiceAsError() {
    int choiceIdx = choices.size() - 1;

    errorMarkings[choiceIdx] = true;

    return choiceIdx;
}

/**
 * Returns the Expression found for some Choice.
 */
std::shared_ptr<Expression> TSLCollector::getChoiceExpression(unsigned int choiceIdx) {
    return choiceExpressions[choiceIdx][0];
}

/**
 * Returns the index of the recently created SimpleExpression for the
 * current choice.
 */
int TSLCollector::recordSimpleExpression(std::string propertyContents) {
    auto simpleExpression = std::make_shared<Expression>(propertyContents);

    int currentChoiceIdx = choices.size() - 1;
    choiceExpressions[currentChoiceIdx].push_back(simpleExpression);

    return choiceExpressions[currentChoiceIdx].size() - 1;
}

/**
 * Returns the index of the recently created Unary Expression for the
 * current choice.
 */
int TSLCollector::recordUnaryExpression(ExpType unaryType) {
    // We want the most recent Expression recorded (2nd call to back()), meaning we have to
    // get the most recent Choice's Expression (1st call to back()).
    auto lastExpression = choiceExpressions.back().back();
    choiceExpressions.back().pop_back();
    auto unaryExpression = std::make_shared<Expression>(unaryType, lastExpression);

    int currentChoiceIdx = choices.size() - 1;
    choiceExpressions[currentChoiceIdx].push_back(unaryExpression);

    return choiceExpressions[currentChoiceIdx].size() - 1;
}


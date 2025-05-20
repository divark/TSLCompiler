#include "tsl_collector.hpp"
#include "tsl_grammar.hpp"

/**
 * Returns an index to the recently stored Category.
 */
int TSLCollector::recordCategory(std::string categoryContents) {
    categories.push_back(Category(categoryContents));

    return categories.size() - 1;
}

/**
* Returns a reference to the specified category
*/
Category& TSLCollector::getCategory(size_t categoryNum) {
    return categories[categoryNum];
}

/**
 * Returns an index to the recently stored Choice for the most recently recorded Category.
 */
int TSLCollector::recordChoice(std::string choiceContents) {
    auto currentCategory = getCategory(categories.size() - 1);
    currentCategory.addChoice(Choice(choiceContents));

    int choiceIdx = currentCategory.getNumChoices() - 1;
    return choiceIdx;
}

/**
 * Returns an index to the recently stored Property for the most recently recorded Choice.
 */
int TSLCollector::recordProperty(std::string propertyContents) {
    auto currentCategory = getCategory(categories.size() - 1);
    auto recentChoice = currentCategory.getRecentChoice();

    auto propertyWithoutComma = getPropertyWithoutComma(propertyContents);
    recentChoice.addProperty(Property(propertyWithoutComma));

    auto propertiesIdx = recentChoice.getNumProperties() - 1;

    return propertiesIdx;
}

/**
 * Returns the Choice used (as an index) to apply a Single Marking.
 */
int TSLCollector::markChoiceAsSingle() {
    auto category = getCategory(categories.size() - 1);

    auto recentlyAddedChoice = category.getRecentChoice();
    recentlyAddedChoice.setMarker(Marker::Single);

    auto choiceIdx = category.getNumChoices() - 1;
    return choiceIdx;
}

/**
 * Returns the Choice used (as an index) to apply an Error Marking.
 */
int TSLCollector::markChoiceAsError() {
    auto category = getCategory(categories.size() - 1);

    auto recentlyAddedChoice = category.getRecentChoice();
    recentlyAddedChoice.setMarker(Marker::Error);

    auto choiceIdx = category.getNumChoices() - 1;
    return choiceIdx;
}

/**
 * Returns the Expression found for some Choice.
 */
std::shared_ptr<Expression> TSLCollector::getChoiceExpression(unsigned int choiceIdx) const {
    return choiceExpressions[choiceIdx][0];
}

/**
 * Records a newly formed Expression for some Choice.
*/
int TSLCollector::recordExpression(std::shared_ptr<Expression> expression) {
    auto recentCategory = getCategory(categories.size() - 1);
    auto recentlyAddedChoice = recentCategory.getRecentChoice();
    recentlyAddedChoice.setExpression(expression);

    return 0;
}

/**
 * Returns the index of the recently created SimpleExpression for the
 * current choice.
 */
int TSLCollector::recordSimpleExpression(std::string propertyContents) {
    auto simpleExpression = std::make_shared<Expression>(propertyContents);

    return recordExpression(simpleExpression);
}

/**
 * Returns the index of the recently created Unary Expression for the
 * current choice.
 * Preconditions: The most recent Choice has at least one Expression.
 */
int TSLCollector::recordUnaryExpression(ExpType unaryType) {
    // We want the most recent Expression recorded (2nd call to back()), meaning we have to
    // get the most recent Choice's Expression (1st call to back()).
    auto lastExpression = choiceExpressions.back().back();
    choiceExpressions.back().pop_back();
    auto unaryExpression = std::make_shared<Expression>(unaryType, lastExpression);

    return recordExpression(unaryExpression);
}

/**
* Returns the index of the recently created Binary Expression for the
* current choice.
* Preconditions: The most recent Choice has exactly two Expressions.
*/
int TSLCollector::recordBinaryExpression(ExpType binaryType) {
    auto firstExpression = choiceExpressions.back().front();
    auto lastExpression = choiceExpressions.back().back();

    for (int i = 0; i < 2; i++) {
        choiceExpressions.back().pop_back();
    }
    auto binaryExpression = std::make_shared<Expression>(binaryType, firstExpression, lastExpression);

    return recordExpression(binaryExpression);
}

/**
* Returns whether the property defined in a simple expression is
* defined in the program or not.
*/
bool TSLCollector::isExpressionUndefined(std::shared_ptr<Expression> expression) const {
    auto currentCategoryIdx = categories.size() - 1;
    auto property = expression->asString();

    return !propertyDefinedInCategory.contains(property) || propertyDefinedInCategory.find(property)->second >= currentCategoryIdx;
}

/**
 * Returns the index of the Choice whose Properties and Markings were
 * defined from an If Statement.
 */
int TSLCollector::convertPropertiesToIfProperties() {
    auto currentChoiceIdx = choices.size() - 1;

    singleIfMarkings[currentChoiceIdx] = singleMarkings[currentChoiceIdx];
    singleMarkings[currentChoiceIdx] = false;
    errorIfMarkings[currentChoiceIdx] = errorMarkings[currentChoiceIdx];
    errorMarkings[currentChoiceIdx] = false;

    choiceIfProperties[currentChoiceIdx] = choiceProperties[currentChoiceIdx];
    choiceProperties[currentChoiceIdx] = std::vector<int>();

    return currentChoiceIdx;
}

/**
 * Returns the index of the Choice whose Properties and Markings were
 * defined from an Else Statement.
 */
int TSLCollector::convertPropertiesToElseProperties() {
    auto currentChoiceIdx = choices.size() - 1;

    singleElseMarkings[currentChoiceIdx] = singleMarkings[currentChoiceIdx];
    singleMarkings[currentChoiceIdx] = false;
    errorElseMarkings[currentChoiceIdx] = errorMarkings[currentChoiceIdx];
    errorMarkings[currentChoiceIdx] = false;

    choiceElseProperties[currentChoiceIdx] = choiceProperties[currentChoiceIdx];
    choiceProperties[currentChoiceIdx] = std::vector<int>();

    return currentChoiceIdx;
}

/**
 * Returns whether a Choice has been flagged having an Else
 * statement.
 */
bool TSLCollector::hasElseExpression(unsigned int choiceIdx) {
    return choiceHasElseStatement[choiceIdx];
}

/**
* Returns whether a Choice has been flagged as having an If
* statement, essentially.
*/
bool TSLCollector::hasStandardExpression(unsigned int choiceIdx) {
    return !choiceExpressions[choiceIdx].empty();
}

/**
 * Returns the most recent Choice index that has
 * now been flagged to have an Else Statement.
 */
int TSLCollector::markChoiceHasElse() {
    auto currentChoiceIdx = choices.size() - 1;

    choiceHasElseStatement[currentChoiceIdx] = true;

    return currentChoiceIdx;
}

/**
* Returns a Property without a comma attached if found in a property list,
* or the string as-is.
*/
std::string getPropertyWithoutComma(const std::string &uncleanedProperty) {
    std::string propertyWithoutComma(uncleanedProperty);
    // We could be recording multiple properties, so we want
    // to remove commas if found, since we don't care about them.
    if (propertyWithoutComma.ends_with(',')) {
        propertyWithoutComma.pop_back();
    }

    return propertyWithoutComma;
}

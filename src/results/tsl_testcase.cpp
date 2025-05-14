#include "tsl_testcase.hpp"

TSLChoice::TSLChoice() {
    categoryIdx = 0;
    choiceIdx = 0;

    isMarker = false;
    containsConditionalMarker = false;

    choiceExpression = "";
}

TSLChoice::TSLChoice(size_t categoryIdx, size_t choiceIdx) {
    this->categoryIdx = categoryIdx;
    this->choiceIdx = choiceIdx;

    isMarker = false;
    containsConditionalMarker = false;

    choiceExpression = "";
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
* Sets the Choice's Expression as a string.
*/
void TSLChoice::setChoiceExpression(std::string newChoiceExpression) {
    choiceExpression = newChoiceExpression;
}

/**
* Gets the Choice's Expression as a string.
*/
std::string TSLChoice::getChoiceExpression() const {
    return choiceExpression;
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

/**
 * Returns the recorded test case number.
 */
size_t TSLTestCase::getTestCaseNumber() {
    return testCaseNumber;
}

/**
* Sets the test case number.
*/
void TSLTestCase::setTestCaseNumber(size_t testCaseNum) {
    this->testCaseNumber = testCaseNum;
}

/**
 * Returns the number of Categories tracked in this
 * test case.
 */
std::vector<std::string>& TSLTestCase::getCategories() {
    return chosenCategories;
}

/**
 * Returns the chosen Choice for some given Category in this
 * test case.
 */
std::string TSLTestCase::getCategoryChoice(const std::string& category) {
    return categoryChoices[category];
}

/**
* Records a Choice for some given Category chosen.
*/
void TSLTestCase::addCategoryChoice(const std::string& chosenCategory, const std::string& chosenChoice) {
    categoryChoices[chosenCategory] = chosenChoice;
    chosenCategories.push_back(chosenCategory);
}

/**
* Returns the expression as a string for some Choice in the chosen Category.
*/
std::string TSLTestCase::getChoiceDependency(const std::string& chosenCategory) {
    auto categoryChoice = categoryChoices[chosenCategory];
    return choiceDependencies[categoryChoice];
}

/**
* Sets the expression for some Choice in the chosen Category.
*/
void TSLTestCase::setChoiceDependency(const std::string& chosenCategory, std::string choiceDependency, bool isIf) {
    auto categoryChoice = categoryChoices[chosenCategory];
    std::string newChoiceDependency = "";
    if (isIf) {
        newChoiceDependency = "follows [if " + choiceDependency + "]";
    } else {
        newChoiceDependency = "follows [else] after [if " + choiceDependency + "]";
    }

    choiceDependencies[categoryChoice] = newChoiceDependency;
}

/**
* Returns whether the test case has a Choice containing a
* single or error marking, or false otherwise.
*/
bool TSLTestCase::hasMarker() const {
    return isMarkerCase;
}

/**
* Sets whether the test case has a Choice containing
* a single or error marking.
*/
void TSLTestCase::toggleIsMarker(bool isMarkerCase) {
    this->isMarkerCase = isMarkerCase;
}

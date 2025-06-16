#include "tsl_testcase.hpp"

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
 * Returns whether a Choice in some chosen Category has a dependency or not.
 */
bool TSLTestCase::hasChoiceDependency(const std::string& chosenCategory) {
    auto categoryChoice = categoryChoices[chosenCategory];
    return choiceDependencies.contains(categoryChoice);
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
        newChoiceDependency = "follows [else] from [if " + choiceDependency + "]";
    }

    choiceDependencies[categoryChoice] = newChoiceDependency;
}

/**
* Returns whether the test case has a Choice containing a
* single or error marking, or false otherwise.
*/
bool TSLTestCase::hasMarker() const {
    return markerCase.has_value();
}

Marker TSLTestCase::getMarker() const {
    return markerCase.value();
}

void TSLTestCase::setMarker(Marker newMarker) {
    markerCase = {newMarker};
}

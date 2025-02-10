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

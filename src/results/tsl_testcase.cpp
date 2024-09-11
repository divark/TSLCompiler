#include "tsl_testcase.hpp"

/**
 * Returns the recorded test case number.
 */
size_t TSLTestCase::getTestCaseNumber() {
    return testCaseNumber;
}

/**
 * Returns the number of Categories tracked in this
 * test case.
 */
size_t TSLTestCase::getNumCategories() {
    return categoryChoices.size();
}

/**
 * Returns the chosen Choice for some given Category in this
 * test case.
 */
std::string TSLTestCase::getCategoryChoice(const std::string& category) {
    return categoryChoices[category];
}

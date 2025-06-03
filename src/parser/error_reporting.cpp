#include "error_reporting.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <fstream>
#include <sstream>

#include "tsl_collector.hpp"

TSLException::TSLException(std::string errorMessage) {
    errorReported = errorMessage;
}

const std::string& TSLException::getErrorMessage() const {
    return errorReported;
}

FileReader::FileReader(std::string fileName) {
    std::ifstream inputContents(fileName);

    for (std::string line; std::getline(inputContents, line);) {
        // Everyone has a preference for how tabs are rendered in their terminal.
        // I'd rather not have to read that preference from a terminal, since C++
        // itself does not care about the width of a tab, only that it exists.
        // Thus, it's best to just replace each tab with a space.
        std::replace(line.begin(), line.end(), '\t', ' ');
        inputLines.push_back(line);
    }
}

bool FileReader::hasLine(int lineNumber) {
    auto adjustedLineNumber = lineNumber - 1;
    return adjustedLineNumber >= 0 && adjustedLineNumber < inputLines.size();
}

std::string FileReader::getLine(size_t lineNumber) {
    return inputLines[lineNumber - 1];
}

/**
 * Returns a table-like representation showing where the error took
 * place in the input file, line-by-line.
*/
std::string getPointingMsg(const yy::location& location) {
    std::string pointingMsg = "";

    auto filePath = std::string(*location.end.filename);
    auto fileReader = FileReader(filePath);

    auto lineNumber = location.end.line;
    auto columnNumber = location.end.column;
    if (fileReader.hasLine(lineNumber - 1)) {
        pointingMsg += fmt::format("{:^3}| {}\n", lineNumber - 1, fileReader.getLine(lineNumber - 1));
    }

    pointingMsg += fmt::format("{:^3}| {}\n", lineNumber, fileReader.getLine(lineNumber));
    std::string errorPointer = "";
    for (auto i = 0; i < columnNumber; i++) {
        if (i + 1 == columnNumber) {
            errorPointer += "^";
            break;
        }

        errorPointer += "-";
    }
    pointingMsg += fmt::format("{:^3}| {}\n", "", errorPointer);

    if (fileReader.hasLine(lineNumber + 1)) {
        pointingMsg += fmt::format("{:^3}| {}\n", lineNumber + 1, fileReader.getLine(lineNumber + 1));
    }

    return pointingMsg;
}

/**
* Prints an error message pointing to some location.
*/
void reportError(const std::string& errorMessage, const yy::location& location) {
    std::stringstream errorMessageSummary;
    errorMessageSummary << errorMessage << std::endl;

    errorMessageSummary << " --> " << location << std::endl;

    std::string errorPointingToMsg = getPointingMsg(location);
    errorMessageSummary << errorPointingToMsg;
    throw TSLException(errorMessageSummary.str());
}

/**
* Throws an exception if the property recorded for some Choice
* was already defined somewhere else.
*/
void checkIfCurrentPropertyRedefined(const std::string property, const TSLCollector &collector, const yy::location &locationInCode) {
    std::string propertyWithoutComma = getPropertyWithoutComma(property);
    bool propertyAlreadyDefined = collector.hasPropertyDefined(propertyWithoutComma);
    if (propertyAlreadyDefined) {
        auto errorSummaryMsg = fmt::format("Error: Property {} was already defined elsewhere.", propertyWithoutComma);
        reportError(errorSummaryMsg, locationInCode);
    }
}

/**
* Throws an exception if the property recorded for some Choice
* is undefined.
*/
void checkIfCurrentPropertyUndefined(const std::string property, const TSLCollector &collector, const yy::location &locationInCode) {
    auto currentCategoryIdx = collector.getNumCategories() - 1;
    auto foundPropertyLocation = collector.getPropertyCategoryIndex(property);
    auto propertyNotFound = !foundPropertyLocation;
    auto propertyInExpressionIsUndefined = propertyNotFound || foundPropertyLocation.value() >= currentCategoryIdx;
    if (propertyInExpressionIsUndefined) {
        std::string errorSummaryMsg = fmt::format("Error: Property {} not defined in any prior Categories.", property);
        reportError(errorSummaryMsg, locationInCode);
    }
}

#include "error_reporting.hpp"

#include <format>
#include <fstream>

#include "parser.hpp"
#include "tsl_collector.hpp"

FileReader::FileReader(std::string fileName) {
    std::ifstream inputContents(fileName);

    for (std::string line; std::getline(inputContents, line);) {
        inputLines.push_back(line);
    }
}

bool FileReader::hasLine(int lineNumber) {
    auto adjustedLineNumber = lineNumber - 1;
    return adjustedLineNumber >= 0 && adjustedLineNumber < inputLines.size();
}

std::string FileReader::getLine(uint lineNumber) {
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
        pointingMsg += std::format("{:^3}| {}\n", lineNumber - 1, fileReader.getLine(lineNumber - 1));
    }

    pointingMsg += std::format("{:^3}| {}\n", lineNumber, fileReader.getLine(lineNumber));
    std::string errorPointer = "";
    for (auto i = 0; i < columnNumber; i++) {
        if (i + 1 == columnNumber) {
            errorPointer += "^";
            break;
        }

        errorPointer += "-";
    }
    pointingMsg += std::format("{:^3}| {}\n", "", errorPointer);

    if (fileReader.hasLine(lineNumber + 1)) {
        pointingMsg += std::format("{:^3}| {}\n", lineNumber + 1, fileReader.getLine(lineNumber + 1));
    }

    return pointingMsg;
}

/**
* Prints an error message pointing to some location.
*/
void reportError(const std::string& errorMessage, const yy::location& location) {
    std::cerr << errorMessage << std::endl;

    std::cerr << " --> " << location << std::endl;

    std::string errorPointingToMsg = getPointingMsg(location);
    std::cerr << errorPointingToMsg;
}

/**
* Prints an error message pointing to the property in the file that is undefined,
* then aborts.
*/
void reportUndefinedPropertyError(std::shared_ptr<Expression> simpleExpression, const yy::location& location) {
    std::string errorSummaryMsg = std::format("Error: Property {} not defined in any prior Categories.", simpleExpression->asString());
    reportError(errorSummaryMsg, location);
}

/**
* Throws an exception if the property recorded for some Choice
* was already defined somewhere else.
*/
void checkIfCurrentPropertyRedefined(const std::string property, const TSLCollector &collector, const yy::location &locationInCode) {
    std::string propertyWithoutComma = getPropertyWithoutComma(property);
    if (collector.propertyDefinedInCategory.contains(propertyWithoutComma)) {
        auto errorSummaryMsg = std::format("Error: Property {} was already defined elsewhere.", propertyWithoutComma);
        reportError(errorSummaryMsg, locationInCode);
        throw yy::parser::syntax_error(locationInCode, "Please see the error message above for more details.");
    }
}

/**
* Throws an exception if the property recorded for some Choice
* is undefined.
*/
void checkIfCurrentPropertyUndefined(const std::string property, const TSLCollector &collector, const yy::location &locationInCode) {
    auto currentCategoryIdx = collector.categories.size() - 1;
    auto foundPropertyLocation = collector.propertyDefinedInCategory.find(property);
    auto propertyNotFound = foundPropertyLocation == collector.propertyDefinedInCategory.end();
    auto propertyInExpressionIsUndefined = propertyNotFound || foundPropertyLocation->second >= currentCategoryIdx;
    if (propertyInExpressionIsUndefined) {
        std::string errorSummaryMsg = std::format("Error: Property {} not defined in any prior Categories.", property);
        reportError(errorSummaryMsg, locationInCode);
        throw yy::parser::syntax_error(locationInCode, "Please see the error message above for more details.");
    }
}

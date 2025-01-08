#include "error_reporting.hpp"

#include <format>
#include <fstream>

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
* Prints an error message pointing to the property in the file that is undefined,
* then aborts.
*/
void reportUndefinedPropertyError(std::shared_ptr<Expression> simpleExpression, const yy::location& location) {
    std::string errorSummaryMsg = std::format("Error: Property {} not defined in prior Categories.", simpleExpression->asString());
    std::cerr << errorSummaryMsg << std::endl;

    std::cerr << " --> " << location << std::endl;

    std::string errorPointingToMsg = getPointingMsg(location);
    std::cerr << errorPointingToMsg;
}

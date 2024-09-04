#include "tsl_lexer.hpp"

#include <iostream>

TSLLexer::TSLLexer() : yyFlexLexer() {
    inputContents = std::istringstream("");
    switch_streams(&inputContents, &std::cout);

    lineNumber = 1;
    hasLoadedErrorTracking = false;
}

/**
 * Stores all contents of some given input file found at a path.
 * Preconditions: File found at inputPath exists.
 */
void TSLLexer::load(const std::filesystem::path& inputPath) {
    if (!std::filesystem::exists(inputPath)) {
        throw "File does not exist: " + std::filesystem::absolute(inputPath).string();
    }

    const auto inputSize = std::filesystem::file_size(inputPath);
    std::ifstream inputStream(inputPath);
    std::string inputBuffer(inputSize, '\0');

    inputStream.read(inputBuffer.data(), inputSize);
    inputContents = std::istringstream(inputBuffer);

    switch_streams(&inputContents, &std::cout);
    filePath = inputPath.string();
}

/**
 * Returns the next Token type as an integer read from the Lexer.
 * Preconditions: TSLLexer::load was called.
 * Postconditions: A Token represented as a number (enum) is returned for
 * some rule defined in the lexer found in lexer.l.
 */
int TSLLexer::getNextToken() {
    return yylex();
}

/**
 * Populates the contents of the next read Token into the current result.
 */
int TSLLexer::constructNextToken(int* currentResult, yy::location* currentLocation) {
    if (!hasLoadedErrorTracking) {
        currentLocation->initialize(&filePath);
        hasLoadedErrorTracking = true;
    }

    auto currentTokenType = getNextToken();
    currentLocation->step();

    *currentResult = currentTokenType;
    currentLocation->columns(YYLeng());

    return currentTokenType;
}

/**
 * Returns the contents of a Token seen currently by the Lexer.
 * Preconditions: TSLLexer::getNextToken() was called.
 * Postconditions: A string containing a token matched from some rule defined
 * in the lexer found in lexer.l.
 */
std::string TSLLexer::getCurrentTokenContents() const {
    return std::string(YYText(), YYText() + YYLeng());
}

/**
 * Returns the currently recorded Line Number based on where
 * the Lexer is in the input file.
 */
size_t TSLLexer::getLineNumber() {
    return lineNumber;
}

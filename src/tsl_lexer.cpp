#include "tsl_lexer.hpp"

#include <iostream>

TSLLexer::TSLLexer() {
    lexer = std::make_unique<yyFlexLexer>();
    inputContents = std::istringstream("");
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

    lexer = std::make_unique<yyFlexLexer>(&inputContents);
}

/**
 * Returns the next Token type read from the Lexer.
 * Preconditions: TSLLexer::load was called.
 * Postconditions: A Token represented as a number (enum) is returned for
 * some rule defined in the lexer found in tsl.l.
 */
TSLToken TSLLexer::getNextToken() const {
    int lexerStatus = lexer->yylex();

    return static_cast<TSLToken>(lexerStatus);
}

/**
 * Returns the contents of a Token seen currently by the Lexer.
 * Preconditions: TSLLexer::getNextToken() was called.
 * Postconditions: A string containing a token matched from some rule defined
 * in the lexer found in tsl.l.
 */
std::string TSLLexer::getCurrentTokenContents() const {
    return std::string(lexer->YYText(), lexer->YYText() + lexer->YYLeng());
}

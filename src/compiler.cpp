#include "compiler.hpp"
#include "lexer_tokens.hpp"

#include <iostream>

TSLCompiler::TSLCompiler() {
    lexer = std::make_unique<yyFlexLexer>();
    inputContents = std::istringstream("");
}

/**
 * Stores all contents of some given input file found at a path.
 * Preconditions: File found at inputPath exists.
 */
void TSLCompiler::load(const std::filesystem::path& inputPath) {
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
 * Returns a Token read from the Lexer.
 * Preconditions: TSLCompiler::load was called.
 * Postconditions: A string containing a token matched from some rule defined
 * in the lexer.
 */
std::string TSLCompiler::getNextToken() const {
    int lexerStatus = lexer->yylex();
    if (lexerStatus == 0) {
        return "EOF";
    }

    return std::string(lexer->YYText(), lexer->YYText() + lexer->YYLeng());
}

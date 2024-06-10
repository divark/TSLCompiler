#include "compiler.hpp"

TSLCompiler::TSLCompiler() {
    lexer = std::make_unique<yyFlexLexer>();
    inputContents = "";
}

/**
 * Stores all contents of some given input file found at a path.
 * Preconditions: File found at inputPath exists.
 */
std::string TSLCompiler::load(const std::filesystem::path& inputPath) {
    if (!std::filesystem::exists(inputPath)) {
        throw "File does not exist: " + std::filesystem::absolute(inputPath).string();
    }

    const auto inputSize = std::filesystem::file_size(inputPath);
    std::ifstream inputStream(inputPath);
    std::string inputBuffer(inputSize, '\0');

    inputStream.read(inputBuffer.data(), inputSize);
    inputContents = inputBuffer;

    return inputContents;
}

/**
 * Returns a Token read from the Lexer.
 * Preconditions: TSLCompiler::load was called.
 * Postconditions: A string containing a token matched from some rule defined
 * in the lexer.
 */
std::string TSLCompiler::getNextToken() const {
    std::istringstream inputStream(inputContents);
    while (lexer->yylex(inputStream, std::cout)) {
        continue;
    }

    return std::string(lexer->YYText());
}

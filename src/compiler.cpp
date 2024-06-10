#include "compiler.hpp"

TSLCompiler::TSLCompiler(const std::string& fileInput) {
    lexer = new yyFlexLexer;
}

TSLCompiler::~TSLCompiler() {
    delete lexer;
    lexer = nullptr;
}

bool TSLCompiler::hasResult() {
    return lexer != nullptr;
}

std::string TSLCompiler::generateTestCases() {
    throw "TSLCompiler::generateTestCases: Not yet implemented.";
}

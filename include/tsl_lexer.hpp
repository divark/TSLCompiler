#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <memory>

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "location.hh"

// yyFlexLexer is derived in order for us to have more control
// over what gets passed to the lexer via the yylex function.
class TSLLexer : yyFlexLexer {
    private:
        std::ifstream inputContents;
        std::string filePath;

        size_t lineNumber;
        size_t lineColumn;

        // This is required by yyFlexLexer to identify
        // which token has been found. However, since
        // we have our own explicit methods of
        // getNextToken and getCurrentTokenContents,
        // this serves as a helper function to those instead.
        int yylex();

    public:
        TSLLexer();

        void load(const std::filesystem::path& inputPath);

        int getNextToken();
        int constructNextToken(int*, yy::location*);
        std::string getCurrentTokenContents() const;

        size_t getLineNumber();
        size_t getLineColumn();
};

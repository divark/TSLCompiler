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
        std::istringstream inputContents;
        std::string filePath;

        bool hasLoadedErrorTracking;

    public:
        TSLLexer();

        void load(const std::filesystem::path& inputPath);
        int yylex();

        int getNextToken();
        int constructNextToken(int*, yy::location*);
        std::string getCurrentTokenContents() const;
};

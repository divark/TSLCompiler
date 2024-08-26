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

class TSLLexer {
    private:
        std::unique_ptr<FlexLexer> lexer;
        std::istringstream inputContents;
        std::string filePath;

        bool hasLoadedErrorTracking;

    public:
        TSLLexer();

        void load(const std::filesystem::path& inputPath);

        int getNextToken() const;
        int constructNextToken(int*, yy::location*);
        std::string getCurrentTokenContents() const;
};

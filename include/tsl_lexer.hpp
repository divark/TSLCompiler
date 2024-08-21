#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <memory>

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "filetracker.hpp"

struct TSLLexer {
    std::unique_ptr<FlexLexer> lexer;
    std::istringstream inputContents;
    std::unique_ptr<FileTracker> contentErrorTracker;

    TSLLexer();

    void load(const std::filesystem::path& inputPath);

    int getNextToken() const;
    int constructNextToken(int*) const;
    std::string getCurrentTokenContents() const;
};

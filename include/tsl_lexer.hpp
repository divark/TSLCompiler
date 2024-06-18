#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <memory>

#include "FlexLexer.h"
#include "lexer_tokens.hpp"

struct TSLLexer {
    std::unique_ptr<FlexLexer> lexer;
    std::istringstream inputContents;

    TSLLexer();

    void load(const std::filesystem::path& inputPath);

    TSLToken getNextToken() const;
    std::string getCurrentTokenContents() const;
};

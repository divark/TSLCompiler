#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <memory>

#include "FlexLexer.h"

struct TSLCompiler {
    std::unique_ptr<FlexLexer> lexer;
    std::istringstream inputContents;

    TSLCompiler();

    void load(const std::filesystem::path& inputPath);
    std::string getNextToken() const;
};

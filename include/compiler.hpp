#ifndef __COMPILER_HPP__
#define __COMPILER_HPP__

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <memory>

#include "FlexLexer.h"

struct TSLCompiler {
    std::unique_ptr<FlexLexer> lexer;
    std::string inputContents;

    TSLCompiler();

    std::string load(const std::filesystem::path& inputPath);
    std::string getNextToken() const;
};

#endif

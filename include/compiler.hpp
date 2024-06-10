#ifndef __COMPILER_HPP__
#define __COMPILER_HPP__

#include <string>
#include "FlexLexer.h"

struct TSLCompiler {
    FlexLexer* lexer;

    TSLCompiler(const std::string&);
    ~TSLCompiler();

    std::string generateTestCases();
    bool hasResult();
};

#endif

#pragma once

#include "tsl_lexer.hpp"
#include "tsl_collector.hpp"
#include "parser.hpp"

struct TSLParser {
    TSLLexer lexer;
    std::unique_ptr<yy::parser> parser;
    TSLCollector collector;

    TSLParser(const std::filesystem::path&);

    int run();
};


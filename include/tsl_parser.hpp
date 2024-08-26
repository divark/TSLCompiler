#pragma once

#include "tsl_lexer.hpp"
#include "tsl_collector.hpp"
#include "parser.hpp"

class TSLParser {
    private:
        TSLLexer lexer;
        std::unique_ptr<yy::parser> parser;
        TSLCollector collector;

    public:
        TSLParser(const std::filesystem::path&);

        TSLCollector& getCollector();
        int run();
};

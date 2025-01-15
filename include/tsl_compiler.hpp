#pragma once

#include "tsl_parser.hpp"
#include "tsl_testcase.hpp"

class TSLCompiler {
    private:
        std::unique_ptr<TSLParser> parser;
        std::vector<TSLTestCase> generatedTestCases;
    public:
        TSLCompiler(const std::filesystem::path&);
        int compile();

        TSLCollector& getCollector();
        std::vector<TSLTestCase>& getTestCases();
};

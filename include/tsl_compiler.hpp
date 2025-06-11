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

enum CompilerArgumentType {
    CountFrames,
    ToStandardOutput,
    OutputFile,
    InputFile,
};

class ArgumentException {
    private:
        std::string cause;
    public:
        ArgumentException(const std::string&);

        const std::string& what() const;
};

class TSLCompilerArgument {
    private:
        CompilerArgumentType argumentType;
        std::filesystem::path filePath;
    public:
        TSLCompilerArgument(CompilerArgumentType);
        TSLCompilerArgument(CompilerArgumentType, const std::filesystem::path&);

        std::string getName() const;
};

std::vector<TSLCompilerArgument> parseArguments(const std::vector<std::string>&);

#include "tsl_compiler.hpp"
#include "fmt/format.h"
#include "tsl_choice_graph.hpp"
#include "tsl_testcase.hpp"
#include <filesystem>
#include <memory>

/**
 * Returns a TSLCompiler with the loaded inputFile.
 */
TSLCompiler::TSLCompiler(const std::filesystem::path& inputFile) {
    parser = std::make_unique<TSLParser>(inputFile);
}

/**
 * Returns a reference to the generated test cases so far.
 */
std::vector<TSLTestCase>& TSLCompiler::getTestCases() {
    return generatedTestCases;
}

/**
 * Returns the collection of variables found after compiling.
 */
TSLCollector& TSLCompiler::getCollector() {
    return parser->getCollector();
}

/**
 * Runs the Compiler to generate test cases.
 */
int TSLCompiler::compile() {
    auto programStatus = parser->run();

    auto tslGraph = TSLGraph(parser->getCollector());

    // All Choices labeled with a Marker is considered an edge case, thus
    // these are processed first and separately.
    // NOTE: This does not exclude conditional Markers. Conditional Markers
    // are edge cases with relation to some other property that exists. This
    // means these Choice's edge cases can show up multiple times.
    auto nodesWithMarkers = filterToNodesWithMarkers(tslGraph.getNodes());
    for (auto categoryNode : nodesWithMarkers) {
        tslGraph.visitDFS(categoryNode);
    }

    auto firstCategory = parser->getCollector().getCategory(0);
    auto firstCategoryNodes = filterToNodesWithCategory(tslGraph.getNodes(), firstCategory);
    firstCategoryNodes = filterToNodesWithoutMarkers(firstCategoryNodes);
    for (auto categoryNode : firstCategoryNodes) {
        tslGraph.visitDFS(categoryNode);
    }

    generatedTestCases = tslGraph.getGeneratedTestCases();

    return programStatus;
}

ArgumentException::ArgumentException(const std::string& cause) {
    this->cause = cause;
}

const std::string& ArgumentException::what() const {
    return cause;
}

TSLCompilerArgument::TSLCompilerArgument(CompilerArgumentType argumentType) {
    this->argumentType = argumentType;
}

TSLCompilerArgument::TSLCompilerArgument(CompilerArgumentType argumentType, const std::filesystem::path& filePath) {
    this->argumentType = argumentType;
    this->filePath = filePath;
}

std::string TSLCompilerArgument::getName() const {
    switch (this->argumentType) {
        case CompilerArgumentType::CountFrames:
            return "count frames";
        case CompilerArgumentType::ToStandardOutput:
            return "output to standard output";
        case CompilerArgumentType::OutputFile:
            return fmt::format("output to {}", filePath.string());
        case CompilerArgumentType::InputFile:
            return fmt::format("input from {}", filePath.string());
        default:
            return "unrecognized argument";
    }
}

std::vector<TSLCompilerArgument> parseArguments(const std::vector<std::string> &argumentsFromArgv) {
    std::vector<TSLCompilerArgument> argumentsParsed;
    size_t numArgumentsToInvestigate = argumentsFromArgv.size() - 1;
    for (int i = 1; i < numArgumentsToInvestigate; i++) {
        auto& argumentFound = argumentsFromArgv[i];
        if (argumentFound == "-c") {
           argumentsParsed.push_back(TSLCompilerArgument(CompilerArgumentType::CountFrames));
        } else if (argumentFound == "-s") {
            argumentsParsed.push_back(TSLCompilerArgument(CompilerArgumentType::ToStandardOutput));
        } else if (argumentFound == "-o") {
            if (i + 1 == numArgumentsToInvestigate) {
                throw ArgumentException("-o needs an output file argument.");
            }

            std::filesystem::path outputFilePath(argumentsFromArgv[i + 1]);
            argumentsParsed.push_back(TSLCompilerArgument(CompilerArgumentType::OutputFile, outputFilePath));
            i++;
        } else {
            throw ArgumentException(fmt::format("Invalid argument: {}", argumentFound));
        }
    }

    std::filesystem::path inputFilePath(argumentsFromArgv[argumentsFromArgv.size() - 1]);
    if (!std::filesystem::exists(inputFilePath)) {
        throw ArgumentException(fmt::format("The input file {} does not exist.", inputFilePath.string()));
    }

    argumentsParsed.push_back(TSLCompilerArgument(CompilerArgumentType::InputFile, inputFilePath));

    return argumentsParsed;
}

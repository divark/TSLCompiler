#include "tsl_compiler.hpp"
#include "fmt/format.h"
#include "tsl_choice_graph.hpp"
#include "tsl_testcase.hpp"

#include <args.hxx>
#include <filesystem>
#include <memory>
#include <regex>

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

    auto foundTestCases = tslGraph.getGeneratedTestCases();
    for (auto& foundTestCase : foundTestCases) {
        generatedTestCases.push_back(foundTestCase.get());
    }

    for (int i = 1; i <= generatedTestCases.size(); i++) {
        generatedTestCases[i - 1].setTestCaseNumber(i);
    }

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

CompilerArgumentType TSLCompilerArgument::getType() const {
    return argumentType;
}

std::filesystem::path TSLCompilerArgument::getValue() const {
    return filePath;
}

/// Returns an error message mapped from the args library into
/// one of our own.
std::string convertToIdealErrorMsg(const std::string& argsErrorMsg) {
    std::regex invalidArgRegex(".+ could not be matched: '?([^']+)'?");
    std::smatch foundInvalidArgument;

    if (std::regex_match(argsErrorMsg, foundInvalidArgument, invalidArgRegex)) {
        return fmt::format("Invalid argument: {}", foundInvalidArgument[1].str());
    }

    return argsErrorMsg;
}

std::vector<TSLCompilerArgument> parseArguments(const std::vector<std::string> &argumentsFromArgv) {
    std::vector<TSLCompilerArgument> argumentsParsed;

    args::ArgumentParser argumentParser("A tool that generates test cases used as reference when writing test code.", "For more information on how this program works, consult USER_MANUAL.md in the docs directory.");

    args::HelpFlag help(argumentParser, "help", "Displays this help menu", {'h', "help"});

    args::Group outputGroup(argumentParser, "Outputs (Choose at most one):", args::Group::Validators::AtMostOne);
    args::Flag standardOutput(outputGroup, "standard_output", "Prints to the standard output", {'s'});
    args::ValueFlag<std::filesystem::path> outputFile(outputGroup, "output_file", "Writes to some output file at a given path", {'o'});

    args::Flag countingTestCases(argumentParser, "counting", "Counts the number of generated test frames", {'c'});
    args::Positional<std::filesystem::path> inputFile(argumentParser, "inputFile", "The TSL file to be read");

    try {
        argumentParser.ParseArgs(argumentsFromArgv);
    } catch (args::Help) {
        std::cout << argumentParser;
        return argumentsParsed;
    } catch (args::ParseError e) {
        std::string parseErrorMsg = convertToIdealErrorMsg(std::string(e.what()));
        throw ArgumentException(parseErrorMsg);
    } catch (args::ValidationError e) {
        throw ArgumentException(e.what());
    }

    if (!inputFile) {
        throw ArgumentException("Missing input file argument.");
    }

    if (countingTestCases) {
        auto countingCasesArg = TSLCompilerArgument(CompilerArgumentType::CountFrames);
        argumentsParsed.push_back(countingCasesArg);
    }

    if (standardOutput) {
        auto stdoutOutput = TSLCompilerArgument(CompilerArgumentType::ToStandardOutput);
        argumentsParsed.push_back(stdoutOutput);
    } else if (outputFile) {
        std::filesystem::path outputFilePath = args::get(outputFile);
        auto outputFileArg = TSLCompilerArgument(CompilerArgumentType::OutputFile, outputFilePath);
        argumentsParsed.push_back(outputFileArg);
    } else {
        std::filesystem::path outputFilePath = args::get(inputFile);
        outputFilePath.concat(".tsl");

        auto outputFileArg = TSLCompilerArgument(CompilerArgumentType::OutputFile, outputFilePath);
        argumentsParsed.push_back(outputFileArg);
    }

    std::filesystem::path inputFilePath = args::get(inputFile);
    if (!std::filesystem::exists(inputFilePath)) {
        throw ArgumentException(fmt::format("The input file {} does not exist.", inputFilePath.string()));
    }

    auto inputFileArg = TSLCompilerArgument(CompilerArgumentType::InputFile, inputFilePath);
    argumentsParsed.push_back(inputFileArg);

    return argumentsParsed;
}

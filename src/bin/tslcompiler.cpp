#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "error_reporting.hpp"
#include "fmt/format.h"
#include "tsl_compiler.hpp"
#include "tsl_testcase.hpp"

std::vector<TSLCompilerArgument> getArguments(int numArguments, char* arguments[]) {
    if (numArguments < 2) {
        throw ArgumentException("No arguments provided.");
    }

    std::vector<std::string> foundArguments;

    for (int i = 0; i < numArguments; i++) {
        std::string argument(arguments[i]);

        foundArguments.push_back(argument);
    }

    return parseArguments(foundArguments);
}

std::filesystem::path getInput(const std::vector<TSLCompilerArgument>& args) {
    return args.back().getValue();
}

std::optional<std::filesystem::path> getOutput(const std::vector<TSLCompilerArgument>& args) {
    for (const auto& arg : args) {
        if (arg.getType() == CompilerArgumentType::ToStandardOutput) {
            return {};
        }

        if (arg.getType() == CompilerArgumentType::OutputFile) {
            auto outputFilePath = arg.getValue();
            return {outputFilePath};
        }
    }

    return {};
}

bool checkIfCounting(const std::vector<TSLCompilerArgument> &args) {
    for (const auto& arg: args) {
        if (arg.getType() == CompilerArgumentType::CountFrames) {
            return true;
        }
    }

    return false;
}

void printCount(const std::vector<TSLTestCase> &testCases) {
    std::cout << testCases.size() << " test frames generated." << std::endl;
}

bool promptedNoToPrintCases(const std::optional<std::filesystem::path> &outputPath) {
    std::string desiredOutput = "";
    if (outputPath) {
        desiredOutput = outputPath.value().string();
    } else {
        desiredOutput = "the standard output";
    }

    std::string response = "";
    while (true) {
        std::cout << "Write test frames to " << desiredOutput << " (y/n)? ";
        std::cin >> response;

        if (response != "y" && response != "n") {
            std::cout << "Invalid response. Try again." << std::endl;
        }

        break;
    }

    return response == "n";
}

void printTestCase(TSLTestCase &testCase, std::ostream& outputStream) {
    std::stringstream formattedTestCase;
    formattedTestCase << fmt::format("Test Case {}", testCase.getTestCaseNumber());
    if (testCase.hasMarker()) {
        std::string markerType = "";
        if (testCase.getMarker() == Marker::Single) {
            markerType = "single";
        } else {
            markerType = "error";
        }

        formattedTestCase << fmt::format("\t\t<{}>", markerType);
    }
    formattedTestCase << std::endl;

    auto categories = testCase.getCategories();
    for (const auto& category : categories) {
        auto choice = testCase.getCategoryChoice(category);

        formattedTestCase << fmt::format("\t{}\t{}", category, choice);
        if (testCase.hasChoiceDependency(category)) {
            formattedTestCase << fmt::format("\t({})", testCase.getChoiceDependency(category));
        }

        formattedTestCase << std::endl;
    }

    outputStream << formattedTestCase.str() << std::endl << std::endl;
}

void printTestCases(std::vector<TSLTestCase> &testCases, const std::optional<std::filesystem::path> &outputLocation) {
    std::fstream outputWritingLocation;
    if (outputLocation) {
        outputWritingLocation = std::fstream(outputLocation.value(), std::ios::out);
    }

    for (auto& testCase : testCases) {
        if (outputLocation) {
            printTestCase(testCase, outputWritingLocation);
        } else {
            printTestCase(testCase, std::cout);
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        auto args = getArguments(argc, argv);
        auto inputFile = getInput(args);

        auto tslcompiler = TSLCompiler(inputFile);
        tslcompiler.compile();
        auto testCases = tslcompiler.getTestCases();

        auto outputLocation = getOutput(args);
        bool countingTestCases = checkIfCounting(args);
        if (countingTestCases) {
            printCount(testCases);
            if (promptedNoToPrintCases(outputLocation)) {
                return 0;
            }
        }

        printTestCases(testCases, outputLocation);
    } catch (const ArgumentException& argumentError) {
        std::cerr << argumentError.what() << std::endl;
        std::cerr << "Usage: " << argv[0] << " [ -cs ] [ -o output_file ] input_file" << std::endl;
        return 1;
    } catch (const TSLException& compilerError) {
        std::cerr << compilerError.getErrorMessage() << std::endl;
        return 1;
    }
}

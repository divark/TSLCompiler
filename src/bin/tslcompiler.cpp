#include <algorithm>
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

    for (int i = 1; i < numArguments; i++) {
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
        // Uppercase letter response are allowed, but we should not
        // have to complicate the if statement below.
        //
        // Referenced from:
        // https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
        std::transform(response.begin(), response.end(), response.begin(), [](unsigned char c) { return std::tolower(c); });

        if (response != "y" && response != "n") {
            std::cout << "Invalid response. Try again." << std::endl;
            continue;
        }

        break;
    }

    return response == "n";
}

size_t getLongestLength(const std::vector<std::string> &strings) {
    size_t longestStringFound = 0;

    for (const auto& someString : strings) {
        longestStringFound = std::max(someString.length(), longestStringFound);
    }

    return longestStringFound;
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
    // This is used for left-aligning the output, since it's easier to the eyes.
    size_t longestCategoryCount = getLongestLength(categories);

    // Likewise, this is used to ensure that all choice dependencies are aligned
    // relative to the longest choice in the Category, making it easier to the eyes.
    std::vector<std::string> choices;
    for (const auto& category : categories) {
        std::string currentChoice = testCase.getCategoryChoice(category);
        choices.push_back(currentChoice);
    }

    size_t longestChoiceCount = getLongestLength(choices);
    for (const auto& category : categories) {
        // We want to exclude the ':' at the end of the Category, since we add
        // our own later on.
        std::string categoryWithoutColon(category);
        categoryWithoutColon.pop_back();

        auto choice = testCase.getCategoryChoice(category);
        // Likewise, we want to remove the '.' to mimic how the TSLgenerator
        // does its Choice output.
        //
        // Except if it's a N/A.
        if (choice.back() == '.') {
            choice.pop_back();
        }

        formattedTestCase << fmt::format("\t{:<{}} : {:{}}", categoryWithoutColon, longestCategoryCount, choice, longestChoiceCount);
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

    if (outputLocation) {
        std::cout << testCases.size() << " test frames generated and written to " << outputLocation.value() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    try {
        auto args = getArguments(argc, argv);
        // This can only happen if someone specifies the help (-h, --help) flag.
        if (args.empty()) {
            return 0;
        }

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
        std::cerr << "For info on how to run the program, run: " << argv[0] << " -h" << std::endl;
        return 1;
    } catch (const TSLException& compilerError) {
        std::cerr << compilerError.getErrorMessage() << std::endl;
        return 1;
    }
}

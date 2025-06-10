#include <boost/ut.hpp>
#include <sstream>
#include <string>
#include <fmt/format.h>

#include "tsl_compiler.hpp"

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = fmt::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

std::vector<std::string> splitIntoArgvStyle(const std::string& rawArguments) {
    std::stringstream argumentReader(rawArguments);

    std::vector<std::string> argumentsFound = {"tslcompiler"};
    for(std::string readArgument; std::getline(argumentReader, readArgument, ' ');) {
        argumentsFound.push_back(readArgument);
    }

    return argumentsFound;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("All TSLgenerator arguments are honored.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("the arguments '{rawArguments}',") = [&](std::string rawArguments) {
              std::vector<std::string> programArguments = splitIntoArgvStyle(rawArguments);

              steps.when("we parse the arguments,") = [&] {
                  std::vector<TSLCompilerArgument> parsedArguments = parseArguments(programArguments);

                  steps.then("the '{argumentName}' argument is returned as argument {argumentNum}.") = [&](std::string argumentName, size_t argumentNum) {
                      auto& argument = parsedArguments[argumentNum - 1];
                      std::string actualArgumentName = argument.getName();
                      expect_eq(argumentName, actualArgumentName);
                  };
              };
          };
      };
    };
  };

    const auto file = [](const auto path) {
        std::ifstream file{path};
        return std::string{(std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>()};
    };

    "compiler-arguments"_test = steps | file(argv[1]);
}

#include <boost/ut.hpp>
#include <fstream>
#include <memory>

#include "tsl_parser.hpp"

namespace fs = std::filesystem;

/**
 * A class that intercepts stderr to be read
 * as a string for testing purposes.
 */
class ErrorMessageListener {
    private:
        std::streambuf* originalCerrStream;

        std::stringstream currentErrorContents;
    public:
        ErrorMessageListener();
        ErrorMessageListener(std::string);
        ~ErrorMessageListener();

        std::string getCurrentErrorMsg();
        std::string getLine(int);
};

/**
 * Intercepts stderr on construction to be read later.
 */
ErrorMessageListener::ErrorMessageListener() {
    auto oldCerrStream = std::cerr.rdbuf();
    std::cerr.rdbuf(currentErrorContents.rdbuf());

    originalCerrStream = oldCerrStream;
}

/**
* Populates the error message listener with some string
* before intercepting the stderr.
*/
ErrorMessageListener::ErrorMessageListener(std::string exceptionMsg) : ErrorMessageListener() {
    currentErrorContents << exceptionMsg;
}

/**
 * Resets stderr back to its original location.
 */
ErrorMessageListener::~ErrorMessageListener() {
    std::cerr.rdbuf(originalCerrStream);
}

/**
 * Returns the contents of stderr thus far.
 */
std::string ErrorMessageListener::getCurrentErrorMsg() {
    std::string currentErrorMsg(currentErrorContents.str());

    return currentErrorMsg;
}

/**
* Returns a line from the currently read stderr message.
*/
std::string ErrorMessageListener::getLine(int lineNumber) {
    auto errorMessage = std::istringstream(this->getCurrentErrorMsg());
    std::string errorLine = "";
    for (auto i = 0; i < lineNumber; i++) {
        if (!std::getline(errorMessage, errorLine)) {
            return "";
        }
    }
    return errorLine;
}

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = std::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("An error message should be provided when an issue comes up with the Lexer or Parser.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("a TSL input file called {file_name}") = [&](std::string file_name) {
              fs::path tslInput = std::format("tests/{}", file_name);

              steps.when("the error messages are redirected to be read by us,") = [&] {
                  auto stderrListener = std::make_unique<ErrorMessageListener>();

                  steps.when("the Parser consumes the input,") = [&] {
                      TSLParser parser(tslInput);
                      try {
                          parser.run();
                      } catch (yy::parser::syntax_error syntaxException) {
                          stderrListener = std::make_unique<ErrorMessageListener>(std::string(syntaxException.what()));
                      }

                      steps.then("line {lineNumber} of the error message should mention '{errorFound}'.") = [&](int lineNumber, std::string errorFound) {
                          auto expectedErrorSummary = std::format("Error: {}", errorFound);
                          auto actualErrorSummary = stderrListener->getLine(lineNumber);

                          expect_eq(expectedErrorSummary, actualErrorSummary);
                      };

                      steps.then("line {lineNumber} of the error message should mention finding {errorFound} in the wrong spot.") = [&](int lineNumber, std::string errorFound) {
                          auto expectedErrorSummary = std::format("Error: A {} is not allowed in its current spot.", errorFound);
                          auto actualErrorSummary = stderrListener->getLine(lineNumber);

                          expect_eq(expectedErrorSummary, actualErrorSummary);
                      };

                      steps.then("line {lineNumber} of the error message should mention the given TSL input file at line {inputLineNumber}, column {inputColumnNumber}.") = [&](int lineNumber, int inputLineNumber, int inputLineColumn) {
                          auto expectedFileLine = std::format(" --> {}:{}.{}", parser.getLexer().getFileName(), inputLineNumber, inputLineColumn);
                          auto actualFileLine = stderrListener->getLine(lineNumber);

                          expect_eq(expectedFileLine, actualFileLine);
                      };

                      steps.then("line {lineNumber} of the error message should point to the token {errorToken} in line {expectedFirstLineNumber} with line {expectedNextLineNumber} below it.") = [&](int lineNumber, std::string errorToken, int expectedFirstLineNumber, int expectedNextLineNumber) {
                          auto actualErrorPointedOut = stderrListener->getLine(lineNumber) + "\n"
                            + stderrListener->getLine(lineNumber + 1) + "\n";

                          auto errorTokenLineNumberFound = actualErrorPointedOut.find(std::format(" {} |", expectedFirstLineNumber)) != std::string::npos;
                          expect(errorTokenLineNumberFound) << std::format("Could not find line number {} in error message {}", expectedFirstLineNumber, actualErrorPointedOut);
                          auto errorTokenFoundInInput = actualErrorPointedOut.find(errorToken) != std::string::npos;
                          expect(errorTokenFoundInInput) << std::format("Could not find error token {} in error message {}", errorToken, actualErrorPointedOut);
                      };
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

    "errors"_test = steps | file(argv[1]);
}

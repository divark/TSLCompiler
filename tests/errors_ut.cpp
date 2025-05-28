#include <boost/ut.hpp>
#include <fstream>

#include "tsl_parser.hpp"

namespace fs = std::filesystem;

/**
 * A class that intercepts stderr to be read
 * as a string for testing purposes.
 */
class ErrorMessageListener {
    private:
        std::stringstream currentErrorContents;
    public:
        ErrorMessageListener();
        ErrorMessageListener(std::string);

        std::string getCurrentErrorMsg();
        void setCurrentErrorMsg(std::string);
        std::string getLine(size_t);
};

ErrorMessageListener::ErrorMessageListener() {
    currentErrorContents = std::stringstream("");
}

/**
* Populates the error message listener with some string
* before intercepting the stderr.
*/
ErrorMessageListener::ErrorMessageListener(std::string exceptionMsg) {
    currentErrorContents << exceptionMsg;
}

/**
 * Returns the contents of stderr thus far.
 */
std::string ErrorMessageListener::getCurrentErrorMsg() {
    std::string currentErrorMsg(currentErrorContents.str());

    return currentErrorMsg;
}

/**
* Sets the contents of the error message.
*/
void ErrorMessageListener::setCurrentErrorMsg(std::string currentErrorMsg) {
    currentErrorContents = std::stringstream(currentErrorMsg);
}

/**
* Returns a line from the currently read stderr message.
*/
std::string ErrorMessageListener::getLine(size_t lineNumber) {
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
    auto assertErrorMsg = fmt::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("An error message should be provided when an issue comes up with the Lexer or Parser.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("a TSL input file called {file_name}") = [&](std::string file_name) {
              fs::path tslInput = fmt::format("test-files/{}", file_name);

              steps.when("the Parser consumes the input,") = [&] {
                    ErrorMessageListener stderrListener;
                    TSLParser parser(tslInput);
                    try {
                        parser.run();
                    } catch (yy::parser::syntax_error syntaxException) {
                        stderrListener.setCurrentErrorMsg(syntaxException.what());
                    } catch (std::string errorMessage) {
                        stderrListener.setCurrentErrorMsg(errorMessage);
                    }

                    steps.then("line {lineNumber} of the error message should mention '{errorFound}'.") = [&](size_t lineNumber, std::string errorFound) {
                        auto expectedErrorSummary = fmt::format("Error: {}", errorFound);
                        auto actualErrorSummary = stderrListener.getLine(lineNumber);

                        expect_eq(expectedErrorSummary, actualErrorSummary);
                    };

                    steps.then("line {lineNumber} of the error message should mention finding '{errorFound}' in the wrong spot.") = [&](size_t lineNumber, std::string errorFound) {
                        auto expectedErrorSummary = fmt::format("Error: A {} is not allowed in its current spot.", errorFound);
                        auto actualErrorSummary = stderrListener.getLine(lineNumber);

                        expect_eq(expectedErrorSummary, actualErrorSummary);
                    };

                    steps.then("line {lineNumber} of the error message should mention the given TSL input file at line {inputLineNumber}, column {inputColumnNumber}.") = [&](size_t lineNumber, size_t inputLineNumber, size_t inputLineColumn) {
                        auto expectedFileLine = fmt::format(" --> {}:{}.{}", parser.getLexer().getFileName(), inputLineNumber, inputLineColumn);
                        auto actualFileLine = stderrListener.getLine(lineNumber);

                        expect_eq(expectedFileLine, actualFileLine);
                    };

                    steps.then("line {lineNumber} of the error message should point to the token {errorToken} in line {expectedFirstLineNumber} with line {expectedNextLineNumber} below it.") = [&](size_t lineNumber, std::string errorToken, size_t expectedFirstLineNumber, size_t expectedNextLineNumber) {
                        auto actualErrorPointedOut = stderrListener.getLine(lineNumber) + "\n"
                            + stderrListener.getLine(lineNumber + 1) + "\n";

                        auto errorTokenLineNumberFound = actualErrorPointedOut.find(fmt::format(" {} |", expectedFirstLineNumber)) != std::string::npos;
                        expect(errorTokenLineNumberFound) << fmt::format("Could not find line number {} in error message {}", expectedFirstLineNumber, actualErrorPointedOut);
                        auto errorTokenFoundInInput = actualErrorPointedOut.find(errorToken) != std::string::npos;
                        expect(errorTokenFoundInInput) << fmt::format("Could not find error token {} in error message {}", errorToken, actualErrorPointedOut);
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

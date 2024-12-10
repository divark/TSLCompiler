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
        std::streambuf* originalCerrStream;

        std::stringstream currentErrorContents;
    public:
        ErrorMessageListener();
        ~ErrorMessageListener();

        std::string getCurrentErrorMsg();
        std::string getLine(uint);
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
std::string ErrorMessageListener::getLine(uint lineNumber) {
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
                  auto stderrListener = ErrorMessageListener();

                  steps.when("the Parser consumes the input,") = [&] {
                      TSLParser parser(tslInput);
                      parser.run();

                      steps.then("line 1 of the error message should mention finding an unexpected property element.") = [&] {
                          auto expectedErrorSummary = "Error: Unexpected property element found";
                          auto actualErrorSummary = stderrListener.getLine(1);

                          expect_eq(expectedErrorSummary, actualErrorSummary);
                      };

                      steps.then("line 2 of the error message should mention the given TSL input file at line 1, column 3.") = [&] {
                          auto expectedFileLine = " --> tests/invalid_category.txt:1.3";
                          auto actualFileLine = stderrListener.getLine(2);

                          expect_eq(expectedFileLine, actualFileLine);
                      };

                      steps.then("line 3 of the error message should point to the \"Huh\" in line 1 with line 2 below it.") = [&] {
                          auto expectedErrorPointedOut =
                            "1 | Huh?\n"
                            "  | --^\n"
                            "2 |\n";

                          auto actualErrorPointedOut = stderrListener.getLine(3) + "\n"
                            + stderrListener.getLine(4) + "\n"
                            + stderrListener.getLine(5) + "\n";

                          expect_eq(expectedErrorPointedOut, actualErrorPointedOut);
                      };
                      //steps.then("the error message should point to line {line_number}.") = [&](uint line_number) {
                      //    auto currentErrorMsg = stderrListener.getCurrentErrorMsg();
                      //    auto expectedLineNumber = std::format(":{}", line_number);
                      //    auto expectedErrorMsgContent = tslInput.string() + expectedLineNumber;
                      //    expect(currentErrorMsg.find(expectedErrorMsgContent) != std::string::npos) << std::format("Could not find {} in {}", expectedErrorMsgContent, currentErrorMsg);

                      //    steps.then("the error message should point to line column {column_number}.") = [&](uint column_number) {
                      //      auto expectedLineColumn = std::format(".{}", column_number);
                      //      expectedErrorMsgContent = expectedErrorMsgContent + expectedLineColumn;
                      //      expect(currentErrorMsg.find(expectedErrorMsgContent) != std::string::npos) << std::format("Could not find {} in {}", expectedErrorMsgContent, currentErrorMsg);

                      //      steps.then("the error message should mention expecting a Category.") = [&] {
                      //          expectedErrorMsgContent = "CATEGORY_CONTENTS";
                      //          expect(currentErrorMsg.find(expectedErrorMsgContent) != std::string::npos) << std::format("Could not find {} in {}", expectedErrorMsgContent, currentErrorMsg);
                      //      };

                      //      steps.then("the error message should mention expecting a Choice.") = [&] {
                      //          expectedErrorMsgContent = "CHOICE_CONTENTS";
                      //          expect(currentErrorMsg.find(expectedErrorMsgContent) != std::string::npos) << std::format("Could not find {} in {}", expectedErrorMsgContent, currentErrorMsg);
                      //      };
                      //  };
                      //};
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

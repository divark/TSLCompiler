#include <catch2/catch_test_macros.hpp>
#include <cstdio>

#include "tsl_lexer.hpp"
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

SCENARIO("The Parser reports a malformed Category.") {
    GIVEN("a TSL input with a Category that ends with a '?' instead of a ':',") {
        fs::path tslInput = "tests/invalid_category.txt";
        WHEN("error messages are redirected to be read by us,") {
            auto stderrListener = ErrorMessageListener();
            
            WHEN("the Compiler consumes the input,") {
                TSLParser parser(tslInput);
                parser.run();
                THEN("the error message should be \"Parser failed at invalid_symbol.txt:1:4: Expected Category does not end with a ':'.\"") {
                    REQUIRE(parser.lexer.contentErrorTracker->getLineNumber() == 1);
                    REQUIRE(parser.lexer.contentErrorTracker->getLineColumn() == 4);
                    REQUIRE("Parser failed at invalid_symbol.txt:1:4: Expected Category does not end with a ':'." == stderrListener.getCurrentErrorMsg());
                }
            }
        }
    }
}

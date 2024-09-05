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

SCENARIO("The Parser reports an invalid Category.") {
    GIVEN("a TSL input with a Category that ends with a '?' instead of a ':',") {
        fs::path tslInput = "tests/invalid_category.txt";
        WHEN("error messages are redirected to be read by us,") {
            auto stderrListener = ErrorMessageListener();

            WHEN("the Parser consumes the input,") {
                TSLParser parser(tslInput);
                parser.run();
                THEN("the error message should point to line 1.") {
                    INFO(stderrListener.getCurrentErrorMsg());
                    REQUIRE(stderrListener.getCurrentErrorMsg().find("tests/invalid_category.txt:1") != std::string::npos);
                }

                THEN("error message should point to line column 4.") {
                    INFO(stderrListener.getCurrentErrorMsg());
                    REQUIRE(stderrListener.getCurrentErrorMsg().find("tests/invalid_category.txt:1.4") != std::string::npos);
                }

                THEN("the error message should mention expecting a Category.") {
                    REQUIRE(stderrListener.getCurrentErrorMsg().find("CATEGORY_CONTENTS") != std::string::npos);
                }
            }
        }
    }
}

SCENARIO("The Parser reports an invalid Choice.") {
    GIVEN("a TSL input with a Category, and a Choice that ends with a '?' instead of a '.',") {
        fs::path tslInput = "tests/invalid_choice.txt";
        WHEN("error messages are redirected to be read by us,") {
            auto stderrListener = ErrorMessageListener();

            WHEN("the Parser consumes the input,") {
                TSLParser parser(tslInput);
                parser.run();
                THEN("the error message should point to line 2.") {
                    INFO(stderrListener.getCurrentErrorMsg());
                    REQUIRE(stderrListener.getCurrentErrorMsg().find("tests/invalid_choice.txt:2") != std::string::npos);
                }

                THEN("error message should point to line column 8.") {
                    INFO(stderrListener.getCurrentErrorMsg());
                    REQUIRE(stderrListener.getCurrentErrorMsg().find("tests/invalid_choice.txt:2.8") != std::string::npos);
                }

                THEN("the error message should mention expecting a Choice.") {
                    REQUIRE(stderrListener.getCurrentErrorMsg().find("CHOICE_CONTENTS") != std::string::npos);
                }
            }
        }
    }
}

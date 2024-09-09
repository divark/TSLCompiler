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
                    auto currentErrorMsg = stderrListener.getCurrentErrorMsg();
                    INFO(currentErrorMsg);
                    auto expectedLineNumber = ":1";
                    REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber) != std::string::npos);
                    THEN("error message should point to line column 3.") {
                        auto expectedLineColumn = ".3";
                        REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber + expectedLineColumn) != std::string::npos);
                    }

                    THEN("the error message should mention expecting a Category.") {
                        REQUIRE(stderrListener.getCurrentErrorMsg().find("CATEGORY_CONTENTS") != std::string::npos);
                    }
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
                    auto currentErrorMsg = stderrListener.getCurrentErrorMsg();
                    INFO(currentErrorMsg);
                    auto expectedLineNumber = ":2";
                    REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber) != std::string::npos);
                    THEN("error message should point to line column 7.") {
                        auto expectedLineColumn = ".7";
                        REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber + expectedLineColumn) != std::string::npos);
                    }

                    THEN("the error message should mention expecting a Choice.") {
                        REQUIRE(currentErrorMsg.find("CHOICE_CONTENTS") != std::string::npos);
                    }
                }
            }
        }
    }
}

SCENARIO("The Parser reports an error for the common case of a property followed by an if statement.") {
    GIVEN("a TSL input with one Category and Choice, and another Category with a Choice defined with a property followed by an if statement,") {
        fs::path tslInput = "tests/invalid_property_then_if.txt";
        WHEN("error messages are redirected to be read by us,") {
            auto stderrListener = ErrorMessageListener();

            WHEN("the Parser consumes the input,") {
                TSLParser parser(tslInput);
                parser.run();
                THEN("the error message should point to line 5.") {
                    auto currentErrorMsg = stderrListener.getCurrentErrorMsg();
                    INFO(currentErrorMsg);
                    auto expectedLineNumber = ":5";
                    REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber) != std::string::npos);
                    THEN("error message should point to line column 43.") {
                        auto expectedLineColumn = ".43";
                        REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber + expectedLineColumn) != std::string::npos);
                    }

                    THEN("the error message should mention expecting a Choice.") {
                        REQUIRE(currentErrorMsg.find("CHOICE_CONTENTS") != std::string::npos);
                    }
                }
            }
        }
    }
}

SCENARIO("The Parser reports an error for the common case of a [single][error].") {
    GIVEN("a TSL input with one Category, one Choice, and another Choice with a [single][error],") {
        fs::path tslInput = "tests/invalid_double_marking.txt";
        WHEN("error messages are redirected to be read by us,") {
            auto stderrListener = ErrorMessageListener();

            WHEN("the Parser consumes the input,") {
                TSLParser parser(tslInput);
                parser.run();
                THEN("the error message should point to line 3.") {
                    auto currentErrorMsg = stderrListener.getCurrentErrorMsg();
                    INFO(currentErrorMsg);
                    auto expectedLineNumber = ":3";
                    REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber) != std::string::npos);
                    THEN("error message should point to line column 39.") {
                        auto expectedLineColumn = ".39";
                        REQUIRE(currentErrorMsg.find(tslInput.string() + expectedLineNumber + expectedLineColumn) != std::string::npos);
                    }

                    THEN("the error message should mention expecting a Choice.") {
                        REQUIRE(currentErrorMsg.find("CHOICE_CONTENTS") != std::string::npos);
                    }
                }
            }
        }
    }
}

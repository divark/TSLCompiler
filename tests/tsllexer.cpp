#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "tsl_lexer.hpp"
#include "parser.hpp"

namespace fs = std::filesystem;

SCENARIO("A Category should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the Lexer consumes the input,") {
            TSLLexer lexer;
            lexer.load(tslInput);
            THEN("the Lexer should return the Category contents as a string.") {
                REQUIRE(lexer.getNextToken() == yy::parser::token::CATEGORY_CONTENTS);
                REQUIRE(lexer.getCurrentTokenContents() == "Simple Category 1:");
            }
        }
    }
}

SCENARIO("A Comment should not be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the Lexer consumes the input,") {
            TSLLexer lexer;
            lexer.load(tslInput);
            THEN("the Lexer should not return the Category's Comment contents as a string.") {
                auto lexerToken = lexer.getNextToken();
                auto tokenContents = lexer.getCurrentTokenContents();

                REQUIRE(tokenContents != "# Comments are quite helpful as reminders.");

                REQUIRE(lexerToken == yy::parser::token::CATEGORY_CONTENTS);
                REQUIRE(tokenContents == "Simple Category 1:");
            }
        }
    }
}

SCENARIO("A Category's Choice should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the Lexer consumes the input,") {
            TSLLexer lexer;
            lexer.load(tslInput);
            THEN("the Lexer should return the Category's Choice contents as a string.") {
                // A Category is detected first, and a Choice is part of a Category,
                // so we have to skip the first Token we see to get the first
                // Choice.
                lexer.getNextToken();

                auto lexerToken = lexer.getNextToken();
                REQUIRE(lexerToken == yy::parser::token::CHOICE_CONTENTS);
                REQUIRE(lexer.getCurrentTokenContents() == "Simple Choice 1.");
            }
        }
    }
}

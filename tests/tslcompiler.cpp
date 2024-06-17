#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "compiler.hpp"

namespace fs = std::filesystem;

SCENARIO("A Category should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the TSLCompiler's Lexer consumes the input,") {
            TSLCompiler compiler;
            compiler.load(tslInput);
            THEN("the Lexer from the TSLCompiler should return the Category contents as a string.") {
                REQUIRE(compiler.getNextToken() == "Simple Category 1:");
            }
        }
    }
}

SCENARIO("A Comment should not be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the TSLCompiler's Lexer consumes the input,") {
            TSLCompiler compiler;
            compiler.load(tslInput);
            THEN("the Lexer from the TSLCompiler should not return the Category's Comment contents as a string.") {
                auto lexerToken = compiler.getNextToken();
                REQUIRE(lexerToken != "# Comments are quite helpful as reminders.");
                REQUIRE(lexerToken == "Simple Category 1:");
            }
        }
    }
}

SCENARIO("A Category's Choice should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the TSLCompiler's Lexer consumes the input,") {
            TSLCompiler compiler;
            compiler.load(tslInput);
            THEN("the Lexer from the TSLCompiler should return the Category's Choice contents as a string.") {
                // A Category is detected first, and a Choice is part of a Category,
                // so we have to skip the first Token we see to get the first
                // Choice.
                compiler.getNextToken();
                REQUIRE(compiler.getNextToken() == "Simple Choice 1.");
            }
        }
    }
}

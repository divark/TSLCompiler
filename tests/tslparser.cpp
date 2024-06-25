#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "tsl_lexer.hpp"
#include "tsl_collector.hpp"
#include "parser.hpp"

namespace fs = std::filesystem;

SCENARIO("A Category should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLLexer lexer;
            lexer.load(tslInput);

            TSLCollector collector;

            yy::parser parser(lexer, collector);
            parser();

            THEN("the Parse Tree should contain one Category named \"Simple Category 1:\".") {
                REQUIRE(collector.categories.size() == 1);
                REQUIRE(collector.categories[0] == "Simple Category 1:");
            }
        }
    }
}

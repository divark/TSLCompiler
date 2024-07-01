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

            THEN("the Parse Tree should contain the contents of the Category.") {
                REQUIRE(collector.categories.size() == 1);
                REQUIRE(collector.categories[0] == "Simple Category 1:");
            }
        }
    }
}

SCENARIO("A Choice should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLLexer lexer;
            lexer.load(tslInput);

            TSLCollector collector;

            yy::parser parser(lexer, collector);
            parser();

            THEN("the Parse Tree should contain the contents of the Choice.") {
                REQUIRE(collector.choices.size() == 1);
                REQUIRE(collector.choices[0] == "Simple Choice 1.");
		REQUIRE(collector.categoryChoicesGraph[0][0] == 0);
            }
        }
    }
}

SCENARIO("Both Categories and Choices should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with two Categories, and one Choice each,") {
        fs::path tslInput = "tests/two_categories_one_choice_each.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLLexer lexer;
            lexer.load(tslInput);

            TSLCollector collector;

            yy::parser parser(lexer, collector);
            parser();

            THEN("the Parse Tree should contain the contents of both Categories,") {
		REQUIRE(collector.categories.size() == 2);
		REQUIRE(collector.categories[0] == "Simple Category 1:");
		REQUIRE(collector.categories[1] == "Simple Category 2:");
            }

	    THEN("the Parse Tree should contain the contents of both Choices.") {
		REQUIRE(collector.choices.size() == 2);
		REQUIRE(collector.choices[0] == "Simple Choice 1.");
		REQUIRE(collector.choices[1] == "Simple Choice 2.");

		REQUIRE(collector.categoryChoicesGraph.size() == 2);

		REQUIRE(collector.categoryChoicesGraph[0].size() == 1);
		REQUIRE(collector.categoryChoicesGraph[0][0] == 0);

		REQUIRE(collector.categoryChoicesGraph[1].size() == 1);
		REQUIRE(collector.categoryChoicesGraph[1][0] == 1);
	    }
        }
    }
}

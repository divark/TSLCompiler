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

SCENARIO("A single marking from a Choice should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with a Category, Choice, and one single marking constraint,") {
        fs::path tslInput = "tests/one_category_choice_with_marking.txt";
        WHEN("the Lexer consumes the input,") {
            TSLLexer lexer;
            lexer.load(tslInput);
            THEN("the Lexer should detect the Choice's single Marking.") {
                // To get to the Constraints, we have to ignore the tokens found for the Category
                // and Choice found first.
                for (int i = 0; i < 2; i++) {
                    lexer.getNextToken();
                }

		// We also want to skip the Constraint start and end tokens.
		lexer.getNextToken();

                auto lexerToken = lexer.getNextToken();
                REQUIRE(lexerToken == yy::parser::token::MARKING_SINGLE);
            }
        }
    }
}

SCENARIO("An error marking from a Choice should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with a Category, Choice, and one error marking constraint,") {
        fs::path tslInput = "tests/one_category_choice_with_error_marking.txt";
        WHEN("the Lexer consumes the input,") {
            TSLLexer lexer;
            lexer.load(tslInput);
            THEN("the Lexer should detect the Choice's error Marking.") {
                // To get to the Constraints, we have to ignore the tokens found for the Category
                // and Choice found first.
                for (int i = 0; i < 2; i++) {
                    lexer.getNextToken();
                }

                // We also want to skip the Constraint start and end tokens.
                lexer.getNextToken();

                auto lexerToken = lexer.getNextToken();
                REQUIRE(lexerToken == yy::parser::token::MARKING_ERROR);
            }
        }
    }
}

SCENARIO("A Property from a Choice should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, Choice, and a single Property,") {
        fs::path tslInput = "tests/choice_with_one_property.txt";
        WHEN("the Lexer consumes the input,") {
            TSLLexer lexer;
            lexer.load(tslInput);
            THEN("the Lexer should return the Property's contents as a string.") {
                // To get to the Constraints, we have to ignore the tokens found for the Category
                // and Choice found, and ignoring the beginning of the Constraint characters.
                for (int i = 0; i < 3; i++) {
                    lexer.getNextToken();
                }

                auto propertyListToken = lexer.getNextToken();
                REQUIRE(propertyListToken == yy::parser::token::PROPERTY_LIST);

                auto propertyElementToken = lexer.getNextToken();
                REQUIRE(propertyElementToken == yy::parser::token::PROPERTY_ELEMENT);
                REQUIRE(lexer.getCurrentTokenContents() == "ABC");
            }
        }
    }
}

SCENARIO("Multiple Properties from a Choice should be returned from the Lexer from a valid TSL file.") {
    GIVEN("a TSL input with one Category, Choice, and multiple Properties,") {
        fs::path tslInput = "tests/choice_with_multiple_properties.txt";
        WHEN("the Lexer consumes the input,") {
            TSLLexer lexer;
            lexer.load(tslInput);
            THEN("the Lexer should return each Property's contents as a string.") {
                // To get to the Constraints, we have to ignore the tokens found for the Category
                // and Choice found, and ignoring the beginning of the Constraint characters.
                for (int i = 0; i < 3; i++) {
                    lexer.getNextToken();
                }

                auto propertyListToken = lexer.getNextToken();
                REQUIRE(propertyListToken == yy::parser::token::PROPERTY_LIST);

                std::string properties[] = {"A,", "B,", "C"};
                int numProperties = 3;
                for (int i = 0; i < numProperties; i++) {
                    auto propertyElementToken = lexer.getNextToken();
                    REQUIRE(propertyElementToken == yy::parser::token::PROPERTY_ELEMENT);
                    REQUIRE(lexer.getCurrentTokenContents() == properties[i]);
                }
            }
        }
    }
}

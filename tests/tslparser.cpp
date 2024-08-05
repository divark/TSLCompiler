#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "tsl_parser.hpp"

namespace fs = std::filesystem;

SCENARIO("A Category should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Parse Tree should contain the contents of the Category.") {
                REQUIRE(parser.collector.categories.size() == 1);
                REQUIRE(parser.collector.categories[0] == "Simple Category 1:");
            }
        }
    }
}

SCENARIO("A Choice should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Parse Tree should contain the contents of the Choice.") {
                REQUIRE(parser.collector.choices.size() == 1);
                REQUIRE(parser.collector.choices[0] == "Simple Choice 1.");
		REQUIRE(parser.collector.categoryChoicesGraph[0][0] == 0);
            }
        }
    }
}

SCENARIO("Both Categories and Choices should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with two Categories, and one Choice each,") {
        fs::path tslInput = "tests/two_categories_one_choice_each.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Parse Tree should contain the contents of both Categories,") {
		REQUIRE(parser.collector.categories.size() == 2);
		REQUIRE(parser.collector.categories[0] == "Simple Category 1:");
		REQUIRE(parser.collector.categories[1] == "Simple Category 2:");
            }

	    THEN("the Parse Tree should contain the contents of both Choices.") {
		REQUIRE(parser.collector.choices.size() == 2);
		REQUIRE(parser.collector.choices[0] == "Simple Choice 1.");
		REQUIRE(parser.collector.choices[1] == "Simple Choice 2.");

		REQUIRE(parser.collector.categoryChoicesGraph.size() == 2);

		REQUIRE(parser.collector.categoryChoicesGraph[0].size() == 1);
		REQUIRE(parser.collector.categoryChoicesGraph[0][0] == 0);

		REQUIRE(parser.collector.categoryChoicesGraph[1].size() == 1);
		REQUIRE(parser.collector.categoryChoicesGraph[1][0] == 1);
	    }
        }
    }
}

SCENARIO("A single marking for a Choice should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with a Category, Choice, and one single marking constraint,") {
        fs::path tslInput = "tests/one_category_choice_with_marking.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Collector should contain the Choice's single marking.") {
                int choiceIdx = 0;
                REQUIRE(parser.collector.singleMarkings[choiceIdx] == true);
            }
        }
    }
}

SCENARIO("A error marking for a Choice should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with a Category, Choice, and one error marking constraint,") {
        fs::path tslInput = "tests/one_category_choice_with_error_marking.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Collector should contain the Choice's error marking.") {
                int choiceIdx = 0;
                REQUIRE(parser.collector.errorMarkings[choiceIdx] == true);
            }
        }
    }
}

SCENARIO("A single and error marking for one Choice each should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with a Category, one Choice with a single marking, and one Choice with an error marking,") {
        fs::path tslInput = "tests/one_category_choices_with_single_and_error_markings.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Collector should contain the first Choice's single marking.") {
                int choiceIdx = 0;
                REQUIRE(parser.collector.singleMarkings[choiceIdx] == true);
            }

            THEN("the Collector should contain the second Choice's error marking.") {
                int choiceIdx = 1;
                REQUIRE(parser.collector.errorMarkings[choiceIdx] == true);
            }
        }
    }
}

SCENARIO("One Property for a Choice should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with one Category, Choice, and a single Property,") {
        fs::path tslInput = "tests/choice_with_one_property.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Collector should contain the Choice's Property.") {
                int choiceIdx = 0;
                int propertyIdx = 0;
                REQUIRE(parser.collector.properties[propertyIdx] == "ABC");
                REQUIRE(parser.collector.choiceProperties[choiceIdx][0] == propertyIdx);
            }
        }
    }
}

SCENARIO("Multiple Properties for a Choice should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with one Category, Choice, and multiple Properties,") {
        fs::path tslInput = "tests/choice_with_multiple_properties.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            parser.run();

            THEN("the Collector should contain the Choice's Properties.") {
                int choiceIdx = 0;
                int property_idx[] = {0, 1, 2};
                std::string property[] = {"A", "B", "C"};

                REQUIRE(parser.collector.properties.size() == 3);
                for (int propertyIdx : property_idx) {
                    REQUIRE(parser.collector.properties[propertyIdx] == property[propertyIdx]);
                    REQUIRE(parser.collector.choiceProperties[choiceIdx][propertyIdx] == propertyIdx);
                }
            }
        }
    }
}

SCENARIO("A Simple Expression from an If Statement should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with two Categories, one with a Choice with a Property, and another with a Choice with an If Statement,") {
        fs::path tslInput = "tests/choices_with_simple_if.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            auto parserStatus = parser.run();

            THEN("the Collector should contain the Choice's Simple Expression.") {
                REQUIRE(parserStatus == 0);

                int choiceIdx = 1;
                std::string expectedExpression = "simple";

                REQUIRE(parser.collector.choiceExpressions.size() == 2);
                REQUIRE(parser.collector.getChoiceExpression(choiceIdx)->asString() == expectedExpression);
            }
        }
    }
}

SCENARIO("An Negation Expression from an If Statement should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with two Categories, one with a Choice with a Property, and another with a Choice with an If Statement containing a negated expression,") {
        fs::path tslInput = "tests/choice_with_unary_expression.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            auto parserStatus = parser.run();

            THEN("the Collector should contain the Choice's Negated Expression.") {
                REQUIRE(parserStatus == 0);

                int choiceIdx = 1;
                std::string expectedExpression = "!simple";

                REQUIRE(parser.collector.choiceExpressions.size() == 2);
                REQUIRE(parser.collector.getChoiceExpression(choiceIdx)->asString() == expectedExpression);
            }
        }
    }
}

SCENARIO("A Grouped Expression from an If Statement should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with two Categories, one with a Choice with a Property, and another with a Choice with an If Statement containing a grouped expression,") {
        fs::path tslInput = "tests/choice_with_grouped_expression.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            auto parserStatus = parser.run();

            THEN("the Collector should contain the Choice's Grouped Expression.") {
                REQUIRE(parserStatus == 0);

                int choiceIdx = 1;
                std::string expectedExpression = "(simple)";

                REQUIRE(parser.collector.choiceExpressions.size() == 2);
                REQUIRE(parser.collector.getChoiceExpression(choiceIdx)->asString() == expectedExpression);
            }
        }
    }
}

SCENARIO("An AND Expression from an If Statement should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with three Categories with one Choice each, where the first two Choices contain a property, and the last an If Statement with an AND Expression,") {
        fs::path tslInput = "tests/choice_with_and_expression.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            auto parserStatus = parser.run();

            THEN("the Collector should contain the Choice's AND Expression.") {
                REQUIRE(parserStatus == 0);

                int choiceIdx = 2;
                std::string expectedExpression = "choice1 && choice2";

                REQUIRE(parser.collector.choiceExpressions.size() == 3);
                REQUIRE(parser.collector.getChoiceExpression(choiceIdx)->asString() == expectedExpression);
            }
        }
    }
}

SCENARIO("An OR Expression from an If Statement should be found in the Parse Tree from a valid TSL file.") {
    GIVEN("Given a TSL input with three Categories with one Choice each, where the first two Choices contain a property, and the last an If Statement with an OR Expression,") {
        fs::path tslInput = "tests/choice_with_or_expression.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            auto parserStatus = parser.run();

            THEN("the Collector should contain the Choice's OR Expression.") {
                REQUIRE(parserStatus == 0);

                int choiceIdx = 2;
                std::string expectedExpression = "choice1 || choice2";

                REQUIRE(parser.collector.choiceExpressions.size() == 3);
                REQUIRE(parser.collector.getChoiceExpression(choiceIdx)->asString() == expectedExpression);
            }
        }
    }
}

SCENARIO("Properties from an If Expression are found in the Parse Tree from a valid TSL file.") {
    GIVEN("a TSL input with two Categories, one with a Choice with a Property, and another with a Choice defining a Property in the If Statement,") {
        fs::path tslInput = "tests/choice_with_property_in_if.txt";
        WHEN("the Parser converts the TSL input into a Parse Tree,") {
            TSLParser parser(tslInput);
            auto parserStatus = parser.run();
            THEN("the Collector should contain the Choice's If Properties.") {
                REQUIRE(parserStatus == 0);

                int choiceIdx = 1;
                REQUIRE(parser.collector.choiceIfProperties[choiceIdx].size() == 1);
                REQUIRE(parser.collector.choiceProperties[choiceIdx].size() == 0);
            }
        }
    }
}

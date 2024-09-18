#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "tsl_compiler.hpp"
#include "tsl_testcase.hpp"
#include "tsl_choice_graph.hpp"

namespace fs = std::filesystem;

SCENARIO("A user provides a file containing a Category and a Choice.") {
    GIVEN("a TSL file containing one Category and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the input is consumed by the TSLCompiler,") {
            TSLCompiler compiler(tslInput);
            REQUIRE(compiler.compile() == 0);
            THEN("the TSLCompiler's result should contain one test case mentioning the Category and Choice.") {
                auto testCases = compiler.getTestCases();
                REQUIRE(testCases.size() == 1);

                auto firstTestCase = testCases[0];
                REQUIRE(firstTestCase.getTestCaseNumber() == 1);
                REQUIRE(firstTestCase.getNumCategories() == 1);
                REQUIRE(firstTestCase.getCategoryChoice("Simple Category 1") == "Simple Choice 1");
            }
        }
    }
}

SCENARIO("A single Node should be made from a file containing one Category, and one Choice.") {
    GIVEN("a TSL file containing one Category, and one Choice,") {
        fs::path tslInput = "tests/one_category_one_choice.txt";
        WHEN("the input is compiled by the TSLCompiler,") {
            TSLCompiler compiler(tslInput);
            REQUIRE(compiler.compile() == 0);
            WHEN("the nodes are created from the TSLCollector,") {
                auto collected_tsl_variables = compiler.getCollector();
                std::vector<Node> nodes = getNodesFromCollector(collected_tsl_variables);

                THEN("the number of Nodes should match the number of Choices,") {
                    REQUIRE(nodes.size() == 1);
                }

                THEN("Node 1 should contain the first Category,") {
                    auto firstNode = nodes[0];
                    auto firstNodeData = firstNode.getData();

                    REQUIRE(firstNodeData.getCategory() == "Simple Category 1");
                }

                THEN("Node 1 should contain the first Choice.") {
                    auto firstNode = nodes[0];
                    auto firstNodeData = firstNode.getData();

                    REQUIRE(firstNodeData.getChoice() == "Simple Choice 1");
                }
            }
        }
    }
}

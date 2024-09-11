#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "tsl_compiler.hpp"
#include "tsl_testcase.hpp"

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

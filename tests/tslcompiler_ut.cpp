#include <boost/ut.hpp>
#include <fstream>
#include <string>
#include <format>

#include "tsl_compiler.hpp"

namespace fs = std::filesystem;

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = std::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("The TSLCompiler should generate test frames.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("a TSL file called {file_name}") = [&](std::string file_name) {
              fs::path tslInput = std::format("tests/{}", file_name);

              steps.when("the input is consumed by the TSLCompiler,") = [&] {
                  TSLCompiler compiler(tslInput);
                  expect_eq(0, compiler.compile());

                  steps.then("the TSLCompiler's result should contain {expected_num_test_cases} test cases.") = [&](int expected_num_test_cases) {
                      auto actual_num_test_cases = compiler.getTestCases().size();
                      expect_eq(expected_num_test_cases, actual_num_test_cases);
                  };

                  steps.then("test case {testCaseNum} should have '{expected_category}' as category {category_num}.") = [&](int testCaseNum, std::string expected_category, int category_num) {
                      auto actual_category = compiler.getTestCases()[testCaseNum - 1].getCategories()[category_num - 1];
                      expect_eq(expected_category, actual_category);
                  };

                  steps.then("test case {testCaseNum} should have '{expected_choice}' chosen in category {category_num}.") = [&](int testCaseNum, std::string expected_choice, int category_num) {
                      auto foundCategory = compiler.getTestCases()[testCaseNum - 1].getCategories()[testCaseNum - 1];
                      auto actual_choice = compiler.getTestCases()[testCaseNum - 1].getCategoryChoice(foundCategory);
                      expect_eq(expected_choice, actual_choice);
                  };
              };
          };
      };
    };
  };

    const auto file = [](const auto path) {
        std::ifstream file{path};
        return std::string{(std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>()};
    };

    "tslcompiler"_test = steps | file(argv[1]);
}

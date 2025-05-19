#include <boost/ut.hpp>
#include <fstream>
#include <string>
#include <fmt/format.h>

#include "tsl_parser.hpp"

namespace fs = std::filesystem;

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = fmt::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("The Parser handles valid TSL input.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("a TSL file called {file_name}") = [&](std::string file_name) {
              fs::path tslInput = fmt::format("test-files/{}", file_name);

              steps.when("the Parser collects the variables from the TSL input,") = [&] {
                  TSLParser parser(tslInput);
                  parser.run();

                  steps.then("Category {categoryNum} should be labeled as '{expectedCategoryLabel}'.") = [&](size_t categoryNum, std::string expectedCategoryLabel) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);

                      std::string actualCategoryLabel = category.getLabel();
                      expect_eq(expectedCategoryLabel, actualCategoryLabel);
                  };

                  steps.then("Choice {choiceNum} from Category {categoryNum} should be labeled as '{expectedChoiceLabel}'.") = [&](size_t choiceNum, size_t categoryNum, std::string expectedChoiceLabel) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);
                      auto choice = category.getChoice(choiceNum - 1);

                      std::string actualChoiceLabel = choice.getLabel();
                      expect_eq(expectedChoiceLabel, actualChoiceLabel);
                  };

                  steps.then("Choice {choiceNum} from Category {categoryNum} should contain a {expectedMarkerType} marking.") = [&](size_t choiceNum, size_t categoryNum, std::string typeOfMarking) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);
                      auto choice = category.getChoice(choiceNum - 1);

                      std::string actualMarkerType = choice.getMarker().as_string();
                      expect_eq(expectedMarkerType, actualMarkerType);
                  };

                  steps.then("Choice {choiceNum} from Category {categoryNum} should contain '{expectedPropertyLabel}' as Property {propertyNum}.") = [&](size_t choiceNum, size_t categoryNum, std::string expectedPropertyContents, size_t propertyNum) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);
                      auto choice = category.getChoice(choiceNum - 1);
                      auto choiceProperties = choice.getProperties();

                      std::string actualPropertyLabel = choiceProperties[propertyNum - 1];
                      expect_eq(expectedPropertyLabel, actualPropertyLabel);
                  };

                  steps.then("Choice {choiceNum} from Category {categoryNum} should contain the '{expectedExpression}' Expression.") = [&](size_t choiceNum, size_t categoryNum, std::string expectedExpression) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);
                      auto choice = category.getChoice(choiceNum - 1);
                      auto choiceExpression = choice.getExpression();

                      std::string actualExpression = choiceExpression.as_string();
                      expect_eq(expectedExpression, actualExpression);
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

    "parser"_test = steps | file(argv[1]);
}

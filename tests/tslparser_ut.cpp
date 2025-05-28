#include <boost/ut.hpp>
#include <fstream>
#include <string>
#include <fmt/format.h>
#include <unordered_set>

#include "tsl_grammar.hpp"
#include "tsl_parser.hpp"

namespace fs = std::filesystem;

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = fmt::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

std::unordered_set<std::string> parseProperties(const std::string& properties) {
    std::unordered_set<std::string> foundProperties;

    std::stringstream propertiesStream(properties);
    std::string foundProperty = "";

    while (std::getline(propertiesStream, foundProperty, ',')) {
        foundProperties.insert(foundProperty);
    }

    return foundProperties;
}

std::string propertyTypeToString(const EvaluationType& propertyType) {
    if (propertyType == EvaluationType::If) {
        return "if";
    } else {
        return "else";
    }
}

Marker parseMarkerType(const std::string& markerString) {
    if (markerString == "single") {
        return Marker::Single;
    } else if (markerString == "error") {
        return Marker::Error;
    }

    return Marker::None;
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

                  steps.when("Choice {choiceNum} from Category {categoryNum} gets evaluated properties from found properties '{properties}',") = [&](size_t choiceNum, size_t categoryNum, std::string properties) {
                      auto& category = parser.getCollector().getCategory(categoryNum - 1);
                      auto& choice = category.getChoice(choiceNum - 1);

                      auto propertiesExtracted = parseProperties(properties);
                      auto foundProperties = choice.getEvaluatedProperties(propertiesExtracted).value();

                      steps.then("'{expectedProperty}' should be in the {expectedPropertiesType} properties.") = [&](std::string expectedProperty, std::string expectedPropertiesType) {
                          auto actualPropertiesType = propertyTypeToString(foundProperties.getType());
                          expect_eq(expectedPropertiesType, actualPropertiesType);

                          bool actualPropertyFound = foundProperties.containsProperty(expectedProperty);
                          expect_eq(true, actualPropertyFound);
                      };

                      steps.then("a {expectedMarkerTypeStr} marker should be in the {expectedPropertiesType} properties.") = [&](std::string expectedMarkerTypeStr, std::string expectedPropertiesType) {
                          auto actualPropertiesType = propertyTypeToString(foundProperties.getType());
                          expect_eq(expectedPropertiesType, actualPropertiesType);

                          auto expectedMarkerType = parseMarkerType(expectedMarkerTypeStr);
                          bool actualMarkerFound = foundProperties.containsMarker(expectedMarkerType);
                          expect_eq(true, actualMarkerFound);
                      };

                      steps.then("there should be no else properties.") = [&] {
                          auto actualPropertiesType = propertyTypeToString(foundProperties.getType());
                          expect_eq("else", actualPropertiesType);

                          expect_eq(true, foundProperties.is_empty());
                      };
                  };

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

                  steps.then("Choice {choiceNum} from Category {categoryNum} should contain a {expectedMarkerType} marking.") = [&](size_t choiceNum, size_t categoryNum, std::string expectedMarkerType) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);
                      auto choice = category.getChoice(choiceNum - 1);

                      std::string actualMarkerType = choice.getMarker().value();
                      expect_eq(expectedMarkerType, actualMarkerType);
                  };

                  steps.then("Choice {choiceNum} from Category {categoryNum} should contain '{expectedPropertyLabel}' as Property {propertyNum}.") = [&](size_t choiceNum, size_t categoryNum, std::string expectedPropertyLabel, size_t propertyNum) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);
                      auto choice = category.getChoice(choiceNum - 1);

                      std::string actualPropertyLabel = choice.getProperty(propertyNum - 1).asString();
                      expect_eq(expectedPropertyLabel, actualPropertyLabel);
                  };

                  steps.then("Choice {choiceNum} from Category {categoryNum} should contain the '{expectedExpression}' Expression.") = [&](size_t choiceNum, size_t categoryNum, std::string expectedExpression) {
                      auto category = parser.getCollector().getCategory(categoryNum - 1);
                      auto choice = category.getChoice(choiceNum - 1);
                      auto choiceExpression = choice.getExpression();

                      std::string actualExpression = choiceExpression.value()->asString();
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

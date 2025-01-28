#include <boost/ut.hpp>
#include <fstream>
#include <string>
#include <format>

#include "tsl_parser.hpp"

namespace fs = std::filesystem;

/**
* Returns a specific Choice's properties depending on the type of Property asked.
*/
std::vector<int> getChoicePropertiesDependingOn(const std::string& typeOfProperty, size_t choiceNum, TSLCollector& variableCollection) {
    std::vector<int> choiceProperties;

    if (typeOfProperty == "If") {
        choiceProperties = variableCollection.choiceIfProperties[choiceNum - 1];
    } else if (typeOfProperty == "Else") {
        choiceProperties = variableCollection.choiceElseProperties[choiceNum - 1];
    }

    return choiceProperties;
}

/**
* Returns true if the specific Choice contains the type of marking, or
* false otherwise.
*/
bool containsMarkingDependingOn(const std::string& typeOfMarking, size_t choiceNum, TSLParser& parser) {
    bool containsMarking = false;

    auto variableCollection = parser.getCollector();
    if (typeOfMarking == "single") {
        containsMarking = variableCollection.singleMarkings[choiceNum - 1];
    } else if (typeOfMarking == "error") {
        containsMarking = variableCollection.errorMarkings[choiceNum - 1];
    }

    return containsMarking;
}

/**
* Returns the contents found for specific Category or Choice from the
* parser.
*/
std::string getContentsDependingOn(const std::string& typeOfItem, const size_t typeNum, TSLParser& parser) {
    std::string contentsFound = "";

    auto variableCollection = parser.getCollector();
    if (typeOfItem == "Category") {
        contentsFound = variableCollection.categories[typeNum - 1];
    } else if (typeOfItem == "Choice") {
        contentsFound = variableCollection.choices[typeNum - 1];
    } else {
        throw "getContentsDependingOn: Unsupported typeOfItem: " + typeOfItem;
    }

    return contentsFound;
}

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = std::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("The Parser handles valid TSL input.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("a TSL file called {file_name}") = [&](std::string file_name) {
              fs::path tslInput = std::format("tests/{}", file_name);

              steps.when("the Parser collects the variables from the TSL input,") = [&] {
                  TSLParser parser(tslInput);
                  parser.run();

                  steps.then("the Collector should contain {typeOfItem} {typeNum} as '{expectedContents}'.") = [&](std::string typeOfItem, size_t typeNum, std::string expectedContents) {
                      std::string actualContents = getContentsDependingOn(typeOfItem, typeNum, parser);
                      expect_eq(expectedContents, actualContents);
                  };

                  steps.then("the Collector should contain a {typeOfMarking} marking for Choice {choiceNum}") = [&](std::string typeOfMarking, size_t choiceNum) {
                      bool choiceContainsMarking = containsMarkingDependingOn(typeOfMarking, choiceNum, parser);
                      boost::ut::expect(choiceContainsMarking);
                  };

                  steps.then("the Collector should contain Property {propertyNum} in Choice {choiceNum} as '{expectedPropertyContents}'.") = [&](size_t propertyNum, size_t choiceNum, std::string expectedPropertyContents) {
                      auto variableCollection = parser.getCollector();

                      auto choiceProperties = variableCollection.choiceProperties[choiceNum - 1];
                      bool choiceContainsPropertyIdx = std::find(choiceProperties.begin(), choiceProperties.end(), propertyNum - 1) != choiceProperties.end();
                      boost::ut::expect(choiceContainsPropertyIdx);

                      std::string actualPropertyContents = variableCollection.properties[propertyNum - 1];
                      expect_eq(expectedPropertyContents, actualPropertyContents);
                  };

                  steps.then("the Collector should contain Property {propertyNum} in an {typeOfProperty} statement in Choice {choiceNum} as '{expectedPropertyContents}'.") = [&](size_t propertyNum, std::string typeOfProperty, size_t choiceNum, std::string expectedPropertyContents) {
                      auto variableCollection = parser.getCollector();

                      auto choiceProperties = getChoicePropertiesDependingOn(typeOfProperty, choiceNum, variableCollection);//variableCollection.choiceProperties[choiceNum - 1];
                      bool choiceContainsPropertyIdx = std::find(choiceProperties.begin(), choiceProperties.end(), propertyNum - 1) != choiceProperties.end();
                      boost::ut::expect(choiceContainsPropertyIdx);

                      std::string actualPropertyContents = variableCollection.properties[propertyNum - 1];
                      expect_eq(expectedPropertyContents, actualPropertyContents);
                  };

                  steps.then("the Collector should contain a {typeOfExpression} Expression in Choice {choiceNum} as '{expectedExpressionString}'.") = [&](std::string typeOfExpression, size_t choiceNum, std::string expectedExpressionString) {
                      std::string actualExpressionString = parser.getCollector().getChoiceExpression(choiceNum - 1)->asString();
                      expect_eq(expectedExpressionString, actualExpressionString);
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

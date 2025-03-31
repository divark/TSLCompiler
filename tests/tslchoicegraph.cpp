#include <boost/ut.hpp>
#include <fstream>
#include <string>
#include <fmt/format.h>

#include "tsl_choice_graph.hpp"
#include "tsl_compiler.hpp"

namespace fs = std::filesystem;

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = fmt::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("A Directed Graph should be derived from the TSLCompiler.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("a TSL file called {file_name}") = [&](std::string file_name) {
              fs::path tslInput = fmt::format("test-files/{}", file_name);

              steps.when("the input is consumed by the TSLCompiler,") = [&] {
                  TSLCompiler compiler(tslInput);
                  expect_eq(0, compiler.compile());

                  steps.when("the nodes and edges are created from the TSLCollector,") = [&] {
                      auto nodes = getNodesFromCollector(compiler.getCollector());
                      auto edges = getEdgesFromTSLNodes(nodes, compiler.getCollector());

                      steps.then("Node {firstNodeNum} should have an edge to Node {nextNodeNum}.") = [&](size_t firstNodeNum, size_t nextNodeNum) {
                          auto firstNode = nodes[firstNodeNum - 1];
                          auto firstNodeEdges = edges.getNodeEdges(firstNode);

                          auto hasNextNodeNum = std::find(firstNodeEdges.begin(), firstNodeEdges.end(), nextNodeNum - 1) != firstNodeEdges.end();
                          expect(hasNextNodeNum);
                      };

                      steps.then("the number of Nodes should match the number of Choices.") = [&] {
                          auto expectedNumNodes = compiler.getCollector().choices.size();
                          auto actualNumNodes = nodes.size();
                          expect_eq(expectedNumNodes, actualNumNodes);
                      };

                      steps.then("Node {nodeNum} should contain '{expectedCategory}' as the Category.") = [&](size_t nodeNum, std::string expectedCategory) {
                          auto chosenNode = nodes[nodeNum - 1];
                          auto nodeCategoryIdx = chosenNode.getData().getCategoryIdx();

                          auto actualCategory = compiler.getCollector().categories[nodeCategoryIdx];
                          expect_eq(expectedCategory, actualCategory);
                      };

                      steps.then("Node {nodeNum} should contain '{expectedChoice}' as the Choice.") = [&](size_t nodeNum, std::string expectedChoice) {
                          auto chosenNode = nodes[nodeNum - 1];
                          auto nodeChoiceIdx = chosenNode.getData().getChoiceIdx();

                          auto actualChoice = compiler.getCollector().choices[nodeChoiceIdx];
                          expect_eq(expectedChoice, actualChoice);
                      };
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

    "choice_graph"_test = steps | file(argv[1]);
}

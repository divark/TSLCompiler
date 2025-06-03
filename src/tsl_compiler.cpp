#include "tsl_compiler.hpp"
#include "tsl_choice_graph.hpp"
#include "tsl_testcase.hpp"
#include <memory>

/**
 * Returns a TSLCompiler with the loaded inputFile.
 */
TSLCompiler::TSLCompiler(const std::filesystem::path& inputFile) {
    parser = std::make_unique<TSLParser>(inputFile);
}

/**
 * Returns a reference to the generated test cases so far.
 */
std::vector<TSLTestCase>& TSLCompiler::getTestCases() {
    return generatedTestCases;
}

/**
 * Returns the collection of variables found after compiling.
 */
TSLCollector& TSLCompiler::getCollector() {
    return parser->getCollector();
}

/**
 * Runs the Compiler to generate test cases.
 */
int TSLCompiler::compile() {
    auto programStatus = parser->run();

    auto tslGraph = TSLGraph(parser->getCollector());

    // All Choices labeled with a Marker is considered an edge case, thus
    // these are processed first and separately.
    // NOTE: This does not exclude conditional Markers. Conditional Markers
    // are edge cases with relation to some other property that exists. This
    // means these Choice's edge cases can show up multiple times.
    auto nodesWithMarkers = filterToNodesWithMarkers(tslGraph.getNodes());
    for (auto categoryNode : nodesWithMarkers) {
        tslGraph.visitDFS(categoryNode);
    }

    auto firstCategory = parser->getCollector().getCategory(0);
    auto firstCategoryNodes = filterToNodesWithCategory(tslGraph.getNodes(), firstCategory);
    firstCategoryNodes = filterToNodesWithoutMarkers(firstCategoryNodes);
    for (auto categoryNode : firstCategoryNodes) {
        tslGraph.visitDFS(categoryNode);
    }

    generatedTestCases = tslGraph.getGeneratedTestCases();

    return programStatus;
}

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

    auto deadEndListener = std::make_shared<TestCaseListener>(parser->getCollector());
    auto symbolTableListener = std::make_shared<SymbolTableListener>(parser->getCollector());
    auto tslGraph = TSLGraph(parser->getCollector());
    tslGraph.addPreorderListener(symbolTableListener);
    tslGraph.addPreorderListener(deadEndListener);

    tslGraph.addPostorderListener(symbolTableListener);

    auto nodesWithMarkers = filterToNodesWithMarkers(tslGraph.getNodes());
    for (auto categoryNode : nodesWithMarkers) {
        tslGraph.visitDFS(categoryNode);
    }

    auto firstCategoryNodes = filterToNodesWithCategoryIdx(tslGraph.getNodes(), 0);
    for (auto categoryNode : firstCategoryNodes) {
        tslGraph.visitDFS(categoryNode);
    }

    generatedTestCases = deadEndListener->getTestCases();

    return programStatus;
}

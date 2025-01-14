#include "tsl_compiler.hpp"
#include "tsl_testcase.hpp"

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

    generatedTestCases = generateTestCases(parser->getCollector());

    return programStatus;
}

/**
* Derives test cases recorded from the results of the tsl input so far.
*/
std::vector<TSLTestCase> generateTestCases(TSLCollector &tslVariables) {
    std::vector<TSLTestCase> foundTestCases;

    foundTestCases.push_back(TSLTestCase());

    return foundTestCases;
}

#include "tsl_parser.hpp"

/**
 * Sets up the TSLParser to be ready to parse for some given input file.
 */
TSLParser::TSLParser(const std::filesystem::path& inputPath) {
    lexer.load(inputPath);

    parser = std::make_unique<yy::parser>(lexer, collector);
}

/**
 * Runs the Parser, returning the status code 
 * when it has read the whole file, or it has encountered an error.
 */
int TSLParser::run() {
    return parser->parse();
}

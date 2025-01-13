#include <filesystem>
#include <iostream>

#include "tsl_parser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid Number of arguments. Expected 2, found " << argc << std::endl;
        return 1;
    }

    std::filesystem::path inputFilePath(argv[argc - 1]);
    if (!std::filesystem::exists(inputFilePath)) {
        std::cerr << "File " << inputFilePath << " does not exist." << std::endl;
        return 1;
    }

    try {
        TSLParser parser(inputFilePath);
        return parser.run();
    } catch (yy::parser::syntax_error exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}

#include <boost/ut.hpp>
#include <bsm/audit.h>
#include <fstream>
#include <string>
#include <format>

#include "tsl_lexer.hpp"
#include "parser.hpp"

namespace fs = std::filesystem;

/**
* Returns all properties read from the Lexer.
* Preconditions: The Lexer is looking at a PROPERTY_LIST
*/
std::string readAllProperties(TSLLexer &lexer) {
    std::string foundProperties = "";

    while (true) {
        auto currentToken = lexer.getNextToken();
        if (currentToken != yy::parser::token_type::PROPERTY_ELEMENT) {
            break;
        }

        foundProperties += lexer.getCurrentTokenContents() + " ";
    }

    if (!foundProperties.empty()) {
        foundProperties.pop_back();
    }

    return foundProperties;
}

/**
* Returns a token type parsed from the given string.
*/
yy::parser::token_type parseTokenType(const std::string& tokenTypeAsked) {
    if (tokenTypeAsked == "Category") {
        return yy::parser::token_type::CATEGORY_CONTENTS;
    } else if (tokenTypeAsked == "Choice") {
        return yy::parser::token_type::CHOICE_CONTENTS;
    } else if (tokenTypeAsked == "Property List") {
        return yy::parser::token_type::PROPERTY_LIST;
    } else if (tokenTypeAsked == "Property Element") {
        return yy::parser::token_type::PROPERTY_ELEMENT;
    } else if (tokenTypeAsked == "Constraint") {
        return yy::parser::token_type::CONSTRAINT_START;
    } else if (tokenTypeAsked == "Constraint End") {
        return yy::parser::token_type::CONSTRAINT_END;
    } else if (tokenTypeAsked == "If") {
        return yy::parser::token_type::IF;
    }

    throw std::format("Unsupported token asked: {}", tokenTypeAsked);
}

/**
 * Skips all Tokens until finding the nth Desired Token.
 */
void waitUntil(TSLLexer &lexer, yy::parser::token::token_kind_type desiredToken, unsigned int nthToken) {
    unsigned int tokenFoundCount = 0;

    while (true) {
        auto currentTokenFound = lexer.getNextToken();
        if (currentTokenFound != desiredToken) {
            continue;
        }

        tokenFoundCount++;
        if (tokenFoundCount == nthToken) {
            break;
        }
    }
}

void expect_eq(auto expected, auto actual) {
    auto assertErrorMsg = std::format("Expected {},\n Found {}", expected, actual);
    boost::ut::expect(expected == actual) << assertErrorMsg;
}

void expect_ne(auto expected, auto actual) {
    auto assertErrorMsg = std::format("Expected not to find {},\n Found {}", expected, actual);
    boost::ut::expect(expected != actual) << assertErrorMsg;
}

int main(int argc, const char** argv) {
  using namespace boost::ut;

  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("The Lexer recognizes valid TSL input.") = [&] {
      steps.scenario("*") = [&] {
          steps.given("a TSL file called {file_name}") = [&](std::string file_name) {
              fs::path tslInput = std::format("tests/{}", file_name);

              steps.when("the Lexer consumes the input,") = [&] {
                  TSLLexer lexer;
                  lexer.load(tslInput);

                  steps.when("the Lexer reads up to '{tokenType}' {instanceNum}") = [&](std::string tokenTypeAsked, size_t instanceNum) {
                      auto tokenType = parseTokenType(tokenTypeAsked);
                      waitUntil(lexer, tokenType, instanceNum);
                  };

                  steps.when("the Lexer reads all properties from 'Property List' {instanceNum},") = [&](size_t instanceNum) {
                      waitUntil(lexer, yy::parser::token_type::PROPERTY_LIST, instanceNum);

                      std::string actualPropertiesFound = readAllProperties(lexer);

                      steps.then("the Properties should be '{expectedPropertiesFound}'.") = [&](std::string expectedPropertiesFound) {
                          expect_eq(expectedPropertiesFound, actualPropertiesFound);
                      };
                  };

                  steps.then("the Lexer should have retrieved '{expected_lexer_contents}'.") = [&](std::string expectedLexerContents) {
                      lexer.getNextToken();

                      auto actualLexerContents = lexer.getCurrentTokenContents();
                      expect_eq(expectedLexerContents, actualLexerContents);
                  };

                  steps.then("the Lexer should not have retrieved '{expected_lexer_contents}'.") = [&](std::string expectedLexerContents) {
                      lexer.getNextToken();

                      auto actualLexerContents = lexer.getCurrentTokenContents();
                      expect_ne(expectedLexerContents, actualLexerContents);
                  };

                  steps.then("the Lexer should be on Line Number {expectedLineNum}.") = [&](size_t expectedLineNum) {
                      auto actualLineNum = lexer.getLineNumber();
                      expect_eq(expectedLineNum, actualLineNum);
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

    "lexer"_test = steps | file(argv[1]);
}

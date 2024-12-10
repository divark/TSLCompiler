/* Prologue (Imports, Macros, etc) */
%code requires {
    #include <string>
    #include <format>

    #include "tsl_lexer.hpp"
    #include "tsl_collector.hpp"
}

%code {
    #define yylex lexer.constructNextToken
}

/* Bison Declarations (Terminal and Non-Terminal definitions) */
%require "3.2"
%language "c++"

%define api.value.type {int}
/*
* The following options help make descriptive error messages
* relating to syntax, according to the following resource:
* https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
*/
%define parse.trace
%define parse.error custom
%define parse.lac full
%locations

%parse-param {TSLLexer &lexer}
%parse-param {TSLCollector &collector}

%token CATEGORY_CONTENTS
%token CHOICE_CONTENTS

%token CONSTRAINT_START

%token IF
%token ELSE
%precedence LOGICAL_NOT
%token LOGICAL_GROUP_START
%token LOGICAL_GROUP_END
/* Since all Grammar rules are written in a left-associative style,
all Binary-based operations should have left-precedence. */
%left LOGICAL_AND
%left LOGICAL_OR

%token MARKING_SINGLE
%token MARKING_ERROR

%token PROPERTY_LIST
%token PROPERTY_ELEMENT

%token CONSTRAINT_END


/* Bison Grammar Rules */
%%

categories: categories category
          | category
          ;
category:   category_label choices
        ;
choices:    choices choice
       |    choice
       ;
choice:     choice_label constraints
      |     choice_label
      ;
constraints: label
          | if_statement else_statement { $$ = collector.markChoiceHasElse(); }
          | if_statement
          ;
if_statement: CONSTRAINT_START IF expression CONSTRAINT_END label { $$ = collector.convertPropertiesToIfProperties(); }
            | CONSTRAINT_START IF expression CONSTRAINT_END
            ;
else_statement: ELSE label { $$ = collector.convertPropertiesToElseProperties(); }
              | ELSE
              ;
expression: expression LOGICAL_AND expression { $$ = collector.recordBinaryExpression(ExpType::And); }
          | expression LOGICAL_OR expression  { $$ = collector.recordBinaryExpression(ExpType::Or);  }
          | LOGICAL_NOT expression { $$ = collector.recordUnaryExpression(ExpType::Negated); }
          | LOGICAL_GROUP_START expression LOGICAL_GROUP_END { $$ = collector.recordUnaryExpression(ExpType::Grouped); }
          | PROPERTY_ELEMENT { $$ = collector.recordSimpleExpression(lexer.getCurrentTokenContents()); }
          ;
label:  marking
     |  CONSTRAINT_START property_list CONSTRAINT_END
     ;
property_list:  PROPERTY_LIST property_elements
             ;
property_elements: property_element
                 | property_elements property_element
                 ;
property_element:   PROPERTY_ELEMENT { $$ = collector.recordProperty(lexer.getCurrentTokenContents()); }
marking:    MARKING_SINGLE { $$ = collector.markChoiceAsSingle(); }
       |    MARKING_ERROR  { $$ = collector.markChoiceAsError();  }
       ;

category_label:   CATEGORY_CONTENTS { $$ = collector.recordCategory(lexer.getCurrentTokenContents()); }
        ;
choice_label:     CHOICE_CONTENTS   { $$ = collector.recordChoice(lexer.getCurrentTokenContents()); }
      ;

%%

/* Bison Epilogue (Function declarations) */

void yy::parser::error(const location_type& l, const std::string &message) {
    std::cerr << "Parser failed at " << l << ": " << message << std::endl;
}

class FileReader {
    private:
        std::vector<std::string> inputLines;
    public:
        FileReader(std::string);

        bool hasLine(int);
        std::string getLine(uint);
};

FileReader::FileReader(std::string fileName) {
    std::ifstream inputContents(fileName);

    for (std::string line; std::getline(inputContents, line);) {
        inputLines.push_back(line);
    }
}

bool FileReader::hasLine(int lineNumber) {
    auto adjustedLineNumber = lineNumber - 1;
    return adjustedLineNumber > 0 && adjustedLineNumber < inputLines.size();
}

std::string FileReader::getLine(uint lineNumber) {
    return inputLines[lineNumber - 1];
}

std::string getPointingMsg(const yy::parser::location_type& location) {
    std::string pointingMsg = "";

    auto filePath = std::string(*location.end.filename);
    auto fileReader = FileReader(filePath);

    auto lineNumber = location.end.line;
    auto columnNumber = location.end.column;
    if (fileReader.hasLine(lineNumber - 1)) {
        pointingMsg += std::format("{:^3}| {}\n", lineNumber - 1, fileReader.getLine(lineNumber - 1));
    }

    pointingMsg += std::format("{:^3}| {}\n", lineNumber, fileReader.getLine(lineNumber));
    std::string errorPointer = "";
    for (auto i = 0; i < columnNumber; i++) {
        if (i + 1 == columnNumber) {
            errorPointer += "^";
            break;
        }

        errorPointer += "-";
    }
    pointingMsg += std::format("{:^3}| {}\n", "", errorPointer);

    if (fileReader.hasLine(lineNumber + 1)) {
        pointingMsg += std::format("{:^3}| {}\n", lineNumber + 1, fileReader.getLine(lineNumber + 1));
    }

    return pointingMsg;
}

std::string getTokenFixDescription(yy::parser::symbol_kind_type currentToken) {
    std::string tokenFixDescription = "";

    switch (currentToken) {
        case yy::parser::symbol_kind_type::S_CATEGORY_CONTENTS:
            tokenFixDescription = "Add a ':' at the end.";
            break;
        case yy::parser::symbol_kind_type::S_CHOICE_CONTENTS:
            tokenFixDescription = "Add a '.' at the end.";
            break;
        case yy::parser::symbol_kind_type::S_CONSTRAINT_START:
            tokenFixDescription = "Start this with a '['";
            break;
        case yy::parser::symbol_kind_type::S_IF:
            tokenFixDescription = "This should be replaced with 'if'";
            break;
        case yy::parser::symbol_kind_type::S_ELSE:
            tokenFixDescription = "This should be replaced with '[else]'";
            break;
        case yy::parser::symbol_kind_type::S_LOGICAL_NOT:
            tokenFixDescription = "Add a '!' at the start.";
            break;
        case yy::parser::symbol_kind_type::S_LOGICAL_GROUP_START:
            tokenFixDescription = "Add a '(' at the start.";
            break;
        case yy::parser::symbol_kind_type::S_LOGICAL_GROUP_END:
            tokenFixDescription = "Add a ')' at the end.";
            break;
        case yy::parser::symbol_kind_type::S_LOGICAL_AND:
            tokenFixDescription = "This should be replaced with '&&'";
            break;
        case yy::parser::symbol_kind_type::S_LOGICAL_OR:
            tokenFixDescription = "This should be replaced with '||'";
            break;
        case yy::parser::symbol_kind_type::S_MARKING_SINGLE:
            tokenFixDescription = "This should be replaced with '[single]'";
            break;
        case yy::parser::symbol_kind_type::S_MARKING_ERROR:
            tokenFixDescription = "This should be replaced with '[error]'";
            break;
        case yy::parser::symbol_kind_type::S_PROPERTY_LIST:
            tokenFixDescription = "This should be replaced with 'property'";
            break;
        case yy::parser::symbol_kind_type::S_PROPERTY_ELEMENT:
            tokenFixDescription = "A property element is any one word (following the pattern [a-zA-Z0-9_-]) separated by a comma.";
            break;
        case yy::parser::symbol_kind_type::S_CONSTRAINT_END:
            tokenFixDescription = "Add a ']' at the end.";
            break;
        case yy::parser::symbol_kind_type::S_YYEOF:
            tokenFixDescription = "The last line of the file should be empty.";
            break;
        default:
            tokenFixDescription = "No fix suggestions available.";
    }


    return tokenFixDescription;
}

std::string getHelpMsg(const yy::parser::context& yyctx) {
    std::string helpMsg = "Help: Was this supposed to be\n";

    auto numTokens = 3;
    yy::parser::symbol_kind_type expectedTokens[numTokens];
    auto numTokensRead = yyctx.expected_tokens(expectedTokens, numTokens);

    for (auto i = 0; i < numTokensRead; i++) {
        auto currentToken = expectedTokens[i];

        std::string tokenName = yy::parser::symbol_name(currentToken);
        std::string tokenFixDescription = getTokenFixDescription(currentToken);
        helpMsg += std::format("- {}: {}\n", tokenName, tokenFixDescription);
    }

    return helpMsg;
}

/**
 * Prints a detailed error message when a problem occurs in the parser.
*/
void yy::parser::report_syntax_error(const yy::parser::context& yyctx) const {
    std::string errorSummaryMsg = std::format("Error: Unexpected {} found", symbol_name(yyctx.token()));
    std::cerr << errorSummaryMsg << std::endl;

    std::cerr << " --> " << yyctx.location() << std::endl;

    std::string errorPointingToMsg = getPointingMsg(yyctx.location());
    std::cerr << errorPointingToMsg;

    std::string helpMsg = getHelpMsg(yyctx);
    std::cerr << helpMsg << std::endl;
}

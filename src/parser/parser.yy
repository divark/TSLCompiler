/* Prologue (Imports, Macros, etc) */
%code requires {
    #include <string>
    #include <fmt/format.h>

    #include "tsl_lexer.hpp"
    #include "tsl_collector.hpp"
    #include "error_reporting.hpp"
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
          | if_statement else_statement
          | if_statement
          ;
if_statement: CONSTRAINT_START IF expression CONSTRAINT_END label { $$ = collector.convertPropertiesToIfProperties(); }
            | CONSTRAINT_START IF expression CONSTRAINT_END
            ;
else_statement: ELSE label { $$ = collector.convertPropertiesToElseProperties(); }
              | ELSE       { $$ = collector.markChoiceHasElse(); }
              ;
expression: expression LOGICAL_AND expression { $$ = collector.recordBinaryExpression(ExpType::And); }
          | expression LOGICAL_OR expression  { $$ = collector.recordBinaryExpression(ExpType::Or);  }
          | LOGICAL_NOT expression { $$ = collector.recordUnaryExpression(ExpType::Negated); }
          | LOGICAL_GROUP_START expression LOGICAL_GROUP_END { $$ = collector.recordUnaryExpression(ExpType::Grouped); }
          | PROPERTY_ELEMENT { checkIfCurrentPropertyUndefined(lexer.getCurrentTokenContents(), collector, @$); $$ = collector.recordSimpleExpression(lexer.getCurrentTokenContents()); }
          ;
label:  marking
     |  CONSTRAINT_START property_list CONSTRAINT_END
     ;
property_list:  PROPERTY_LIST property_elements
             ;
property_elements: property_element
                 | property_elements property_element
                 ;
property_element:   PROPERTY_ELEMENT { checkIfCurrentPropertyRedefined(lexer.getCurrentTokenContents(), collector, @$); $$ = collector.recordProperty(lexer.getCurrentTokenContents()); }
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
    throw message;
}

/**
* Returns a human-readable context message for some identified token.
*/
std::string getContextMessage(const yy::parser::symbol_kind_type& identifiedToken) {
    std::string contextMessage = "";

    switch (identifiedToken) {
        case yy::parser::symbol_kind::S_CATEGORY_CONTENTS:
            contextMessage += "Category";
            break;
        case yy::parser::symbol_kind::S_CHOICE_CONTENTS:
            contextMessage += "Choice";
            break;
        case yy::parser::symbol_kind::S_CONSTRAINT_START:
            contextMessage += "Constraint";
            break;
        case yy::parser::symbol_kind::S_CONSTRAINT_END:
            contextMessage += "end of a Constraint";
            break;
        case yy::parser::symbol_kind::S_IF:
            contextMessage += "If statement";
            break;
        case yy::parser::symbol_kind::S_ELSE:
            contextMessage += "Else statement";
            break;
        case yy::parser::symbol_kind::S_LOGICAL_NOT:
            contextMessage += "Logical operator !";
            break;
        case yy::parser::symbol_kind::S_LOGICAL_GROUP_START:
            contextMessage += "Logical grouping character (";
            break;
        case yy::parser::symbol_kind::S_LOGICAL_GROUP_END:
            contextMessage += "Logical grouping character )";
            break;
        case yy::parser::symbol_kind::S_LOGICAL_AND:
            contextMessage += "Logical operator &&";
            break;
        case yy::parser::symbol_kind::S_LOGICAL_OR:
            contextMessage += "Logical operator ||";
            break;
        case yy::parser::symbol_kind::S_MARKING_SINGLE:
            contextMessage += "Single marking";
            break;
        case yy::parser::symbol_kind::S_MARKING_ERROR:
            contextMessage += "Error marking";
            break;
        case yy::parser::symbol_kind::S_PROPERTY_LIST:
            contextMessage += "Property list definition";
            break;
        case yy::parser::symbol_kind::S_PROPERTY_ELEMENT:
            contextMessage += "Property variable";
            break;
        default:
            return yy::parser::symbol_name(identifiedToken);
    }

    return contextMessage;
}

/**
 * Prints a detailed error message when a problem occurs in the parser.
*/
void yy::parser::report_syntax_error(const yy::parser::context& yyctx) const {
    std::string contextMessage = getContextMessage(yyctx.token());
    std::string errorSummaryMsg = fmt::format("Error: A {} is not allowed in its current spot.", contextMessage);
    reportError(errorSummaryMsg, yyctx.location());
}

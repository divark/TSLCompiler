/* Prologue (Imports, Macros, etc) */
%code requires {
    #include <string>

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

%parse-param {TSLLexer &lexer}
%parse-param {TSLCollector &collector}

%token CATEGORY_CONTENTS
%token CHOICE_CONTENTS

%token CONSTRAINT_START

%token IF
%token LOGICAL_NOT

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
choice:     choice_label constraint
      |     choice_label
      ;
constraint: CONSTRAINT_START label CONSTRAINT_END
          | CONSTRAINT_START IF expression CONSTRAINT_END
          ;
expression: LOGICAL_NOT expression { $$ = collector.recordNegatedExpression(); }
          | PROPERTY_ELEMENT { $$ = collector.recordSimpleExpression(lexer.getCurrentTokenContents()); }
          ;
label:  marking
     |  property_list
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

void yy::parser::error(const std::string &message) {
    std::cerr << "Parser Error: " << message << std::endl;
}

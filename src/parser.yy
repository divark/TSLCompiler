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
choice:     choice_label
      ;

category_label:   CATEGORY_CONTENTS { $$ = collector.recordCategory(lexer.getCurrentTokenContents()); } 
        ;
/* $$ as an argument is implied to be the result of an earlier call to collector.recordCategory. */
choice_label:     CHOICE_CONTENTS   { $$ = collector.recordChoice(lexer.getCurrentTokenContents()); }
      ;

%%

/* Bison Epilogue (Function declarations) */

void yy::parser::error(const std::string &message) {
    std::cerr << "Parser Error: " << message << std::endl;
}

/* Prologue (Imports, Macros, etc) */
%code requires {
    #include <string>
    #include "tsl_lexer.hpp"
}

%code {
    #define yylex lexer.constructNextToken
}

/* Bison Declarations (Terminal and Non-Terminal definitions) */
%require "3.2"
%language "c++"

%define api.value.type {std::string}

%parse-param {TSLLexer &lexer}

%token CATEGORY_CONTENTS
%token CHOICE_CONTENTS

/* Bison Grammar Rules */
%%

categories: categories category
          | category

category:   CATEGORY_CONTENTS 

%%

/* Bison Epilogue (Function declarations) */

void yy::parser::error(const std::string &message) {
    std::cerr << "Parser Error: " << message << std::endl;
}

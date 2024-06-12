/* Includes */
%{
    #include "lexer_tokens.hpp"
%}
%option noyywrap

/* Lexer Definitions */
CATEGORY    .*":"

/* Rules */
%%

{CATEGORY}  return Tokens::Category;

%%
/* Extra Code */

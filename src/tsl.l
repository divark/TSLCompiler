/* Includes */
%{
    #include "lexer_tokens.hpp"
%}
%option noyywrap

/* Lexer Definitions */
WHITESPACE  [ \r\n]+
CATEGORY    .+":"
CHOICE      .+"."

/* Rules */
%%

{WHITESPACE} // Go wild with whitespace for formatting, we ignore it by default.
{CATEGORY}  return Tokens::Category;
{CHOICE}    return Tokens::Choice;

%%
/* Extra Code */

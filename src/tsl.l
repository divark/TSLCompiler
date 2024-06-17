/* Includes */
%{
    #include "lexer_tokens.hpp"
%}
%option noyywrap

/* Lexer Definitions */
WHITESPACE  [ \r\n]+
COMMENT     "#".*
CATEGORY    .+":"
CHOICE      .+"."

/* Rules */
%%

{WHITESPACE} /* Go wild with whitespace for formatting, we ignore it by default. */
{COMMENT}   /* Comments are nice for documentation, but we don't look at these either. */
{CATEGORY}  return Tokens::Category;
{CHOICE}    return Tokens::Choice;

%%
/* Extra Code */

/* Includes */
%{
    #include "parser.hpp"
%}
%option noyywrap

/* Lexer Definitions */
WHITESPACE  [ \r\n]+
COMMENT     "#".*
CATEGORY    .+":"
CHOICE      .+"."
MARKING_SINGLE  "[single]"
MARKING_ERROR   "[error]"

/* Rules */
%%

{WHITESPACE} /* Go wild with whitespace for formatting, we ignore it by default. */
{COMMENT}   /* Comments are nice for documentation, but we don't look at these either. */
{CATEGORY}  return yy::parser::token::CATEGORY_CONTENTS;
{CHOICE}    return yy::parser::token::CHOICE_CONTENTS;
{MARKING_SINGLE}    return yy::parser::token::MARKING_SINGLE;
{MARKING_ERROR}     return yy::parser::token::MARKING_ERROR;

%%
/* Extra Code */

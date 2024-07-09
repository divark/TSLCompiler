/* Includes */
%{
    #include "parser.hpp"
%}
%option warn nodefault
%option noyywrap

/* Lexer Definitions */
WHITESPACE  [ \r\n]+
COMMENT     "#".*
CATEGORY    .+":"
CHOICE      .+"."

CONSTRAINT_START "["

PROPERTY_LIST   "property"
PROPERTY_ELEMENT [a-zA-Z0-9]+","?

MARKING_SINGLE  "single"
MARKING_ERROR   "error"

CONSTRAINT_END   "]"

/* Rules */
%%

{WHITESPACE} /* Go wild with whitespace for formatting, we ignore it by default. */
{COMMENT}   /* Comments are nice for documentation, but we don't look at these either. */
{CATEGORY}  return yy::parser::token::CATEGORY_CONTENTS;
{CHOICE}    return yy::parser::token::CHOICE_CONTENTS;
{CONSTRAINT_START}  return yy::parser::token::CONSTRAINT_START;
{MARKING_SINGLE}    return yy::parser::token::MARKING_SINGLE;
{MARKING_ERROR}     return yy::parser::token::MARKING_ERROR;
{PROPERTY_LIST}     return yy::parser::token::PROPERTY_LIST;
{PROPERTY_ELEMENT}  return yy::parser::token::PROPERTY_ELEMENT;
{CONSTRAINT_END}    return yy::parser::token::CONSTRAINT_END;

%%
/* Extra Code */

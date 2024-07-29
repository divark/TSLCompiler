/* Includes */
%{
    #include "parser.hpp"
%}
%option warn nodefault
%option noyywrap

/* Lexer Definitions */
NEWLINES    [\r\n]+
WHITESPACE  [[:space:]]+
COMMENT     "#".*
CATEGORY    .+":"
CHOICE      .+"."

CONSTRAINT_START "["

IF              "if"
LOGICAL_NOT       "!"
LOGICAL_GROUP_START "("
LOGICAL_GROUP_END   ")"
LOGICAL_AND         "&&"
LOGICAL_OR          "||"

MARKING_SINGLE  "single"
MARKING_ERROR   "error"

PROPERTY_LIST   "property"
PROPERTY_ELEMENT [a-zA-Z0-9]+","?

CONSTRAINT_END   "]"

/* Rules */
%%

{NEWLINES}  /* Newlines are nice for formatting, but we don't track them right now. */
{WHITESPACE} /* Go wild with whitespace for formatting, we ignore it by default. */
{COMMENT}   /* Comments are nice for documentation, but we don't look at these either. */
{CATEGORY}  return yy::parser::token::CATEGORY_CONTENTS;
{CHOICE}    return yy::parser::token::CHOICE_CONTENTS;
{CONSTRAINT_START}  return yy::parser::token::CONSTRAINT_START;
{IF}                return yy::parser::token::IF;
{LOGICAL_NOT}       return yy::parser::token::LOGICAL_NOT;
{LOGICAL_GROUP_START}   return yy::parser::token::LOGICAL_GROUP_START;
{LOGICAL_GROUP_END}     return yy::parser::token::LOGICAL_GROUP_END;
{LOGICAL_AND}       return yy::parser::token::LOGICAL_AND;
{LOGICAL_OR}        return yy::parser::token::LOGICAL_OR;
{MARKING_SINGLE}    return yy::parser::token::MARKING_SINGLE;
{MARKING_ERROR}     return yy::parser::token::MARKING_ERROR;
{PROPERTY_LIST}     return yy::parser::token::PROPERTY_LIST;
{PROPERTY_ELEMENT}  return yy::parser::token::PROPERTY_ELEMENT;
{CONSTRAINT_END}    return yy::parser::token::CONSTRAINT_END;

%%
/* Extra Code */

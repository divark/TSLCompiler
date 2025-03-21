/* Includes */
%{
    #include "parser.hpp"
%}
%option warn nodefault
%option noyywrap
%option yyclass="TSLLexer"

/* Lexer Definitions */
NEWLINES    (\r\n)|\n
WHITESPACE  [[:blank:]]
COMMENT     #.*
CATEGORY    [^#\r\n[:blank:]][^#\r\n]*:
CHOICE      [^#\r\n[:blank:]][^#\r\n]*\.

CONSTRAINT_START "["

IF              "if"
ELSE            "[else]"
LOGICAL_NOT       "!"
LOGICAL_GROUP_START "("
LOGICAL_GROUP_END   ")"
LOGICAL_AND         "&&"
LOGICAL_OR          "||"

MARKING_SINGLE  "[single]"
MARKING_ERROR   "[error]"

PROPERTY_LIST   "property"
PROPERTY_ELEMENT [a-zA-Z0-9_\-]+","?

CONSTRAINT_END   "]"

/* Rules */
%%

{NEWLINES}  { lineNumber++; lineColumn = 0; }
{WHITESPACE} { lineColumn += 1; }
{COMMENT}   /* Comments are nice for documentation, but we don't look at these either. */
{CATEGORY}  return yy::parser::token::CATEGORY_CONTENTS;
{CHOICE}    return yy::parser::token::CHOICE_CONTENTS;
{CONSTRAINT_START}  return yy::parser::token::CONSTRAINT_START;
{IF}                return yy::parser::token::IF;
{ELSE}              return yy::parser::token::ELSE;
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
.                   return yy::parser::token::YYUNDEF;

%%
/* Extra Code */

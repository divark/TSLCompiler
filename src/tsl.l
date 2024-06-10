%option noyywrap

/* Lexer Definitions */
COMMENTS    "#".*

WHITESPACE  [ \r\t]+
CATEGORY    [a-zA-Z0-9]([a-zA-Z0-9]+|{WHITESPACE})":"
CHOICE      [a-zA-Z0-9]([a-zA-Z0-9]+|{WHITESPACE})"."

/* Rules */
%%
{COMMENTS}

{CATEGORY}  std::string(yytext, yyleng);
{CHOICE}    printf("Choice: %s", yytext);

%%
/* Extra Code */

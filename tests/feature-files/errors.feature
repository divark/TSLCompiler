Feature: An error message should be provided when an issue comes up with the Lexer or Parser.
    Scenario: The Lexer reports an unrecognized symbol.
        Given a TSL input with a Category that ends with a '?' instead of a ':',
        When error messages are redirected to be read by us,
        And the Compiler consumes the input,
        Then the error message should be "Parser failed at invalid_symbol.txt:1:4: Category does not end with a ':'."

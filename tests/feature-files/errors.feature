Feature: An error message should be provided when an issue comes up with the Lexer or Parser.
    Scenario: The Parser reports an unrecognized symbol.
        Given a TSL input with a Category that ends with a '?' instead of a ':',
        When error messages are redirected to be read by us,
        And the Parser consumes the input,
        Then the error message should point to line 1.
        And the error message should mention expecting a Category.

    Scenario: The Parser reports an invalid Choice.
        Given a TSL input with a Category, and a Choice that ends with a '?' instead of a '.',
        When the error messages are redirected to be read by us,
        And the Parser consumes the input,
        Then the error message should point to line 2.
        And the error message should mention expecting a Choice.

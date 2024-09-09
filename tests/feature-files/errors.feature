Feature: An error message should be provided when an issue comes up with the Lexer or Parser.
    Scenario: The Parser reports an invalid Category.
        Given a TSL input with a Category that ends with a '?' instead of a ':',
        When error messages are redirected to be read by us,
        And the Parser consumes the input,
        Then the error message should point to line 1.
        And the error message should point to line column 4.
        And the error message should mention expecting a Category.

    Scenario: The Parser reports an invalid Choice.
        Given a TSL input with a Category, and a Choice that ends with a '?' instead of a '.',
        When the error messages are redirected to be read by us,
        And the Parser consumes the input,
        Then the error message should point to line 2.
        And the error message should point to line column 8.
        And the error message should mention expecting a Choice.

    Scenario: The Parser reports an error for the common case of a property followed by an if statement.
        Given a TSL input with one Category and Choice, and another Category with a Choice defined with a property followed by an if statement,
        When the error messages are redirected to be read by us,
        And the Parser consumes the input,
        Then the error message should point to line 5.
        And the error message should point to line column 43.
        And the error message should mention expecting another Choice.

    Scenario: The Parser reports an error for the common case of a [single][error].
        Given a TSL input with one Category, one Choice, and another Choice with a [single][error],
        When the error messages are redirected to be read by us,
        And the Parser consumes the input,
        Then the error message should point to line 3.
        And the error message should point to line column 39.
        And the error message should mention expecting another Choice.

Feature: An error message should be provided when an issue comes up with the Lexer or Parser.
    Scenario: The Parser reports an invalid Category.
        Given a TSL input file called invalid_category.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding an unexpected property element.
        Then line 2 of the error message should mention the given TSL input file at line 1, column 3.
        Then line 3 of the error message should point to the "Huh" in line 1 with line 2 below it.
        Then line 5 of the error message should display a help message with three bullet points.

    Scenario: The Parser reports an invalid Choice.
        Given a TSL input file called invalid_choice.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then the error message should point to line 2.
        Then the error message should point to line column 7.
        Then the error message should mention expecting a Choice.

    Scenario: The Parser reports an error for the common case of a property followed by an if statement.
        Given a TSL input file called invalid_property_then_if.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then the error message should point to line 5.
        Then the error message should point to line column 43.
        Then the error message should mention expecting a Choice.

    Scenario: The Parser reports an error for the common case of a [single][error].
        Given a TSL input file called invalid_double_marking.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then the error message should point to line 3.
        Then the error message should point to line column 39.
        Then the error message should mention expecting a Choice.

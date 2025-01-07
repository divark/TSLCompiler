Feature: An error message should be provided when an issue comes up with the Lexer or Parser.
    Scenario: The Parser reports an invalid Category.
        Given a TSL input file called invalid_category.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding PROPERTY_ELEMENT in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 1, column 3.
        Then line 3 of the error message should point to the token Huh? in line 1 with line 2 below it.

    Scenario: The Parser reports an invalid Choice.
        Given a TSL input file called invalid_choice.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding PROPERTY_ELEMENT in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 2, column 7.
        Then line 3 of the error message should point to the token Huh? in line 2 with line 3 below it.

    Scenario: The Parser reports an error for the common case of a property followed by an if statement.
        Given a TSL input file called invalid_property_then_if.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding CONSTRAINT_START in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 5, column 43.
        Then line 3 of the error message should point to the token if in line 5 with line 6 below it.

    Scenario: The Parser reports an error for the common case of a [single][error].
        Given a TSL input file called invalid_double_marking.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding MARKING_ERROR in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 3, column 39.
        Then line 3 of the error message should point to the token [error] in line 3 with line 4 below it.

    Scenario: The Parser reports an undefined property error.
        Given a TSL input file called undefined_property.txt,
        When the error messages are redirected to be read by us,
        When the Parser consumes the input,
        Then line 1 of the error message should mention 'Property test2 not defined in prior Categories.'.
        Then line 2 of the error message should mention the given TSL input file at line 3, column 33.
        Then line 3 of the error message should point to the token test2 in line 3 with line 4 below it.

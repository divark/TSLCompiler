Feature: An error message should be provided when an issue comes up with the Lexer or Parser.
    Scenario: The Parser reports an invalid Category.
        Given a TSL input file called invalid_category.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding 'Property variable' in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 1, column 3.
        Then line 3 of the error message should point to the token Huh? in line 1 with line 2 below it.

    Scenario: The Parser reports an invalid Choice.
        Given a TSL input file called invalid_choice.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding 'Property variable' in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 2, column 7.
        Then line 3 of the error message should point to the token Huh? in line 2 with line 3 below it.

    Scenario: The Parser reports an error for the common case of a property followed by an if statement.
        Given a TSL input file called invalid_property_then_if.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding 'Constraint' in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 5, column 43.
        Then line 3 of the error message should point to the token if in line 5 with line 6 below it.

    Scenario: The Parser reports an error for the common case of a [single][error].
        Given a TSL input file called invalid_double_marking.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention finding 'Error marking' in the wrong spot.
        Then line 2 of the error message should mention the given TSL input file at line 3, column 39.
        Then line 3 of the error message should point to the token [error] in line 3 with line 4 below it.

    Scenario: The Parser reports an undefined property error when a property used was defined in the same Category.
        Given a TSL input file called defined_property_used_in_same_category.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention 'Property test1 not defined in any prior Categories.'.
        Then line 2 of the error message should mention the given TSL input file at line 3, column 33.
        Then line 3 of the error message should point to the token test1 in line 3 with line 4 below it.

    Scenario: The Parser reports an undefined property error if the property was never defined.
        Given a TSL input file called undefined_property.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention 'Property test2 not defined in any prior Categories.'.
        Then line 2 of the error message should mention the given TSL input file at line 3, column 33.
        Then line 3 of the error message should point to the token test2 in line 3 with line 4 below it.

    Scenario: The Parser reports a duplicated property error if the property was defined twice.
        Given a TSL input file called duplicate_property.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention 'Property test2 was already defined elsewhere.'.
        Then line 2 of the error message should mention the given TSL input file at line 3, column 39.
        Then line 3 of the error message should point to the token test2 in line 3 with line 4 below it.

    Scenario: The Parser reports a duplicated Category error when two Categories are named the same.
        Given a TSL input file called duplicate_categories.txt,
        When the Parser consumes the input,
        Then line 1 of the error message should mention 'Category Sample Category 1: was already defined elsewhere.'.
        Then line 2 of the error message should mention the given TSL input file at line 4, column 18.
        Then line 3 of the error message should point to the token 1: in line 4 with line 5 below it.

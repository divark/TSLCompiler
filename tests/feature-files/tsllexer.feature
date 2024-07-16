Feature: The Lexer recognizes valid TSL input.
    Scenario: An If Statement should be recognized from the Lexer from a valid TSL file.
        Given a TSL input with two Categories, one with a Choice with a Property, and another with a Choice with an If Statement,
        When the Lexer consumes the input,
        Then the Lexer should detect the Choice's If Statement.

    Scenario: Multiple Properties from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL input with one Category, Choice, and multiple Properties,
        When the Lexer consumes the input,
        Then the Lexer should return each Property's contents as a string.

    Scenario: A Property from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL input with one Category, Choice, and a single Property,
        When the Lexer consumes the input,
        Then the Lexer should return the Property's contents as a string.

    Scenario: An error marking from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL input with a Category, Choice, and one error marking constraint,
        When the Lexer consumes the input,
        Then the Lexer should detect the Choice's error Marking.

    Scenario: A single marking from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL input with a Category, Choice, and one single marking constraint,
        When the Lexer consumes the input,
        Then the Lexer should detect the Choice's single Marking.

    Scenario: A Choice from a Category should be returned from the Lexer from a valid TSL file.
        Given a TSL input with one Category, and one Choice,
        When the Lexer consumes the input,
        Then the Lexer should return the Category's Choice contents as a string.

    Scenario: A Comment should not be returned from the Lexer from a valid TSL file.
        Given a TSL input with one Category, and one Choice,
        When the Lexer consumes the input,
        Then the Lexer should not return the Category Comment contents as a string.

    Scenario: A Category should be returned from the Lexer from a valid TSL file.
        Given a TSL input with one Category, and one Choice,
        When the Lexer consumes the input,
        Then the Lexer should return the Category contents as a string.


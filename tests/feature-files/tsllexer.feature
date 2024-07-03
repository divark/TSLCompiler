Feature: The Lexer recognizes valid TSL input.
    Scenario: A single marking from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL input with a Category, Choice, and one marking constraint,
        When the Lexer consumes the input,
        Then the Lexer should detect the Choice's Marking.

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

Feature: The TSLCompiler should emit an error for whatever the TSLgenerator ignores.
    Scenario: A user provides a file containing only Categories.
        Given an input with only Categories,
        When the input is compiled by the TSLCompiler,
        Then the TSLCompiler should return an error.

Feature: The TSLCompiler should generate test frames identical to the TSLgenerator.
    Scenario: A user provides a file containing a Category and a Choice.
        Given an input with one Category, and one Choice,
        When the input is compiled by the TSLCompiler,
        And the input is compiled by the TSLgenerator,
        Then the TSLCompiler's test frames should match the TSLgenerator's test frames.

    Scenario: A Category should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with one Category, and one Choice,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Parse Tree should contain one Category named "Sample Category 1:".

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

Feature: The TSLCompiler should honor the input flags from the TSLgenerator.
    Scenario: A user wants the results of the TSLCompiler to be in a file using the -o flag.
        Given an input file with one Category, and one Choice,
        And a desired output name,
        When the TSLCompiler is ran with the input file and -o with a desired output name as parameters,
        And the TSLgenerator is ran with the input file and -o with a desired output name as parameters,
        Then the TSLCompiler output file should match the TSLgenerator output file.

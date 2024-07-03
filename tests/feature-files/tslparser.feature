Feature: The Parser handles valid TSL input.
    Scenario: A single marking for a Choice should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with a Category, Choice, and one marking constraint,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's marking.

    Scenario: Both Categories and Choices should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with two Categories, and one Choice each,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Parse Tree should contain the contents of both Categories,
        And the Parse Tree should contain the contents of both Choices.

    Scenario: A Choice should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with one Category, and one Choice,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Parse Tree should contain the contents of the Choice.

    Scenario: A Category should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with one Category, and one Choice,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Parse Tree should contain the contents of the Category.

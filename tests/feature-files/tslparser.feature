Feature: The Parser handles valid TSL input.
    Scenario: An AND Expression from an If Statement should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with three Categories with one Choice each, where the first two Choices contain a property, and the last an If Statement with an AND Expression,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's AND Expression.

    Scenario: A Grouped Expression from an If Statement should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with two Categories, one with a Choice with a Property, and another with a Choice with an If Statement containing a grouped expression,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's Grouped Expression.

    Scenario: An Unary Expression from an If Statement should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with two Categories, one with a Choice with a Property, and another with a Choice with an If Statement containing an unary expression,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's Negated Expression.

    Scenario: A Simple Expression from an If Statement should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with two Categories, one with a Choice with a Property, and another with a Choice with an If Statement,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's Simple Expression.

    Scenario: One Property for a Choice should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with one Category, Choice, and a single Property,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's Property.

    Scenario: Multiple Properties for a Choice should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with one Category, Choice, and multiple Properties,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's Properties.

    Scenario: A single and error marking for one Choice each should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with a Category, one Choice with a single marking, and one Choice with an error marking,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the first Choice's single marking.
        And the Collector should contain the second Choice's error marking.

    Scenario: An error marking for a Choice should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with a Category, Choice, and one error marking constraint,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's error marking.

    Scenario: A single marking for a Choice should be found in the Parse Tree from a valid TSL file.
        Given a TSL input with a Category, Choice, and one single marking constraint,
        When the Parser converts the TSL input into a Parse Tree,
        Then the Collector should contain the Choice's single marking.

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

Feature: The TSLCompiler should generate test frames.
    Scenario: A user provides a file containing a Category and a Choice.
        Given a TSL file containing one Category and one Choice,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain one test frame mentioning the Category and Choice.

    Scenario: A single Node should be made from a file containing one Category, and one Choice.
        Given a TSL file containing one Category, and one Choice,
        When the input is compiled by the TSLCompiler,
        And the nodes are created from the TSLCollector,
        Then the number of Nodes should match the number of Choices,
        And Node 1 should contain the first Category,
        And Node 1 should contain the first Choice.

    Scenario: Edges should be made from a file containing two Categories, and two Choices.
        Given a TSL file containing two Categories, and two Choices each,
        When the input is compiled by the TSLCompiler,
        And the nodes are created from the TSLCollector,
        And the edges are created from the nodes,
        Then the number of Nodes should match the number of Choices,
        And Node 1 should have an edge to Nodes 3 and 4.
        And Node 2 should have an edge to Nodes 3 and 4.

Feature: The TSLCompiler should generate test frames identical to the TSLgenerator.
    Scenario: A user provides a file containing a Category and a Choice.
        Given an input with one Category, and one Choice,
        When the input is compiled by the TSLCompiler,
        And the input is compiled by the TSLgenerator,
        Then the TSLCompiler's test frames should match the TSLgenerator's test frames.

Feature: The TSLCompiler should honor the input flags from the TSLgenerator.
    Scenario: A user wants the results of the TSLCompiler to be in a file using the -o flag.
        Given an input file with one Category, and one Choice,
        And a desired output name,
        When the TSLCompiler is ran with the input file and -o with a desired output name as parameters,
        And the TSLgenerator is ran with the input file and -o with a desired output name as parameters,
        Then the TSLCompiler output file should match the TSLgenerator output file.

Feature: The TSLCompiler should generate test frames.
    Scenario: A user provides a file containing a Category and a Choice.
        Given a TSL file called one_category_one_choice.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 1 test cases.
        Then test case 1 should have 'Simple Category 1' as category 1.
        Then test case 1 should have 'Simple Choice 1' chosen in category 1.

    Scenario: A single Node should be made from a file containing one Category, and one Choice.
        Given a TSL file called one_category_one_choice.txt,
        When the input is compiled by the TSLCompiler,
        When the nodes are created from the TSLCollector,
        Then the number of Nodes should match the number of Choices,
        Then Node 1 should contain the first Category,
        Then Node 1 should contain the first Choice.

    Scenario: Edges should be made from a file containing two Categories, and two Choices.
        Given a TSL file called two_categories_two_choices_each.txt,
        When the input is compiled by the TSLCompiler,
        When the nodes are created from the TSLCollector,
        When the edges are created from the nodes,
        Then the number of Nodes should match the number of Choices,
        Then Node 1 should have an edge to Nodes 3 and 4.
        Then Node 2 should have an edge to Nodes 3 and 4.

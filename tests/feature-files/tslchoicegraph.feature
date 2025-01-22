Feature: A Directed Graph should be derived from the TSLCompiler.
    Scenario: A single Node should be made from a file containing one Category, and one Choice.
        Given a TSL file called one_category_one_choice.txt,
        When the input is consumed by the TSLCompiler,
        When the nodes and edges are created from the TSLCollector,
        Then the number of Nodes should match the number of Choices.
        Then Node 1 should contain 'Simple Category 1:' as the Category.
        Then Node 1 should contain 'Simple Choice 1.' as the Choice.

    Scenario: Edges should be made from a file containing two Categories, and two Choices.
        Given a TSL file called two_categories_two_choices_each.txt,
        When the input is consumed by the TSLCompiler,
        When the nodes and edges are created from the TSLCollector,
        Then the number of Nodes should match the number of Choices.
        Then Node 1 should have an edge to Node 3.
        Then Node 1 should have an edge to Node 4.
        Then Node 2 should have an edge to Node 3.
        Then Node 2 should have an edge to Node 4.

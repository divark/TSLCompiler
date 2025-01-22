Feature: The TSLCompiler should generate test frames.
    Scenario: A user provides a file containing a Category and a Choice.
        Given a TSL file called one_category_one_choice.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 1 test cases.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 1.' chosen in category 1.

Feature: The TSLCompiler should generate test frames.
    Scenario: A user provides a file containing a Category and a Choice.
        Given a TSL file called one_category_one_choice.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 1 test cases.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 1.' chosen in category 1.

    Scenario: A user provides a file containing two Categories with one Choice each.
        Given a TSL file called two_categories_two_choices_each.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 4 test cases.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 1 should have 'Simple Category 2:' as category 2.
        Then test case 1 should have 'Simple Choice 3.' chosen in category 2.
        Then test case 2 should have 'Simple Category 1:' as category 1.
        Then test case 2 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 2 should have 'Simple Category 2:' as category 2.
        Then test case 2 should have 'Simple Choice 4.' chosen in category 2.
        Then test case 3 should have 'Simple Category 1:' as category 1.
        Then test case 3 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 3 should have 'Simple Category 2:' as category 2.
        Then test case 3 should have 'Simple Choice 3.' chosen in category 2.
        Then test case 4 should have 'Simple Category 1:' as category 1.
        Then test case 4 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 4 should have 'Simple Category 2:' as category 2.
        Then test case 4 should have 'Simple Choice 4.' chosen in category 2.

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

    Scenario: Files with markers in it should show up first as test cases.
        Given a TSL file called choices_with_multiple_markers.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 3 test cases.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 1 should be flagged as having a marker.
        Then test case 2 should have 'Simple Category 2:' as category 1.
        Then test case 2 should have 'Simple Choice 3.' chosen in category 1.
        Then test case 2 should be flagged as having a marker.

    Scenario: Properties influence if statements in a test case.
        Given a TSL file called choices_with_simple_if.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 1 test cases.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 1 should have 'Simple Category 2:' as category 2.
        Then test case 1 should have 'Simple Choice 2.' chosen in category 2.

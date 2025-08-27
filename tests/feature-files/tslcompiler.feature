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

    Scenario: If statements with single markers are not similar to normal test cases.
        Given a TSL file called choice_with_conditional_single_marker.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 2 test cases.
        Then test case 1 should have 1 categories.
        Then test case 1 should have 'Simple Category 2:' as category 1.
        Then test case 1 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 2 should have 3 categories.
        Then test case 2 should have 'Simple Category 1:' as category 1.
        Then test case 2 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 2 should have 'Simple Category 2:' as category 2.
        Then test case 2 should have 'Simple Choice 3.' chosen in category 2.
        Then test case 2 should have 'Simple Category 3:' as category 3.
        Then test case 2 should have 'Simple Choice 4.' chosen in category 3.

    Scenario: If statements with error markers are not similar to normal test cases.
        Given a TSL file called choice_with_conditional_error_marker.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 2 test cases.
        Then test case 1 should have 1 categories.
        Then test case 1 should have 'Simple Category 2:' as category 1.
        Then test case 1 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 2 should have 3 categories.
        Then test case 2 should have 'Simple Category 1:' as category 1.
        Then test case 2 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 2 should have 'Simple Category 2:' as category 2.
        Then test case 2 should have 'Simple Choice 3.' chosen in category 2.
        Then test case 2 should have 'Simple Category 3:' as category 3.
        Then test case 2 should have 'Simple Choice 4.' chosen in category 3.

    Scenario: A Choice mentions how it was chosen from an if statement.
        Given a TSL file called choice_with_conditional_single_marker.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 2 test cases.
        Then test case 1 should have 1 categories.
        Then test case 1 should have 'Simple Category 2:' as category 1.
        Then test case 1 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 1 should have 'follows [if simple]' as a dependency for the chosen choice in category 1.

    Scenario: A Choice mentions how it was chosen from an else statement.
        Given a TSL file called choice_chosen_from_else_statement.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 1 test cases.
        Then test case 1 should have 2 categories.
        Then test case 1 should have 'follows [else] from [if !simple]' as a dependency for the chosen choice in category 2.

    Scenario: Not Applicable shows up if a Category has no other way of progressing.
        Given a TSL file called category_with_na.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 2 test cases.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 1 should have 'Simple Category 2:' as category 2.
        Then test case 1 should have 'N/A' chosen in category 2.
        Then test case 2 should have 'Simple Category 1:' as category 1.
        Then test case 2 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 2 should have 'Simple Category 2:' as category 2.
        Then test case 2 should have 'Simple Choice 3.' chosen in category 2.

    Scenario: Not Applicable shows up if multiple Categories have no other way of progressing.
        Given a TSL file called categories_with_na.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 2 test cases.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 1 should have 'Simple Category 2:' as category 2.
        Then test case 1 should have 'N/A' chosen in category 2.
        Then test case 1 should have 'Simple Category 3:' as category 3.
        Then test case 1 should have 'N/A' chosen in category 3.
        Then test case 2 should have 'Simple Category 1:' as category 1.
        Then test case 2 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 2 should have 'Simple Category 2:' as category 2.
        Then test case 2 should have 'Simple Choice 3.' chosen in category 2.
        Then test case 2 should have 'Simple Category 3:' as category 3.
        Then test case 2 should have 'Simple Choice 4.' chosen in category 3.

    Scenario: A Choice dependency should only be present for a Choice with a conditional.
        Given a TSL file called chained_choices_with_if.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 1 test cases.
        Then test case 1 should not have a choice dependency for category 1.
        Then test case 1 should not have a choice dependency for category 2.
        Then test case 1 should have 'follows [if abc]' as a dependency for the chosen choice in category 3.

    Scenario: All marker test cases should show up first.
        Given a TSL file called markers_found_in_order.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 3 test cases.
        Then test case 1 should have 1 categories.
        Then test case 1 should have 'Simple Category 1:' as category 1.
        Then test case 1 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 2 should have 1 categories.
        Then test case 2 should have 'Simple Category 2:' as category 1.
        Then test case 2 should have 'Simple Choice 4.' chosen in category 1.
        Then test case 3 should have 2 categories.
        Then test case 3 should have 'Simple Category 1:' as category 1.
        Then test case 3 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 3 should have 'Simple Category 2:' as category 2.
        Then test case 3 should have 'Simple Choice 3.' chosen in category 2.

    Scenario: A marker should not be visited more than once.
        Given a TSL file called markers_not_visited_twice.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 3 test cases.
        Then test case 1 should have 1 categories.
        Then test case 1 should have 'Simple Category 2:' as category 1.
        Then test case 1 should have 'Simple Choice 3.' chosen in category 1.
        Then test case 2 should have 2 categories.
        Then test case 2 should have 'Simple Category 1:' as category 1.
        Then test case 2 should have 'Simple Choice 1.' chosen in category 1.
        Then test case 2 should have 'Simple Category 2:' as category 2.
        Then test case 2 should have 'Simple Choice 4.' chosen in category 2.
        Then test case 3 should have 2 categories.
        Then test case 3 should have 'Simple Category 1:' as category 1.
        Then test case 3 should have 'Simple Choice 2.' chosen in category 1.
        Then test case 3 should have 'Simple Category 2:' as category 2.
        Then test case 3 should have 'Simple Choice 4.' chosen in category 2.

    Scenario: Duplicate test cases made from N/As should not be displayed.
        Given a TSL file called duplicate_cases_from_nas.txt,
        When the input is consumed by the TSLCompiler,
        Then the TSLCompiler's result should contain 1 test cases.
        Then test case 1 should have 'Category 1:' as category 1.
        Then test case 1 should have 'Choice 1.' chosen in category 1.
        Then test case 1 should have 'Category 2:' as category 2.
        Then test case 1 should have 'Choice 4.' chosen in category 2.

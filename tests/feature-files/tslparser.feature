Feature: The Parser handles valid TSL input.
    Scenario: A Category should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Parser collects the variables from the TSL input,
        Then Category 1 should be labeled as 'Simple Category 1:'.

    Scenario: A Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 1 should be labeled as 'Simple Choice 1.'.

    Scenario: Both Categories and Choices should be found in the Collector from a valid TSL file.
        Given a TSL file called two_categories_one_choice_each.txt,
        When the Parser collects the variables from the TSL input,
        Then Category 1 should be labeled as 'Simple Category 1:'.
        Then Choice 1 from Category 1 should be labeled as 'Simple Choice 1.'.
        Then Category 2 should be labeled as 'Simple Category 2:'.
        Then Choice 1 from Category 2 should be labeled as 'Simple Choice 2.'.

    Scenario: A single marking for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_choice_with_marking.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 1 should contain a single marking.

    Scenario: An error marking for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_choice_with_error_marking.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 1 should contain a error marking.

    Scenario: A single and error marking for one Choice each should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_choices_with_single_and_error_markings.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 1 should contain a single marking.
        Then Choice 2 from Category 1 should contain a error marking.

    Scenario: One Property for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_one_property.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 1 should contain 'ABC' as Property 1.

    Scenario: Multiple Properties for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_multiple_properties.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 1 should contain 'A' as Property 1.
        Then Choice 1 from Category 1 should contain 'B' as Property 2.
        Then Choice 1 from Category 1 should contain 'C' as Property 3.

    Scenario: A Simple Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choices_with_simple_if.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 2 should contain the 'simple' Expression.

    Scenario: An Unary Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_unary_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 2 should contain the '!simple' Expression.

    Scenario: A Grouped Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_grouped_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 2 should contain the '(simple)' Expression.

    Scenario: An AND Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_and_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 3 should contain the 'choice1 && choice2' Expression.

    Scenario: An OR Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_or_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then Choice 1 from Category 3 should contain the 'choice1 || choice2' Expression.

    # PENDING: The following Scenarios have to be implemented below:
    Scenario: Properties from an If Expression are found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_property_in_if.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then 'simple2' should be in the if properties.

    Scenario: Properties from an Else Expression are found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_property_in_else.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then 'simple2' should be in the else properties.

    Scenario: A Single Marking from an If Expression is found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_single_in_if.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then a single marker should be in the if properties.

    Scenario: An Error Marking from an If Expression is found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_error_in_if.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then a error marker should be in the if properties.

    Scenario: A Single Marking from an Else Expression is found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_single_in_else.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then a single marker should be in the else properties.

    Scenario: An Error Marking from an Else Expression is found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_error_in_else.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then a error marker should be in the else properties.

    Scenario: An Else Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_else_alone.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then there should be no else properties.

    Scenario: An Else Statement following an If Statement with a Label should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_if_label_else.txt,
        When the Parser collects the variables from the TSL input,
        When Choice 1 from Category 2 gets evaluated properties from found properties 'simple1',
        Then a error marker should be in the if properties.
        Then there should be no else properties.

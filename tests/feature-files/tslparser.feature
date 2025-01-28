Feature: The Parser handles valid TSL input.
    Scenario: A Category should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain Category 1 as 'Simple Category 1:'.

    Scenario: A Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain Choice 1 as 'Simple Choice 1.'.

    Scenario: Both Categories and Choices should be found in the Collector from a valid TSL file.
        Given a TSL file called two_categories_one_choice_each.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain Category 1 as 'Simple Category 1:'.
        Then the Collector should contain Choice 1 as 'Simple Choice 1.'.
        Then the Collector should contain Category 2 as 'Simple Category 2:'.
        Then the Collector should contain Choice 2 as 'Simple Choice 2.'.

    Scenario: A single marking for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_choice_with_marking.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a single marking for Choice 1

    Scenario: An error marking for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_choice_with_error_marking.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a error marking for Choice 1

    Scenario: A single and error marking for one Choice each should be found in the Collector from a valid TSL file.
        Given a TSL file called one_category_choices_with_single_and_error_markings.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a single marking for Choice 1
        Then the Collector should contain a error marking for Choice 2

    Scenario: One Property for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_one_property.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain Property 1 in Choice 1 as 'ABC'.

    Scenario: Multiple Properties for a Choice should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_multiple_properties.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain Property 1 in Choice 1 as 'A'.
        Then the Collector should contain Property 2 in Choice 1 as 'B'.
        Then the Collector should contain Property 3 in Choice 1 as 'C'.

    Scenario: A Simple Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choices_with_simple_if.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a Simple Expression in Choice 2 as 'simple'.

    Scenario: An Unary Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_unary_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a Negated Expression in Choice 2 as '!simple'.

    Scenario: A Grouped Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_grouped_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a Grouped Expression in Choice 2 as '(simple)'.

    Scenario: An AND Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_and_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a AND Expression in Choice 3 as 'choice1 && choice2'.

    Scenario: An OR Expression from an If Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_or_expression.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain a OR Expression in Choice 3 as 'choice1 || choice2'.

    Scenario: Properties from an If Expression are found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_property_in_if.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain Property 2 in an If statement in Choice 2 as 'simple2'.

    Scenario: Properties from an Else Expression are found in the Collector from a valid TSL file.
        Given a TSL file called choice_with_property_in_else.txt,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain Property 2 in an Else statement in Choice 2 as 'simple2'.

### TODO: Translate these

    Scenario: An Error Marking from an Else Expression is found in the Collector from a valid TSL file.
        Given a TSL file called two Categories, one with a Choice with a Property, and another with a Choice defining an Error Marking in the Else Statement,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain the Choice's Error Else Marking.

    Scenario: A Single Marking from an Else Expression is found in the Collector from a valid TSL file.
        Given a TSL file called two Categories, one with a Choice with a Property, and another with a Choice defining a Single Marking in the Else Statement,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain the Choice's Single Else Marking.


    Scenario: An Else Statement following an If Statement with a Label should be found in the Collector from a valid TSL file.
        Given a TSL file called two Categories, one with a Choice with a Property, and another with a Choice containing an If Label Else,
        When the Parser collects the variables from the TSL input,
        Then the Collector should have the second Choice flagged as having an Else Statement.
        And the Collector should have the second Choice flagged as having an Error marking defined from the If Statement.

    Scenario: An Else Statement should be found in the Collector from a valid TSL file.
        Given a TSL file called two Categories, one with a Choice with a Property, and another with a Choice with an Else Statement,
        When the Parser collects the variables from the TSL input,
        Then the Collector should have the Choice flagged as having an Else Statement.

    Scenario: An Error Marking from an If Expression is found in the Collector from a valid TSL file.
        Given a TSL file called two Categories, one with a Choice with a Property, and another with a Choice defining an Error Marking in the If Statement,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain the Choice's Error If Marking.

    Scenario: A Single Marking from an If Expression is found in the Collector from a valid TSL file.
        Given a TSL file called two Categories, one with a Choice with a Property, and another with a Choice defining a Single Marking in the If Statement,
        When the Parser collects the variables from the TSL input,
        Then the Collector should contain the Choice's Single If Marking.

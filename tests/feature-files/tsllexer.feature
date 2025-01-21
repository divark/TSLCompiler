Feature: The Lexer recognizes valid TSL input.
    Scenario: An Else Statement should be recongized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_else_alone.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved the Choice's Else Statement.

    Scenario: An Or Expression should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_or_expression.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved the Choice Expression's OR Operator.

    Scenario: An And Expression should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_and_expression.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved the Choice Expression's AND Operator.

    Scenario: A Grouped Expression should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_grouped_expression.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved the Grouped Expression's beginning and end.

    Scenario: A Negated Logical Operator should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_unary_expression.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved the Choice Expression's Negated Operator.

    Scenario: An If Statement should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choices_with_simple_if.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved the Choice's If Statement.

    Scenario: Multiple Properties from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_multiple_properties.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved each Property's contents as a string.

    Scenario: A Property from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_one_property.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved 'ABC'.

    Scenario: An error marking from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_choice_with_error_marking.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved '[error]'.

    Scenario: A single marking from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_choice_with_marking.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved '[single]'.

    Scenario: A Choice from a Category should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved 'Simple Choice 1.'.

    Scenario: A Comment should not be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        Then the Lexer should not have retrieved '# Comments are quite helpful as reminders.'.

    Scenario: The Line Number should be tracking the whereabouts of a single Category.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to first Category,
        Then the Lexer should be on Line Number 2.

    Scenario: The Line Number should be tracking the whereabouts of a single Choice.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to first Choice,
        Then the Lexer should be on Line Number 3.

    Scenario: A Category should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved 'Simple Category 1:'.

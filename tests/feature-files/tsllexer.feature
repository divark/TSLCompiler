Feature: The Lexer recognizes valid TSL input.
    Scenario: A Category should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        Then the Lexer should have retrieved 'Simple Category 1:'.

    Scenario: The Line Number should be tracking the whereabouts of a single Choice.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Choice' 1,
        Then the Lexer should be on Line Number 3.

    Scenario: The Line Number should be tracking the whereabouts of a single Category.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Category' 1,
        Then the Lexer should be on Line Number 2.

    Scenario: A Comment should not be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        Then the Lexer should not have retrieved '# Comments are quite helpful as reminders.'.

    Scenario: A Choice from a Category should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_one_choice.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Category' 1,
        Then the Lexer should have retrieved 'Simple Choice 1.'.

    Scenario: A single marking from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_choice_with_marking.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Choice' 1,
        Then the Lexer should have retrieved '[single]'.

    Scenario: An error marking from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called one_category_choice_with_error_marking.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Choice' 1,
        Then the Lexer should have retrieved '[error]'.

    Scenario: A Property from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_one_property.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Property List' 1,
        Then the Lexer should have retrieved 'ABC'.

    Scenario: Multiple Properties from a Choice should be returned from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_multiple_properties.txt,
        When the Lexer consumes the input,
        When the Lexer reads all properties from 'Property List' 1,
        Then the Properties should be 'A, B, C'.

    Scenario: An If Statement should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choices_with_simple_if.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Constraint' 2,
        Then the Lexer should have retrieved 'if'.

    Scenario: A Negated Logical Operator should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_unary_expression.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'If' 1,
        Then the Lexer should have retrieved '!'.

    Scenario: The beginning of a Grouped Expression should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_grouped_expression.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'If' 1,
        Then the Lexer should have retrieved '('.

    Scenario: The end of a Grouped Expression should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_grouped_expression.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Property Element' 2,
        Then the Lexer should have retrieved ')'.

    Scenario: An And Expression should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_and_expression.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Property Element' 3,
        Then the Lexer should have retrieved '&&'.

    Scenario: An Or Expression should be recognized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_or_expression.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Property Element' 3,
        Then the Lexer should have retrieved '||'.

    Scenario: An Else Statement should be recongized from the Lexer from a valid TSL file.
        Given a TSL file called choice_with_else_alone.txt,
        When the Lexer consumes the input,
        When the Lexer reads up to 'Constraint End' 2,
        Then the Lexer should have retrieved the Choice's Else Statement.

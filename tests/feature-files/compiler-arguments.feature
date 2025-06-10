Feature: All TSLgenerator arguments are honored.
    Scenario: The '-c' argument is specified.
        Given the arguments '-c test-files/one_category_one_choice.txt',
        When we parse the arguments,
        Then the 'count frames' argument is returned as argument 1.

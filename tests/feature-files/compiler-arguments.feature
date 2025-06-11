Feature: All TSLgenerator arguments are honored.
    Scenario: The '-c' argument is specified.
        Given the arguments '-c test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the 'count frames' argument is returned as argument 1.

    Scenario: The '-s' argument is specified.
        Given the arguments '-s test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the 'output to standard output' argument is returned as argument 1.

    Scenario: The '-o' argument is specified.
        Given the arguments '-o one_category_one_choice.txt.tsl test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the 'output to one_category_one_choice.txt.tsl' argument is returned as argument 1.

    Scenario: The input file argument is specified.
        Given the arguments 'test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the 'input from test-files/one_category_one_choice.txt' argument is returned as argument 1.

    Scenario: '-c' and '-s' are specified.
        Given the arguments '-c -s test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the 'count frames' argument is returned as argument 1.
        Then the 'output to standard output' argument is returned as argument 2.
        Then the 'input from test-files/one_category_one_choice.txt' argument is returned as argument 3.

    Scenario: '-c' and '-o' are specified.
        Given the arguments '-o one_category_one_choice.txt.tsl -c test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the 'output to one_category_one_choice.txt.tsl' argument is returned as argument 1.
        Then the 'count frames' argument is returned as argument 2.
        Then the 'input from test-files/one_category_one_choice.txt' argument is returned as argument 3.

    Scenario: An invalid argument returns an error.
        Given the arguments '-e test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the error message returned should read 'Invalid argument: -e'.

    Scenario: Specifying '-o' without an output file returns an error.
        Given the arguments '-o test-files/one_category_one_choice.txt',
        When the arguments are parsed,
        Then the error message returned should read '-o needs an output file argument.'.

    Scenario: Pointing to an input file that does not exist returns an error.
        Given the arguments 'some_tsl_file.txt',
        When the arguments are parsed,
        Then the error message returned should read 'The input file some_tsl_file.txt does not exist.'.

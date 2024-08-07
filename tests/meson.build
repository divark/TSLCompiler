fs = import('fs')

test_dep = dependency('catch2-with-main')
# Without these, the tests will not be able to find their input files since
# paths are relative, and the test-files directory by default is not brought over.
one_category_test_file = fs.copyfile('test-files/one_category_one_choice.txt', 'one_category_one_choice.txt')
multiple_categories_test_file = fs.copyfile('test-files/two_categories_one_choice_each.txt', 'two_categories_one_choice_each.txt')
simple_test_files = [one_category_test_file, multiple_categories_test_file]

single_marking_test_file = fs.copyfile('test-files/one_category_choice_with_marking.txt', 'one_category_choice_with_marking.txt')
error_marking_test_file = fs.copyfile('test-files/one_category_choice_with_error_marking.txt', 'one_category_choice_with_error_marking.txt')
single_and_error_marking_test_file = fs.copyfile('test-files/one_category_choices_with_single_and_error_markings.txt', 'one_category_choices_with_single_and_error_markings.txt')
marking_test_files = [single_marking_test_file, error_marking_test_file, single_and_error_marking_test_file]

choice_with_one_property_test_file = fs.copyfile('test-files/choice_with_one_property.txt', 'choice_with_one_property.txt')
choice_with_multiple_properties_test_file = fs.copyfile('test-files/choice_with_multiple_properties.txt', 'choice_with_multiple_properties.txt')
properties_test_files = [choice_with_one_property_test_file, choice_with_multiple_properties_test_file]

choices_with_simple_if_test_file = fs.copyfile('test-files/choices_with_simple_if.txt', 'choices_with_simple_if.txt')
choice_with_unary_expression_test_file = fs.copyfile('test-files/choice_with_unary_expression.txt', 'choice_with_unary_expression.txt')
choice_with_grouped_expression_test_file = fs.copyfile('test-files/choice_with_grouped_expression.txt', 'choice_with_grouped_expression.txt')
choice_with_and_expression_test_file = fs.copyfile('test-files/choice_with_and_expression.txt', 'choice_with_and_expression.txt')
choice_with_or_expression_test_file = fs.copyfile('test-files/choice_with_or_expression.txt', 'choice_with_or_expression.txt')
choice_with_property_in_if_test_file = fs.copyfile('test-files/choice_with_property_in_if.txt', 'choice_with_property_in_if.txt')
choice_with_single_in_if_test_file = fs.copyfile('test-files/choice_with_single_in_if.txt', 'choice_with_single_in_if.txt')
choice_with_error_in_if_test_file = fs.copyfile('test-files/choice_with_error_in_if.txt', 'choice_with_error_in_if.txt')
choice_with_else_alone_test_file = fs.copyfile('test-files/choice_with_else_alone.txt', 'choice_with_else_alone.txt')
expression_test_files = [choices_with_simple_if_test_file, choice_with_unary_expression_test_file, choice_with_grouped_expression_test_file, choice_with_and_expression_test_file, choice_with_or_expression_test_file, choice_with_property_in_if_test_file, choice_with_single_in_if_test_file, choice_with_error_in_if_test_file, choice_with_else_alone_test_file]

test_files = simple_test_files + marking_test_files + properties_test_files + expression_test_files

# Testing just the TSL Lexer.
lexer_test_sources = files('tsllexer.cpp')
lexer_test_executable = executable('tsllexer-test', sources: lexer_test_sources + app_sources, dependencies: test_dep, include_directories: inc)
test('tsl_lexer', lexer_test_executable, verbose: true, depends: test_files)

# Testing the TSL Parser.
parser_test_sources = files('tslparser.cpp')
parser_test_executable = executable('tslparser-test', sources: parser_test_sources + app_sources, dependencies: test_dep, include_directories: inc)
test('tsl_parser', parser_test_executable, verbose: true, depends: test_files)

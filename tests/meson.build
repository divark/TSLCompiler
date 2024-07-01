fs = import('fs')

test_dep = dependency('catch2-with-main')
# Without this, the tests will not be able to find their input files since
# paths are relative, and the test-files directory by default is not brought over.
one_category_test_file = fs.copyfile('test-files/one_category_one_choice.txt', 'one_category_one_choice.txt')
multiple_categories_test_file = fs.copyfile('test-files/two_categories_one_choice_each.txt', 'two_categories_one_choice_each.txt')
test_files = [one_category_test_file, multiple_categories_test_file]

# Testing just the TSL Lexer.
lexer_test_sources = files('tsllexer.cpp')
lexer_test_executable = executable('tsllexer-test', sources: lexer_test_sources + app_sources, dependencies: test_dep, include_directories: inc)
test('tsl_lexer', lexer_test_executable, verbose: true, depends: test_files)

# Testing the TSL Parser.
parser_test_sources = files('tslparser.cpp')
parser_test_executable = executable('tslparser-test', sources: parser_test_sources + app_sources, dependencies: test_dep, include_directories: inc)
test('tsl_parser', parser_test_executable, verbose: true, depends: test_files)

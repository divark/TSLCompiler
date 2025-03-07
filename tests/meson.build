fs = import('fs')

test_ut_dep = dependency('boost.ut')
# Without these, the tests will not be able to find their input files since
# paths are relative, and the test-files directory by default is not brought over.
one_category_test_file = fs.copyfile('test-files/one_category_one_choice.txt', 'one_category_one_choice.txt')
multiple_categories_test_file = fs.copyfile(
  'test-files/two_categories_one_choice_each.txt',
  'two_categories_one_choice_each.txt',
)
two_categories_two_choices_test_file = fs.copyfile(
  'test-files/two_categories_two_choices_each.txt',
  'two_categories_two_choices_each.txt',
)
simple_test_files = [
  one_category_test_file,
  multiple_categories_test_file,
  two_categories_two_choices_test_file,
]

single_marking_test_file = fs.copyfile(
  'test-files/one_category_choice_with_marking.txt',
  'one_category_choice_with_marking.txt',
)
error_marking_test_file = fs.copyfile(
  'test-files/one_category_choice_with_error_marking.txt',
  'one_category_choice_with_error_marking.txt',
)
single_and_error_marking_test_file = fs.copyfile(
  'test-files/one_category_choices_with_single_and_error_markings.txt',
  'one_category_choices_with_single_and_error_markings.txt',
)
multiple_markings_test_file = fs.copyfile(
  'test-files/choices_with_multiple_markers.txt',
  'choices_with_multiple_markers.txt',
)
marking_test_files = [
  single_marking_test_file,
  error_marking_test_file,
  single_and_error_marking_test_file,
  multiple_markings_test_file,
]

choice_with_one_property_test_file = fs.copyfile('test-files/choice_with_one_property.txt', 'choice_with_one_property.txt')
choice_with_property_in_if_test_file = fs.copyfile(
  'test-files/choice_with_property_in_if.txt',
  'choice_with_property_in_if.txt',
)
choice_with_property_in_else_test_file = fs.copyfile(
  'test-files/choice_with_property_in_else.txt',
  'choice_with_property_in_else.txt',
)
choice_with_multiple_properties_test_file = fs.copyfile(
  'test-files/choice_with_multiple_properties.txt',
  'choice_with_multiple_properties.txt',
)
choice_with_single_in_if_test_file = fs.copyfile('test-files/choice_with_single_in_if.txt', 'choice_with_single_in_if.txt')
choice_with_single_in_else_test_file = fs.copyfile(
  'test-files/choice_with_single_in_else.txt',
  'choice_with_single_in_else.txt',
)
choice_with_error_in_if_test_file = fs.copyfile('test-files/choice_with_error_in_if.txt', 'choice_with_error_in_if.txt')
choice_with_error_in_else_test_file = fs.copyfile('test-files/choice_with_error_in_else.txt', 'choice_with_error_in_else.txt')
properties_test_files = [
  choice_with_one_property_test_file,
  choice_with_multiple_properties_test_file,
  choice_with_single_in_if_test_file,
  choice_with_single_in_else_test_file,
  choice_with_error_in_if_test_file,
  choice_with_property_in_if_test_file,
  choice_with_property_in_else_test_file,
  choice_with_error_in_else_test_file,
]

choices_with_simple_if_test_file = fs.copyfile('test-files/choices_with_simple_if.txt', 'choices_with_simple_if.txt')
choice_with_unary_expression_test_file = fs.copyfile(
  'test-files/choice_with_unary_expression.txt',
  'choice_with_unary_expression.txt',
)
choice_with_grouped_expression_test_file = fs.copyfile(
  'test-files/choice_with_grouped_expression.txt',
  'choice_with_grouped_expression.txt',
)
choice_with_and_expression_test_file = fs.copyfile(
  'test-files/choice_with_and_expression.txt',
  'choice_with_and_expression.txt',
)
choice_with_or_expression_test_file = fs.copyfile('test-files/choice_with_or_expression.txt', 'choice_with_or_expression.txt')
choice_with_else_alone_test_file = fs.copyfile('test-files/choice_with_else_alone.txt', 'choice_with_else_alone.txt')
choice_with_if_label_else_test_file = fs.copyfile('test-files/choice_with_if_label_else.txt', 'choice_with_if_label_else.txt')
expression_test_files = [
  choices_with_simple_if_test_file,
  choice_with_unary_expression_test_file,
  choice_with_grouped_expression_test_file,
  choice_with_and_expression_test_file,
  choice_with_or_expression_test_file,
  choice_with_else_alone_test_file,
  choice_with_if_label_else_test_file,
]

test_files = simple_test_files + marking_test_files + properties_test_files + expression_test_files

# Testing just the TSL Lexer.
lexer_feature_file = fs.copyfile('feature-files/tsllexer.feature', 'tsllexer.feature')
lexer_test_sources = files('tsllexer_ut.cpp')
lexer_test_executable = executable(
  'tsllexer-ut-test',
  sources: lexer_test_sources + app_sources,
  dependencies: test_ut_dep,
  include_directories: inc,
)
test(
  'tsl_lexer',
  lexer_test_executable,
  args: lexer_feature_file,
  verbose: true,
  depends: test_files,
)

# Testing the TSL Parser.
parser_feature_file = fs.copyfile('feature-files/tslparser.feature', 'tslparser.feature')
parser_test_sources = files('tslparser_ut.cpp')
parser_test_executable = executable(
  'tslparser-ut-test',
  sources: parser_test_sources + app_sources,
  dependencies: test_ut_dep,
  include_directories: inc,
)
test(
  'tsl_parser',
  parser_test_executable,
  args: parser_feature_file,
  verbose: true,
  depends: test_files,
)

# Testing the TSL Compiler overall.
compiler_test_sources = files('tslcompiler_ut.cpp')
compiler_feature_file = fs.copyfile('feature-files/tslcompiler.feature', 'tslcompiler.feature')
compiler_test_executable = executable(
  'compiler-test',
  sources: compiler_test_sources + app_sources,
  dependencies: test_ut_dep,
  include_directories: inc,
)
test(
  'compiler',
  compiler_test_executable,
  args: compiler_feature_file,
  verbose: true,
  depends: test_files,
)

graph_test_sources = files('tslchoicegraph.cpp')
graph_feature_file = fs.copyfile('feature-files/tslchoicegraph.feature', 'tslchoicegraph.feature')
graph_test_executable = executable(
  'graph-test',
  sources: graph_test_sources + app_sources,
  dependencies: test_ut_dep,
  include_directories: inc,
)
test(
  'graph',
  graph_test_executable,
  args: graph_feature_file,
  verbose: true,
  depends: test_files,
)

# Testing error messages generated from the TSL Lexer or TSL Parser.
invalid_symbol_test_file = fs.copyfile('test-files/invalid_category.txt', 'invalid_category.txt')
invalid_choice_test_file = fs.copyfile('test-files/invalid_choice.txt', 'invalid_choice.txt')
invalid_property_then_if_test_file = fs.copyfile('test-files/invalid_property_then_if.txt', 'invalid_property_then_if.txt')
invalid_double_marking_test_file = fs.copyfile('test-files/invalid_double_marking.txt', 'invalid_double_marking.txt')
undefined_property_test_file = fs.copyfile(
  'test-files/undefined_property.txt',
  'undefined_property.txt',
)
defined_property_in_same_category_test_file = fs.copyfile(
  'test-files/defined_property_used_in_same_category.txt',
  'defined_property_used_in_same_category.txt',
)
duplicate_property_test_file = fs.copyfile('test-files/duplicate_property.txt', 'duplicate_property.txt')

error_feature_file = fs.copyfile('feature-files/errors.feature', 'errors.feature')

error_test_files = [
  invalid_symbol_test_file,
  invalid_choice_test_file,
  invalid_property_then_if_test_file,
  invalid_double_marking_test_file,
  error_feature_file,
  undefined_property_test_file,
  defined_property_in_same_category_test_file,
  duplicate_property_test_file,
]

error_test_sources = files('errors_ut.cpp')
error_test_executable = executable(
  'errors-test',
  sources: error_test_sources + app_sources,
  dependencies: test_ut_dep,
  include_directories: inc,
)
test(
  'errors',
  error_test_executable,
  args: error_feature_file,
  verbose: true,
  depends: error_test_files,
)

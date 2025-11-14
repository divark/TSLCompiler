test_ut_dep = dependency('boost.ut')

test_dependencies = [test_ut_dep, format_dependency] + compiler_dependencies

working_directory = meson.current_source_dir()

# Testing just the TSL Lexer.
lexer_feature_file = files('feature-files/tsllexer.feature')
lexer_test_sources = files('tsllexer_ut.cpp')
lexer_test_executable = executable(
  'tsllexer-ut-test',
  sources: lexer_test_sources + app_sources,
  dependencies: test_dependencies,
  include_directories: inc,
)
test(
  'tsl_lexer',
  lexer_test_executable,
  args: lexer_feature_file,
  verbose: true,
  workdir: working_directory,
)

# Testing the TSL Parser.
parser_feature_file = files('feature-files/tslparser.feature')
parser_test_sources = files('tslparser_ut.cpp')
parser_test_executable = executable(
  'tslparser-ut-test',
  sources: parser_test_sources + app_sources,
  dependencies: test_dependencies,
  include_directories: inc,
)
test(
  'tsl_parser',
  parser_test_executable,
  args: parser_feature_file,
  verbose: true,
  workdir: working_directory,
)

# Testing the TSL Compiler overall.
compiler_test_sources = files('tslcompiler_ut.cpp')
compiler_feature_file = files('feature-files/tslcompiler.feature')
compiler_test_executable = executable(
  'compiler-test',
  sources: compiler_test_sources + app_sources,
  dependencies: test_dependencies,
  include_directories: inc,
)
test(
  'compiler',
  compiler_test_executable,
  args: compiler_feature_file,
  verbose: true,
  workdir: working_directory,
)

graph_test_sources = files('tslchoicegraph.cpp')
graph_feature_file = files('feature-files/tslchoicegraph.feature')
graph_test_executable = executable(
  'graph-test',
  sources: graph_test_sources + app_sources,
  dependencies: test_dependencies,
  include_directories: inc,
)
test(
  'graph',
  graph_test_executable,
  args: graph_feature_file,
  verbose: true,
  workdir: working_directory,
)

# Testing error messages generated from the TSL Lexer or TSL Parser.
error_feature_file = files('feature-files/errors.feature')
error_test_sources = files('errors_ut.cpp')
error_test_executable = executable(
  'errors-test',
  sources: error_test_sources + app_sources,
  dependencies: test_dependencies,
  include_directories: inc,
)
test(
  'errors',
  error_test_executable,
  args: error_feature_file,
  verbose: true,
  workdir: working_directory,
)

# Testing whether arguments are parsed correctly.
compiler_arguments_feature = files('feature-files/compiler-arguments.feature')
compiler_arguments_sources = files('compiler-arguments.cpp')
compiler_arguments_executable = executable(
  'compiler_arguments-test',
  sources: compiler_arguments_sources + app_sources,
  dependencies: test_dependencies,
  include_directories: inc,
)
test(
  'compiler_arguments',
  compiler_arguments_executable,
  args: compiler_arguments_feature,
  verbose: true,
  workdir: working_directory,
)

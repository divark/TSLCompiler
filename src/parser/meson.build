bison = find_program('bison', version: '>=3.2')
bison_input = files('parser.yy')
bison_generated_sources = custom_target(
  'parser.cpp',
  output: ['parser.cpp', 'parser.hpp', 'location.hh'],
  input: bison_input,
  command: [bison, '-d', '-Wcounterexamples', '-o@OUTPUT0@', '@INPUT@'],
)

parser_sources = files(
  'error_reporting.cpp',
  'expressions.cpp',
  'tsl_collector.cpp',
  'tsl_grammar.cpp',
  'tsl_parser.cpp',
) + bison_generated_sources

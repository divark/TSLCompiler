bison = find_program('bison', version: '>=3.2')
bison_input = files('parser.yy')
bison_generated_sources = custom_target('parser.cpp',
  output: ['parser.cpp', 'parser.hpp'],
  input: bison_input,
  command: [bison, '-d', '-Wcounterexamples', '-o@OUTPUT0@', '@INPUT@' ],
)

parser_sources = files('tsl_parser.cpp', 'expressions.cpp') + bison_generated_sources

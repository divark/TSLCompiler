flex = find_program('flex')
flex_input = files('lexer.l')
flex_generated_sources = custom_target('lexer.cpp',
  output: ['lexer.cpp', 'lexer.hpp'],
  input: flex_input,
  command: [flex, '--c++', '--header-file=@OUTPUT1@', '-o@OUTPUT0@', '@INPUT@' ],
)

bison = find_program('bison', version: '>=3.2')
bison_input = files('parser.yy')
bison_generated_sources = custom_target('parser.cpp',
  output: ['parser.cpp', 'parser.hpp'],
  input: bison_input,
  command: [bison, '-d', '-o@OUTPUT0@', '@INPUT@' ],
)

app_sources = files('tsl_lexer.cpp', 'parse_tree.cpp') + bison_generated_sources + flex_generated_sources

#executable('tslcompiler', 'main.cpp')

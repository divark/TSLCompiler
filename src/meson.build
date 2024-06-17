bison = find_program('bison', version: '>=3.2')
flex = find_program('flex')
flex_input = files('tsl.l')
flex_generated_sources = custom_target('lexer.cpp',
  output: ['lexer.cpp', 'lexer.hpp'],
  input: flex_input,
  command: [flex, '--c++', '--header-file=@OUTPUT1@', '-o@OUTPUT0@', '@INPUT@' ],
)

app_sources = files('compiler.cpp') + flex_generated_sources

#executable('tslcompiler', 'main.cpp')

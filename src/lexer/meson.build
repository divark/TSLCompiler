flex = find_program('flex', version: '>=2.6')
flex_input = files('lexer.l')
flex_generated_sources = custom_target('lexer.cpp',
  output: ['lexer.cpp', 'lexer.hpp'],
  input: flex_input,
  command: [flex, '--c++', '--header-file=@OUTPUT1@', '-o@OUTPUT0@', '@INPUT@' ],
)

lexer_sources = files('tsl_lexer.cpp') + flex_generated_sources

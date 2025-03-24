project(
  'tslcompiler',
  'cpp',
  license: 'GPLv3',
  version: '0.0.1',
  default_options: ['cpp_std=c++20'],
)

format_dependency = dependency('fmt', required: true)

inc = include_directories('include')

subdir('src')
if get_option('build_for_release') == false
  subdir('tests')
endif

executable(
  'tslchecker',
  tslchecker_sources + lexer_sources + parser_sources,
  include_directories: inc,
  dependencies: format_dependency,
)

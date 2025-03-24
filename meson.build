project(
  'tslcompiler',
  'cpp',
  license: 'GPLv3',
  version: '0.0.1',
  default_options: ['cpp_std=c++20'],
)

if meson.get_compiler('cpp').get_id() == 'clang' and meson.get_compiler('cpp').version() == '14.0.0'
  add_global_arguments('-fexperimental-library', language: 'cpp')
endif

inc = include_directories('include')

subdir('src')
if get_option('build_for_release') == false
  subdir('tests')
endif

executable(
  'tslchecker',
  tslchecker_sources + lexer_sources + parser_sources,
  include_directories: inc,
)

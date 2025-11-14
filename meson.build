project(
  'tslcompiler',
  'cpp',
  license: 'GPLv3',
  version: '0.0.1',
  meson_version: '>=1.1',
  default_options: ['cpp_std=c++20'],
)

fmt = subproject('fmt', default_options: 'default_library=static')
format_dependency = fmt.get_variable('fmt_dep')
args = subproject('args')
args_dependency = args.get_variable('args_dep')

compiler_dependencies = [format_dependency, args_dependency]

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

executable(
  'tslcompiler',
  tslcompiler_sources + lexer_sources + parser_sources + compiler_sources,
  include_directories: inc,
  dependencies: compiler_dependencies,
)

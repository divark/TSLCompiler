project('tslcompiler', 'cpp',
  license : 'GPLv3',
  version: '0.0.1',
  default_options: ['cpp_std=c++20'])

inc = include_directories('include')

subdir('src')
if get_option('build_for_release') == false
    subdir('tests')
endif

executable('tslchecker', tslchecker_sources + app_sources, include_directories: inc)

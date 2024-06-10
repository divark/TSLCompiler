test_sources = files('sample.cpp')
test_dep = dependency('catch2-with-main')
test_executable = executable('sample', sources: test_sources + app_sources, dependencies: test_dep, include_directories: inc)
test('catch2 test', test_executable)

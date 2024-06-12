fs = import('fs')

test_sources = files('tslcompiler.cpp')
test_dep = dependency('catch2-with-main')
test_executable = executable('tslcompiler-test', sources: test_sources + app_sources, dependencies: test_dep, include_directories: inc)

# Without this, the tests will not be able to find their input files since
# paths are relative, and the test-files directory by default is not brought over.
test_file = fs.copyfile('test-files/one_category_one_choice.txt', 'one_category_one_choice.txt')

test('tslcompiler parsing tests', test_executable, verbose: true, depends: test_file)

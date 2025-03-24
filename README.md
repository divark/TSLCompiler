# Purpose
The TSLCompiler is a remake of the original [TSLgenerator](https://github.com/alexorso/tslgenerator), a tool used to generate test cases for reference when writing test code. The reasons behind making this edition comes from the limitations of the TSLgenerator, specifically:
- There are situations when certain keywords are ignored due to precedence in the TSL grammar [implemented by the TSLgenerator](https://github.com/alexorso/tslgenerator/blob/master/Docs/TSLgenerator-manual.txt), with no error messages stating this occured.
- While the source code is available, there are four different codebases depending on the Operating System implementing very identical behavior, making it non-trivial to contribute changes.
- Even if the code base was consolidated into one, most of the code is hard to test automatically due to the over-reliance of static variables, and functions that do not return their results by default.

# Goals
The TSLCompiler is designed to be a drop-in replacement for the TSLgenerator, honoring all of the original command-line options. However, this will reject TSL files containing syntax that would generally be accepted, albeit ignored by the TSLgenerator. This is achieved by implementing the grammar explicitly through a Lexer and Parser library.

In addition, the TSLCompiler is developed using the principles behind Test Driven Development/Behavior Driven Development to provide confidence in any proposed changes.

Finally, to remain fairly close to the original C implementation of the TSLgenerator, and to reduce memory leaks, this project utilizes C++.

# Building
## Prerequisites
- A C++ Compiler supporting the C++20 standard (Tested and built with Clang++ on GitHub).
- The Lexer library Flex.
- The Parser library Bison version >= 3.2.
- [The Meson Build System](https://mesonbuild.com/SimpleStart.html).

## Steps
1. Clone this repository.
2. Make a build directory called `build` for meson.
3. Run `meson setup build`, where `build` is the name of the build directory.

# Verifying
1. Open the `build` directory from the `Building` step.
2. Run `meson test`.
3. Confirm that all tests are passing as intended.

# Running
## TSLChecker
1. Open the `build` directory from the `Building` step.
2. Run `meson compile`.
3. Verify that there is now an executable in there called `tslchecker` if you're on Linux or macOS, `tslchecker.exe` if you're on Windows.
4. Run `./tslchecker path/to/input/file`.

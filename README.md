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
- A C++ Compiler supporting the C++20 standard.
- The Lexer library Flex.
- The Parser library Bison version >= 3.2.
- [The Meson Build System](https://mesonbuild.com/SimpleStart.html).
- [Catch2](https://github.com/catchorg/Catch2), which can be installed into the project locally using meson via `meson wrap install catch2`

## Steps
1. Clone this repository.
2. Make a build directory called `build` for meson.
3. Run `meson setup build`, where `build` is the name of the build directory.
4. Open the `build` directory, then run `meson test` to check the status of the codebase via compilation and tests ran.

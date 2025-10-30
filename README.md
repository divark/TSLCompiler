# Purpose
The TSLCompiler is a remake of the original [TSLgenerator](https://github.com/alexorso/tslgenerator), a tool used to generate test cases for reference when writing test code. The reasons behind making this edition comes from the limitations of the TSLgenerator, specifically:
- There are situations when certain keywords are ignored due to precedence in the TSL grammar [implemented by the TSLgenerator](https://github.com/alexorso/tslgenerator/blob/master/Docs/TSLgenerator-manual.txt), with no error messages stating this occured.
- While the source code is available, there are four different codebases depending on the Operating System implementing very identical behavior, making it non-trivial to contribute changes.
- Even if the code base was consolidated into one, most of the code is hard to test automatically due to the over-reliance of static variables, and functions that do not return their results by default.

# Goals
The TSLCompiler is designed to be a drop-in replacement for the TSLgenerator, honoring all of the original command-line options. However, this will reject TSL files containing syntax that would generally be accepted, albeit ignored by the TSLgenerator. This is achieved by implementing the grammar explicitly through a Lexer and Parser library.

In addition, the TSLCompiler is developed using the principles behind Test Driven Development/Behavior Driven Development to provide confidence in any proposed changes.

Finally, to remain fairly close to the original C implementation of the TSLgenerator, and to reduce memory leaks, this project utilizes C++.

# Running
It's highly recommended to download and use the [pre-built binaries](https://github.com/divark/TSLCompiler/releases/latest).

If they do not work, go to the [Building](#Building) section for compilation instructions.
## TSLCompiler
1. Open the `build` directory from the `Building` step.
2. Run `meson compile`.
3. Verify that there is now an executable in there called `tslcompiler` if you're on Linux or macOS, `tslcompiler.exe` if you're on Windows.
4. Run `./tslcompiler path/to/input/file`.
  - For more information on available arguments, consult the [Manual.](docs/USER_MANUAL.md)
## TSLChecker
1. Open the `build` directory from the `Building` step.
2. Run `meson compile`.
3. Verify that there is now an executable in there called `tslchecker` if you're on Linux or macOS, `tslchecker.exe` if you're on Windows.
4. Run `./tslchecker path/to/input/file`.

# Building
## Prerequisites
- A C++ Compiler supporting the C++20 standard (Tested and built with Clang++ on GitHub).
- The Lexer library Flex.
- The Parser library Bison version >= 3.2.
- The fmt library, which can be installed via `meson wrap install fmt`.
- [The Meson Build System](https://mesonbuild.com/SimpleStart.html).

## Steps
1. Clone this repository.
2. (Recommended) Install the fmt library with `meson wrap install fmt`

### macOS
3. Install the latest versions of meson, flex and bison.
    - With brew: `brew install meson bison flex` 
4. Run `meson setup --native-file macos-build.ini build`, where `build` is the name of the build directory.

### Ubuntu
3. Install the latest versions of cmake, flex and bison.
    - With apt: `sudo apt install libbison-dev bison flex cmake libfl-dev` 
4. Install the latest version of clang++
    - With apt: `sudo apt install clang`
    - NOTE: `linux-build.ini` expects clang++-19 for the CI/CD. On your own, so as long as clang supports C++20, you can remove the -19. If your version of clang is too old, use LLVM's [Automatic installation script](https://apt.llvm.org/) and specify version 19.
5. Install the latest version of meson with `sudo apt install meson`
    - NOTE: If the version of meson is too old for the project, install it with `pipx install meson`
6. Run `meson setup --native-file linux-build.ini build`, where `build` is the name of the build directory.

# Verifying
1. Open the `build` directory from the `Building` step.
2. Run `meson test`.
3. Confirm that all tests are passing as intended.

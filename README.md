[![progress-banner](https://backend.codecrafters.io/progress/shell/d5e900a2-6dd1-468d-b389-a119b9912b16)](https://app.codecrafters.io/users/Shriya1181?r=2qF)

This is a starting point for C++ solutions to the
["Build Your Own Shell" Challenge](https://app.codecrafters.io/courses/shell/overview).

In this challenge, you'll build your own POSIX compliant shell that's capable of
interpreting shell commands, running external programs and builtin commands like
cd, pwd, echo and more. Along the way, you'll learn about shell command parsing,
REPLs, builtin commands, and more.

**Note**: If you're viewing this repo on GitHub, head over to
[codecrafters.io](https://codecrafters.io) to try the challenge.

# Passing the first stage

The entry point for your `shell` implementation is in `src/main.cpp`. Study and
uncomment the relevant code, then run the command below to execute the tests on
our servers:

```sh
cmake -B build -S .
cmake --build ./build
./build/shell
```

Notes:
- If `VCPKG_ROOT` is set incorrectly, the helper script may fail; in that
  case either set `VCPKG_ROOT` correctly or run the manual build without the
  toolchain file as shown above.
- The built binary is `./build/shell`.

### What the current implementation does

- The `shell` is a REPL implemented in `src/main.cpp`.
- Implemented builtins: `echo`, `type`, `exit`, `pwd`, and `cd`.
- Behavior details:
  - Prints prompt `$ ` and reads a full input line (whitespace-separated
    arguments are parsed with a simple splitter).
  - `exit` exits the loop and terminates `main` (normal exit).
  - `echo` prints its arguments separated by spaces.
  - `type <cmd>` reports whether `<cmd>` is a shell builtin or prints the
    file path to the executable found in `PATH`; prints `not found` if missing.
  - `pwd` prints the current working directory.
  - `cd <path>` changes the working directory; supports `~` expansion to the
    `HOME` directory and reports an error if the target doesn't exist or is
    not a directory.
  - External commands: the shell searches the directories from the `PATH`
    environment variable for an executable file (checks existence, regular
    file, and owner execute permission). If found, the shell forks and uses
    `execv` to run the program, waiting for the child to finish before
    printing the next prompt.
  - Empty input lines are ignored and the prompt is reprinted.
  - `std::cout` and `std::cerr` are set to `std::unitbuf` so output is flushed
    immediately.

## Project layout

- `src/` — source files (`src/main.cpp` is the entry point).
- `CMakeLists.txt` — CMake project file.
- `your_program.sh` — local build + run script.

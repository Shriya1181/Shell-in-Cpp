## SHELL-CPP — Basic Shell Implemented in C++

A minimal REPL shell implementation. Entry point is `src/main.cpp` and the
repository contains build tooling to compile and run it locally.

## Features

- Minimal starter `shell` program following REPL loop.
- CMake build system (C++23).
- Convenience scripts for local build/run.

## Requirements

- CMake 3.13+ and a C++23-capable compiler (clang/gcc).
- `vcpkg` is referenced by the build scripts; if you rely on it, ensure
  `VCPKG_ROOT` is set in your environment. No vcpkg dependencies are required
  by default.

## How to Set Up and Run

### Prerequisites

- **CMake 3.13+** and a **C++23** capable compiler (clang or gcc).
- **POSIX-compatible OS** (Linux or macOS) is recommended — the shell uses
  `fork`/`execv` and `waitpid` which rely on POSIX APIs.
- **vcpkg (optional)**: the provided scripts reference vcpkg. If you want to
  use vcpkg, set the `VCPKG_ROOT` environment variable to your vcpkg
  installation directory. No vcpkg packages are required by default.

### Installation

1. Clone the repository (HTTPS):

```sh
git clone https://github.com/Shriya1181/Shell-in-Cpp.git
cd Shell-in-Cpp
```

2. Build and run using the helper script:

```sh
./your_program.sh
```

This script configures CMake to build into `./build` and then executes
`./build/shell`.

### Manual build (without the helper script)

If you prefer to run the CMake steps manually or wish to omit vcpkg usage,
follow these commands from the repository root:

With vcpkg (if `VCPKG_ROOT` is set):

```sh
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
cmake --build ./build
./build/shell
```

Without vcpkg (omit the toolchain file):

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

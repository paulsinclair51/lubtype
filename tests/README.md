# lubtype Tests

Copyright (c) 2026 paulsinclair51  
SPDX-License-Identifier: MIT

This directory contains the regression and unit test suite for [lubtype.h](../lubtype.h).

## Building and Running

### Linux / macOS

From this directory:

```sh
make
make run
```

Or from the repository root:

```sh
make -C tests run
```

To build with gcc instead of clang:

```sh
make -C tests CC=gcc run
```

### Windows (MSVC)

From this directory:

```powershell
powershell -ExecutionPolicy Bypass -File ./build_lubtype_tests.ps1
.\lubtype_tests.exe
```

### Makefile targets

| Target  | Description                              |
|---------|------------------------------------------|
| `all`   | Build the `lubtype_tests` executable     |
| `run`   | Build and run the test suite             |
| `clean` | Remove object files and the executable   |

Running the test suite writes `lubtype_tests_report.txt` next to the executable.

## Test Files

`lubtype_tests.c` is the main orchestrator. It calls each module's `run_*()` function, aggregates results, and writes the report.

| File                    | What it tests                                              | Compiled       |
|-------------------------|------------------------------------------------------------|----------------|
| `lubtype_tests.c`       | Main runner and report writer                              | once           |
| `test_type_matrix.c`    | Cross-type bounded (`n`/`nn`) APIs                         | once           |
| `test_reserved_matrix.c`| Reserved word matrix and related logic                     | once           |
| `test_fuzz_edge.c`      | Fuzz and edge cases for string operations                  | once           |
| `test_charclass.c`      | Character classification and case transformation           | twice (L + U)  |
| `test_cmp_search.c`     | String comparison and search functions                     | twice (L + U)  |
| `test_strlen_strclass.c`| String length and classification functions                 | twice (L + U)  |
| `test_count.c`          | Character counting functions                               | twice (L + U)  |
| `test_skip.c`           | `xxsnskip` and cross-type `ulsnskip`                       | twice (L + U)  |
| `test_search_families.c`| Search and compare function families                       | twice (L + U)  |
| `test_core_families.c`  | Core function families (`xx`, plus `lu`/`ul` cross-type)   | twice (L + U)  |
| `test_advanced_ops.c`   | Advanced string operations (pad, trim, reverse, repeat, replace) | twice (L + U) |
| `test_error_edge.c`     | Error and edge cases for string operations                 | twice (L + U)  |
| `test_utilities.c`      | Utility functions (reverse, trim, pad, repeat)             | twice (L + U)  |
| `test_xmacros.c`        | X-macro alias mappings (`isx*` → `isl*` and `isu*`)        | twice (L + U)  |

## X-Macro Dual Compilation

Files marked **twice (L + U)** in the table above are compiled twice from the same source:

- once with `-DLUB_X_IS_L` — exercises the Latin (`lchar_t`) API
- once with `-DLUB_X_IS_U` — exercises the Unicode (`uchar_t`) API

This allows a single test source file to cover both encodings via the x-macro aliases defined in `lubtype.h` (e.g. `xchar_t`, `isxalpha`). The two resulting object files are linked into the single `lubtype_tests` executable. See [Header Usage](../README.md#header-usage) in the root README for background on the x-macro pattern.

## Test Framework

The test framework is defined in `lubtype_test_declarations.h`. Key points:

- Each test module has a `run_*()` function that returns a `lub_test_result_t` with `pass`, `fail`, and `exception` counts.
- The `LUB_ASSERT` macro wraps every assertion with per-assert signal handlers (SIGSEGV, SIGABRT, SIGBUS). Faults are caught and counted as `exception` rather than crashing the suite, allowing remaining assertions to continue executing.
- Fault injection can be enabled via the `inject_faults` argument passed to each `run_*()` function.

## Adding a New Test Module

1. Create `test_<name>.c`. Add the `@file` / `@brief` doc comment, a file-scoped `static lub_test_result_t test_result;`, and a `run_<name>_tests(int inject_faults)` function.
2. Declare `run_<name>_tests` in `lubtype_test_declarations.h`.
3. Call it from `lubtype_tests.c` and accumulate its result.
4. Add the source to `TEST_SOURCES` in the Makefile (compiled once), or add explicit `_l.o` / `_u.o` rules and add them to `X_OBJECTS` (compiled twice).

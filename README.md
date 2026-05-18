# lubtype

[GitHub Actions](https://github.com/paulsinclair51/lubtype/actions)

Copyright (c) 2026 paulsinclair51  
SPDX-License-Identifier: MIT  
For license details, see the LICENSE file in the project root.

lubtype is a C/C++ API for Latin-8 (lchar_t), Unicode-16 (uchar_t), and raw byte (byte_t) character and string operations. The API is declared and defined by a single header file in this repository:

- lubtype.h - core API and version macros.

The documented API version is 1.0.0 (LUB_VERSION in lubtype.h).

See lubtype.h for the full API reference and more examples.

*Tip: To view this file with formatting applied, Ctrl-Shift-V or see [Viewing Formatted README](#viewing-formatted-readme).*

## Header Usage

Include lubtype.h in every source file that uses the API.

```c
#include "lubtype.h"
```

The API is declared and defined by this single header file, but function
body definitions are emitted only when the macro LUB_DEFINITIONS is defined.
Define this macro in exactly one source file per final binary. In practice, this
source file is usually a LUB_DEFINITIONS-only source file such
as the following:

```c
// File: lubdefinitions.c
// Brief: Provide function definitions for the LUB API.
#if !defined(LUB_DEFINITIONS)
#define LUB_DEFINITIONS
#endif
#include "lubtype.h"
// End of lubdefinitions.c
```

This repository includes a sample source file as [lubdefinitions.c](lubdefinitions.c)
that expands on the example above.

All other source files must include lubtype.h without defining LUB_DEFINITIONS
in the source file or in the compile command.

To use the x macros (see Polymorphic Macros), the names of extern
functions in the source file must be defined based on LUB_X_IS_L or
LUB_X_IS_U. For example:

```c
#if defined(LUB_X_IS_L) && defined(LUB_X_IS_U)
#error "Both LUB_X_IS_L and LUB_X_IS_U are defined. "
       "Specify either -DLUB_X_IS_L or -DLUB_X_IS_U, but not both. "
       "If neither is defined, LUB_X_IS_L is defined as a validator-friendly default."
#endif
#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
// Set default.
#define LUB_X_IS_L
#endif
// Define xfunc as lfunc or ufunc based on LUB_X_IS_L or LUB_X_IS_U.
#if defined(LUB_X_IS_L) 
#define xfunc lfunc
#else
#define xfunc ufunc
#endif

extern int xfunc(...)
{ <function body> }
```

Then compile using the define flag (-D) to define LUB_X_IS_L:

```sh
gcc -DLUB_X_IS_L -c myfile.c -o myfile.o
```

Or to define LUB_X_IS_U:

```sh
gcc -DLUB_X_IS_U -c myfile.c -o myfile.o
```

## Design Principles

The API is designed for symmetry, clarity, safety, predictability, portability, and C/C++ compatibility. See lubtype.h for full details and caveats.

## What The API Covers

The lubtype.h API includes:

- Token-handling and compile-time assertion macros.
- Versioning macros.
- Character classification and case transformation.
- Hex-digit conversion.
- Bounded string-length validation.
- String classification.
- Compare, fixed-length compare, prefix compare, and suffix compare.
- Pointer-trim and skip.
- Search and count.
- Concatenate and copy.
- Quoted concatenate and copy.
- Pad, trim, reverse, repeat, and replace.
- Latin formatting.

Operations are provided in Latin/Unicode/byte combinations where they are meaningful. Function names encode the target/source types, bound behavior, operation, and case behavior.

## Polymorphic Macros

Polymorphic macros map x macros onto LUB API symbols. With x macros, one source file can be built twice: once for Latin and once for Unicode (see [Header Usage](#header-usage)). For example:

|                 | #if defined(LUB_X_IS_L) | #if defined(LUB_X_IS_U) |
| x macro         | maps to:                | maps to:                |
|-----------------|-------------------------|-------------------------|
| xchar_t         | lchar_t                 | uchar_t                 |
| LUB_MAX_XCHAR   | LUB_MAX_LCHAR           | LUB_MAX_UCHAR           |
| LUB_MAX_XSTRLEN | LUB_MAX_LSTRLEN         | LUB_MAX_USTRLEN         |
| isxalpha        | islalpha                | isualpha                |
| xltolower       | lltolower               | ultolower               |
| xxsnncat        | llsnncat                | uusnncat                |

## Quick Start

```c
#define LUB_DEFINITIONS
#include "lubtype.h"

int main(void) {
    lchar_t src[] = "hello";
    uchar_t dst[16];

    if (!ulsnncpy(dst, 15, src, 5, NULL)) {
        return 1;
    }

    return 0;
}
```

## Examples

```c
/* Search for the second occurrence of "two". */
const lchar_t haystack[] = "one two two";
const lchar_t needle[] = "two";
const lchar_t *match = llsnstrm(haystack, sizeof(haystack) - 1, needle, 0, 2);
/* match -> points to the second "two" */
```

```c
/* Trim both ends and collapse runs of whitespace to a single space. */
lchar_t out[16 + 1];
const lchar_t spaced[] = "  abc  ";
llsnntrim(out, 16, spaced, 16, NULL, "B", ' ');
/* out -> "abc" */
```

```c
/* Replace the second occurrence of "cat" with "dog". */
lchar_t buf[32 + 1];
const lchar_t animals[] = "cat and cat";
const lchar_t map[] = "cat|dog";
llsnnreplace(buf, 32, animals, 32, map, '|', 2);
/* buf -> "cat and dog" */
```

```c
/* Pad with '.' and centered with odd-width padding: extra pad
 * character goes on the right. */
lchar_t padded[8 + 1];
const lchar_t src[] = "cat";
llsnnpad(padded, 8, src, 8, "B.");
/* padded -> "..cat..." */
```

## Version Macros

```c
LUB_VERSION_MAJOR                /* 1 */
LUB_VERSION_MINOR                /* 0 */
LUB_VERSION_PATCH                /* 0 */
LUB_VERSION                     /* "1.0.0" */
LUB_VERSION_NUM                 /* 10000 */
LUB_VERSION_HEX                 /* 0x010000 */
LUB_VERSION_EQ(1, 0, 0)         /* true if exactly 1.0.0 */
LUB_VERSION_AT_LEAST(1, 0, 0)   /* true if >= 1.0.0 */
```

## Utility Macros

The header provides public token-handling and compile-time assertion macros:

- **LUB_PASTE** and **LUB_STRINGIFY**: Macros for token pasting and stringification. (If these or their internal forms are already defined before including lubtype.h, a compile error is raised.)
- **LUB_STATIC_ASSERT**: Compile-time assertion macro. (A compile error is raised if this macro is already defined before including lubtype.h.)

## Core Types And Limits

| Type    | Base type | Notes                              |
|---------|-----------|------------------------------------|
| lchar_t | uint8_t   | Latin-8 code unit                  |
| uchar_t | uint16_t  | Unicode BMP code unit              |
| byte_t  | uint8_t   | Raw byte, no null-string semantics |

| Macro            | Value   |
|------------------|---------|
| LUB_MAX_LCHAR    |     255 |
| LUB_MAX_UCHAR    |   65535 |
| LUB_MAX_BYTE     |     255 |
| LUB_MAX_LSTRLEN  | 1000000 |
| LUB_MAX_USTRLEN  |  500000 |
| LUB_MAX_NAMELEN  |     128 |
| LUB_MAX_QNAMELEN |     258 |
| LUB_MAX_LOPTLEN  |   64000 |
| LUB_MAX_UOPTLEN  |   32000 |
| LUB_MAX_BSTRLEN  | 1000000 |
| LUB_MAX_BOPTLEN  |   64000 |

Caller-supplied bounds are clamped to the corresponding maximums.

## Return Values And Error Handling

Comparison functions return:

- LUB_CMP_LESS_THAN = -1
- LUB_CMP_EQUAL = 0
- LUB_CMP_GREATER_THAN = 1
- error value

Name classification functions return:

- LUB_UNQUOTEDNAME = 0
- LUB_QUOTEDNAME = 1
- error value (see below)

String classification functions (isln*, isun*, islreserved, isureserved, islqname, isuqname) return:

- 1 for satisfied condition and no error.
- 0 for unsatisfied condition and no error.
- error value (see below).

Reserved error values occupy the range -99 to -2 for int (with equivalent values for size_t and intptr_t). The currently defined error values for int, size_t, and pointer returning functions are:

- LUB_PTR_INVALID
- LUB_UNTERMINATED
- LUB_NONLATIN_SOURCE
- LUB_BSTR_TOO_LONG
- LUB_NAME_TOO_LONG
- LUB_NAME_INVALID
- LUB_OPT_TOO_LONG
- LUB_OPT_INVALID
- LUB_OPT_RESERVED
- LUB_NON_LATIN_CHAR
- LUB_OVERLAP
- LUB_TRUNCATED
- LUB_INTERNAL_ERROR

Use the error classification/cast macros instead of comparing a value directly with an error value:

```c
size_t len = lcsnlen(src, 32);
if (LUB_SIZE_ERR(len, 0)) {
  /* handle error */
}
```

Three macros handle the three return types:

- LUB_SIZE_ERR(value, error) - for size_t-returning functions.
- LUB_PTR_ERR(value, error)  - for pointer-returning functions.
- LUB_INT_ERR(value, error)  - for int-returning functions.

Pass 0 as the error argument to test for any error, or pass a specific error value to test for that exact error. Each macro returns the error cast to the appropriate type, or 0 if not an error.

If LUB_PTR_ERR is used to return an error to a caller, cast it to the specific pointer type for the function result.

## Naming Conventions

The naming grammar in lubtype.h is the authoritative reference. In short:

- l, u, and b identify Latin, Unicode, and byte families.
- ll, lu, ul, uu, lb, ub, bl, and bu identify target/source combinations.
- n means a bounded source argument is present.
- nn means both target size and source bound are explicit.
- Lowercase operation suffixes like cpyc are lowercase-transform variants.
- Uppercase operation suffixes like CMP and REPLACE are case-insensitive variants.

Examples:

- llsncmp - Latin/Latin bounded compare, case-sensitive
- ulsnCMP - Unicode/Latin bounded compare, case-insensitive
- llsnncpy - Latin target, Latin source, explicit target and source bounds
- uusnnREPLACE - Unicode target/source replace, explicit bounds, case-insensitive

## Reserved Names

Besides the API-provided type and function names (see Naming Conventions), the API reserves names of the following forms:

- "LUB_<W>[_<W>]..." for macro names (W = uppercase letters)
- "lub_<w>[_<w>]..." for function names (w = lowercase letters)

See lubtype.h for reserved name patterns and conflict resolution policy.

## API Notes and Assumptions

- Target buffers are always null-terminated on error.
- No dynamic allocation or recursion; most functions are static inline.
- Some operations are O(n*m); see lubtype.h for performance notes.
- lchar_t/uchar_t/byte_t types and encoding assumptions are detailed in lubtype.h.

See lubtype.h for all API notes, error handling, and platform requirements.

## Building And Running Tests

From the tests directory:

```sh
make
make run
```

On Windows, use the provided PowerShell helper in [tests/build_lubtype_tests.ps1](tests/build_lubtype_tests.ps1).

### Windows (MSVC)

From the tests directory:

```powershell
powershell -ExecutionPolicy Bypass -File ./build_lubtype_tests.ps1
.\lubtype_tests.exe
```

### Linux / macOS

From the repository root:

```sh
make -C tests run
```

To build with gcc instead of clang:

```sh
make -C tests CC=gcc run
```

The checked-in test runner writes lubtype_tests_report.txt next to the test executable when executed.

For the current test module matrix (including per-file coverage and which files are built once vs. twice with x-macro defines), see [tests/README.md](tests/README.md).

## Continuous Integration

GitHub Actions runs the test suite on pushes and pull requests in the hosted repository. Use the GitHub Actions link at the top of this README for the current CI state.

## Viewing Formatted README

To view the formatted README while editing in VS Code:

- Open README.md and press Ctrl+Shift+V.
- For side-by-side editor + preview, press Ctrl+K then V.
- To revert to unformatted viewing, close and reopen.

For repository formatted viewing, open the README on GitHub after committing changes.

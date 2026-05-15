# lubtype

[GitHub Actions](https://github.com/paulsinclair51/lubtype/actions)

Copyright (c) 2026 paulsinclair51  
SPDX-License-Identifier: MIT  
For license details, see the LICENSE file in the project root.

lubtype is a header-only C/C++ API for Latin-8 (lchar_t), Unicode-16 (uchar_t), and raw byte (byte_t) character and string operations.

The repository includes onr header file:

- lubtype.h - core API and version macros.

The documented API version is 1.0.0 (LUB_VERSION in lubtype.h).

See lubtype.h for the full API reference and more examples.

*Tip: To view this file with formatting applied, Ctrl-Shift-V or see [Viewing Formatted README](#viewing-formatted-readme).*

## Header Usage

Include lubtype.h in every source file that uses the API.

```c
#include "lubtype.h"
```

The API is header-only, but function body definitions are emitted only when the macro LUB_DEFINITIONS is defined. Define this macro in exactly one source file
per final binary. In practice, this source file is usually a
LUB_DEFINITIONS-only source file such as the following:

```c
/**
 * @file lubdefinitions.c
 * @brief Provide function definitions for the LUB API.
 */
#if !defined(LUB_DEFINITIONS)
#define LUB_DEFINITIONS
#endif
#include "lubtype.h"
/* End of lubdefinitions.c */
```

This repository includes this sample source file as [lubdefinitions.c](lubdefinitions.c).

All other source files must include lubtype.h without defining LUB_DEFINITIONS
in the source file or in the compile command.

To use the x macros, the names of extern functions in the source file
must be defined based on LUB_X_IS_L or LUB_X_IS_U. For example:

```c
#if defined(LUB_X_IS_L) 
#define xfunc lfunc
#elif defined(LUB_X_IS_U)
#define xfunc ufunc
#else
#error "Neither LUB_X_IS_L nor LUB_X_IS_U. Specify option -DLUB_X_IS_L "
       "or -DLUB_X_IS_U in compile command. For example, "
       "gcc -DLUB_X_IS_L -c myfile.c -o myfile.o."
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

lubtype.h is built around six guiding principles documented in the header:

- Symmetry: operations exist for every encoding direction where meaningful.
- Clarity: function names encode type, direction, bounds, operation, and case behavior.
- Safety: explicit bounds, terminator validation, representability checks,
  and defined error values throughout.
- Predictability: behavior mirrors familiar C string patterns; comparison
  functions operate on raw code units only with no normalization or substitution.
- Portability: requires only standard C headers; compatible with C90 and later.
- Compatibility: usable from C and C++; all declarations are wrapped in
  extern "C" when compiled as C++.

## What The API Covers

The lubtype.h API includes:

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
|-----------------|---------------------------------------------------|
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

The header also provides public token-handling and compile-time assertion macros:

- LUB_PASTE and LUB_STRINGIFY are macros for token pasting and stringification.
- LUB_STATIC_ASSERT is used for compile-time checks.

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
If LUB_PTR_ERR is used to return an error to a caller, cast it to the specific pointer type.

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

The API reserves the following name patterns for current and future use:

- LUB_<W>[_<W>]... for macro names (<W> is uppercase letters).
- lub_<w>[_<w>]... for function names (<w> is lowercase letters).

If a name conflict arises with a macro, #undef the conflicting macro before including lubtype.h and redefine it afterward if needed. For type or function conflicts, rename the conflicting symbol in the including code. See lubtype.h for the full reserved names policy.

## Notes

- Unicode classification and case conversion use the C runtime's isw* and tow* routines, so results are locale- and
  CRT-dependent.
- The API operates on individual code units only. It does not perform Unicode normalization or surrogate-pair handling.
- When a function writes into a caller-supplied target buffer, the target is null-terminated on error.
- Overlapping source and target buffers are implementation-defined: a function may succeed or return an error depending
  on the operation.
- No dynamic memory allocation or recursion is used.
- The API is thread-safe as long as threads do not concurrently write to the same target buffer without external synchronization.
- Search and replace functions have worst-case O(n*m) complexity where n is input length and m is pattern or map size. Most other operations are O(n).
- The header is compatible with C90 and later, and with C++. All declarations are wrapped in extern "C" when compiled as C++.
- Supported platforms must satisfy the compile-time assertions documented in lubtype.h.

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

The checked-in test runner writes lubtype_tests_report.txt to its current working directory when executed.

## Continuous Integration

GitHub Actions runs the test suite on pushes and pull requests. If workflow files are included in your checkout, see .github/workflows/ci.yml.

## Viewing Formatted README

To view the formatted README while editing in VS Code:

- Open README.md and press Ctrl+Shift+V.
- For side-by-side editor + preview, press Ctrl+K then V.
- To revert to unformatted viewing, close and reopen.

For repository formatted viewing, open the README on GitHub after committing changes.

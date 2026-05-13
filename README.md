# lubtype

[GitHub Actions](https://github.com/paulsinclair51/lubtype/actions)

Copyright (c) 2026 paulsinclair51  
SPDX-License-Identifier: MIT  
For license details, see the LICENSE file in the project root.

lubtype is a header-only C/C++ API for Latin-8 (lchar_t), Unicode-16 (uchar_t), and raw byte (byte_t) character and string operations.

The repository includes three headers:

- lubtype.h - core API and version macros.
- xlatin.h - polymorphic x... macro remapping to Latin-backed operations.
- xunicode.h - polymorphic x... macro remapping to Unicode-backed operations.

The current documented API version is 1.0.0 (LUB_VERSION in lubtype.h).

## Header Layout

Include lubtype.h everywhere you use the API.

#include "lubtype.h"

The API is header-only, but function body definitions are emitted only when macro LUB_DEFINITIONS is defined before the include. Define this macro in exactly one source file per final binary:

#define LUB_DEFINITIONS
#include "lubtype.h"

All other source files must include lubtype.h without that define.

## What The API Covers

The lubtype.h API includes:

- Character classification and case transformation.
- Hex-digit conversion.
- Bounded string-length validation.
- String classification.
- Compare, fixed-length compare, prefix compare, and suffix compare.
- Pointer-trim, and skip.
- Search and count.
- Concatenate and copy.
- Pad, trim, reverse, repeat, and replace.
- Latin formatting (llsnprintf, llsnvprintf).

Operations are provided in Latin/Unicode/byte combinations where they are meaningful. Function names encode the target/source types, bound behavior, operation, and case behavior.

## Polymorphic Mapping Headers

xlatin.h and xunicode.h remap x macros onto existing lubtype.h symbols so one source file can be built twice: once for Latin and once for Unicode. For example,

 - LUB_MAX_XCHAR maps to LUB_MAX_LCHAR with xlatin.h and to LUB_MAX_UCHAR with xunicode.h.
 - xltolower maps to lltolower with xlatin.h and to ultolower with xunicode.h.
 - xxsnncat maps to llsnncat with xlatin.h and to uusnncat with xunicode.h.

## Quick Start

#define LUB_DEFINITIONS
#include "lubtype.h"

int main(void) {
    lchar_t src[] = { 'h', 'e', 'l', 'l', 'o', 0 };
    uchar_t dst[16];

    if (!ulsnncpy(dst, 15, src, 5, NULL)) {
        return 1;
    }

    return 0;
}

## Verified Examples

/* Search for the second occurrence of "two". */
const lchar_t haystack[] = { 'o','n','e',' ','t','w','o',' ','t','w','o',0 };
const lchar_t needle[] = { 't','w','o',0 };
const lchar_t *match = llsnstrm(haystack, needle, 32, 0, 2);

/* Trim both ends and collapse runs of whitespace to a single space. */
lchar_t out[16 + 1];
const lchar_t spaced[] = { ' ',' ','a','b','c',' ',' ',0 };
llsnntrim(out, 16, spaced, 16, 0, NULL, 'B', ' ');

/* Replace the second occurrence of "cat" with "dog". */
lchar_t out[32 + 1];
const lchar_t src[] = { 'c','a','t',' ','a','n','d',' ','c','a','t',0 };
const lchar_t map[] = { 'c','a','t','|','d','o','g',0 };
llsnnreplace(out, 32, src, 32, map, '|', 2);

/* Pad centered with odd-width padding: extra pad character goes on the right. */
lchar_t out[8 + 1];
const lchar_t src[] = { 'c','a','t',0 };
llsnnpad(out, 8, src, 8, "B.");
/* out -> "..cat..." */

See lubtype.h, xlatin.h, and ulatin.h for the full API reference and additional examples,.

## Version Macros

LUB_VERSION                     /* "1.0.0" */
LUB_VERSION_NUM                 /* 10000 */
LUB_VERSION_HEX                 /* 0x010000 */
LUB_VERSION_EQ(1, 0, 0)         /* true if exactly 1.0.0 */
LUB_VERSION_AT_LEAST(1, 0, 0)   /* true if >= 1.0.0 */

## Core Types And Limits

| Type    | Base type | Notes                              |
|---------|-----------|------------------------------------|
| lchar_t | uint8_t   | Latin-8 code unit                  |
| uchar_t | uint16_t  | Unicode BMP code unit              |
| byte_t  | uint8_t   | Raw byte, no null-string semantics |

| Macro | Value |
|-------|-------|
| LUB_MAX_LCHAR | 255 |
| LUB_MAX_UCHAR | 65535 |
| LUB_MAX_BYTE | 255 |
| LUB_MAX_LSTRLEN | 1000000 |
| LUB_MAX_USTRLEN | 500000 |
| LUB_MAX_NAMELEN | 128 |
| LUB_MAX_QNAMELEN | 258 |
| LUB_MAX_LOPTLEN | 64000 |
| LUB_MAX_UOPTLEN | 32000 |
| LUB_MAX_BSTRLEN | 1000000 |
| LUB_MAX_BOPTLEN | 64000 |

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

- 1 for satisfied condition and no error
- 0 for unsatisfied condition and no error
- error value (see below)

Reserved error values occupy the range -99 to -2. The currently defined error values for int, size_t, and pointer-returning functions are:

- LUB_PTR_INVALID
- LUB_UNTERMINATED
- LUB_BSTR_TOO_LONG
- LUB_NAME_TOO_LONG
- LUB_NAME_INVALID
- LUB_OPT_TOO_LONG
- LUB_OPT_INVALID
- LUB_OPT_RESERVED
- LUB_NONLATIN_SOURCE
- LUB_NON_LATIN_CHAR
- LUB_OVERLAP
- LUB_TRUNCATED
- LUB_INTERNAL_ERROR

Use the error classification/cast macros instead of comparing a value directly with an error value:

c
size_t len = lcsnlen(src, 32);
if (LUB_SIZE_ERR(len, 0)) {
    /* handle error */
}


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

## Notes

- Unicode classification and case conversion use the C runtime's isw* and tow* routines, so results are locale- and
  CRT-dependent.
- The API operates on individual code units only. It does not perform Unicode normalization or surrogate-pair handling.
- When a function writes into a caller-supplied target buffer, the target is null-terminated on error.
- Overlapping source and target buffers are implementation-defined: a function may succeed or return an error depending 
  the operation.
- No dynamic memory allocation or recursion is used.
- The API is thread-safe as long as threads do not concurrently write to the same target buffer without external synchronization.
- Supported platforms must satisfy the compile-time assertions documented in lubtype.h.

## Building And Running Tests

### Windows (MSVC)

From the tests directory:

powershell
powershell -ExecutionPolicy Bypass -File ./build_lubtype_tests.ps1
.\lubtype_tests.exe


### Linux / macOS

From the repository root:

make -C tests run

To build with gcc instead of clang:

make -C tests CC=gcc run

The checked-in test runner writes lubtype_tests_report.txt to its current working directory when executed.

## Continuous Integration

GitHub Actions runs the test suite on pushes and pull requests. If workflow files are included in your checkout, see .github/workflows/ci.yml.

# lubtype

[![CI](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml/badge.svg)](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml)

Copyright (c) 2026 paulsinclair51  
SPDX-License-Identifier: MIT  
For license details, see the LICENSE file in the project root.

## Header Files

The API is provided as three header files:

  - lubtype.h — Core API for Latin-8, Unicode-16, and byte character/string operations.
  - xlatin.h — Polymorphic macro mapping (x → l) header.
  - xunicode.h — Polymorphic macro mapping (x → u) header.

For all use cases, include `lubtype.h` and, optionally, include
`xlatin.h` or `xunicode.h` for generic code that uses the polymorphic
x macros.

### Providing Function Definitions

By default, `lubtype.h` declares `extern` functions but does not define them.
To include the function definitions in a translation unit, define
`__LUB_DEFINITIONS__` before including the header:

```c
#define __LUB_DEFINITIONS__
#include "lubtype.h"
```

Do this in exactly one translation unit per linked binary. All other
translation units should include `lubtype.h` without the define.

## Polymorphic Macro Mapping Headers

For source files that implement generic character and string logic
for both Latin and Unicode, two macro mapping headers are provided:

  - xlatin.h — Maps x to l in polymorphic macro names.
  - xunicode.h — Maps x to u in polymorphic macro names.

Mappings applied by xlatin.h:

  - `lx → ll`, `xl → ll`, `xu → lu`, `ux → ul`, `xx → ll`, `x → l`

Mappings applied by xunicode.h:

  - `lx → lu`, `xl → ul`, `xu → uu`, `ux → uu`, `xx → uu`, `x → u`

For example (`xlatin.h` / `xunicode.h`):

  - `isxalpha`  → `islalpha`  / `isualpha`
  - `xlsncpy`   → `llsncpy`   / `ulsncpy`
  - `xchar_t`   → `lchar_t`   / `uchar_t`
  - `xcstr_t`   → `lcstr_t`   / `ucstr_t`

This allows you to write a single generic source file and compile it
twice — once with `xlatin.h` and once with `xunicode.h` — to produce
both Latin and Unicode object files.

Example:

```c
#define __LUB_DEFINITIONS__
#include "lubtype.h"
#include "xlatin.h"   // or "xunicode.h"

// xchar_t is lchar_t (xlatin.h) or uchar_t (xunicode.h).
xchar_t buf[64+1];

// xlsncpy is llsncpy (xlatin.h) or ulsncpy (xunicode.h).
xlsncpy(buf, 64, (const lchar_t *)"hello", 5, NULL);
```

## Quick Start

```c
#define __LUB_DEFINITIONS__
#include "lubtype.h"

uchar_t buf[64+1]; // Buffer for up to 64 Unicode characters plus null terminator.
ulsnncpy(buf, 64, (const lchar_t *)"hello world", 11, NULL);
// buf = u"hello world"
```

## Notes

- Unicode classification and case conversion use the C runtime's
  wide-character routines (`isw*`, `tow*`), which are locale- and
  CRT-dependent. Results may differ across platforms or locale settings.

- This API does not perform Unicode normalization or surrogate pair
  handling; all operations are on individual code units.

- When a function writes to a caller-supplied target buffer `t`, the
  buffer is always null-terminated on error (at `t[tn]` for bounded
  variants or `t[0]` for unbounded copy variants), preventing buffer
  overreads.

- Overlapping source and target buffers result in
  implementation-defined behavior; a function may return an error or
  produce a correct result depending on the operation.

- Some search and replace functions may have worst-case O(n×m)
  complexity, where n is the length of the input and m is the pattern
  or map size. Most other operations are O(n).

- No dynamic memory is allocated or freed; no recursion occurs.

- The API is thread-safe provided threads do not share target buffers
  without external synchronization.

## Version Macros

```c
LUB_VERSION                     // "1.0.0"
LUB_VERSION_NUM                 // 10000  (uint32_t MMmmpp)
LUB_VERSION_HEX                 // 0x010000
LUB_VERSION_EQ(1, 0, 0)         // true if exactly 1.0.0
LUB_VERSION_AT_LEAST(1, 0, 0)   // true if >= 1.0.0
```

## API Types

| Type    | Base type  | Description                               |
|---------|------------|-------------------------------------------|
| lchar_t | uint8_t    | Latin-8 character (values 0–255)          |
| uchar_t | uint16_t   | Unicode BMP character (values 0–65535)    |
| byte_t  | uint8_t    | Raw byte (values 0x00–0xFF, no null semantics) |

String and const-string pointer types (`lchar_t *`, `const lchar_t *`,
`uchar_t *`, `const uchar_t *`, `byte_t *`, `const byte_t *`) are used
directly in function signatures; no separate `lstr_t` / `ustr_t`
typedefs are defined in `lubtype.h`.

## Polymorphic Type Mapping (xlatin.h / xunicode.h)

| Macro    | xlatin.h  | xunicode.h |
|----------|-----------|------------|
| xchar_t  | lchar_t   | uchar_t    |
| xcstr_t  | lcstr_t   | ucstr_t    |
| xstr_t   | lstr_t    | ustr_t     |

## Character and String Limits

| Constant           | Value   | Description                          |
|--------------------|---------|--------------------------------------|
| LUB_MAX_LCHAR      | 255     | Maximum Latin character value        |
| LUB_MAX_UCHAR      | 65535   | Maximum Unicode character value      |
| LUB_MAX_BYTE       | 255     | Maximum byte value                   |
| LUB_MAX_LSTRLEN    | 1000000 | Maximum Latin string length          |
| LUB_MAX_USTRLEN    | 500000  | Maximum Unicode string length        |
| LUB_MAX_UNAMELEN   | 128     | Maximum unquoted name length         |
| LUB_MAX_UQNAMELEN  | 258     | Maximum quoted name length (2×128+2) |
| LUB_MAX_LOPTLEN    | 64000   | Maximum Latin option string length   |
| LUB_MAX_UOPTLEN    | 32000   | Maximum Unicode option string length |
| LUB_MAX_BSTRLEN    | 1000000 | Maximum byte string length           |
| LUB_MAX_BOPTLEN    | 64000   | Maximum byte option string length    |

## Special Return Values

| Constant              | Value | Description                     |
|-----------------------|-------|---------------------------------|
| LUB_CMP_GREATER_THAN  | 1     | Comparison: s1 > s2             |
| LUB_CMP_EQUAL         | 0     | Comparison: s1 == s2            |
| LUB_CMP_LESS_THAN     | -1    | Comparison: s1 < s2             |
| LUB_QUOTEDNAME        | 1     | Name requires quoting           |
| LUB_UNQUOTEDNAME      | 0     | Name does not require quoting   |

Comparison functions return exactly -1, 0, or 1 (not an arbitrary
negative/positive value as with `strcmp`), making these distinct from
the error range -99 to -2.

## Error Values

Functions that write to a caller-supplied buffer return `NULL` on error.
Size-returning functions return `(size_t)error_value`. Int-returning
functions return the error value directly. All error values fall in the
reserved range -99 to -2.

| Constant          | Value | Description                          |
|-------------------|-------|--------------------------------------|
| LUB_PTR_INVALID   | -2    | Pointer argument is an error value   |
| LUB_UNTERMINATED  | -3    | Source string not null-terminated    |
| LUB_NAME_INVALID  | -4    | String is not a valid name           |
| LUB_OPT_TOO_LONG  | -5    | Option string exceeds maximum length |
| LUB_OPT_INVALID   | -6    | Option string is invalid             |
| LUB_OPT_RESERVED  | -7    | Option string uses a reserved value  |
| LUB_OVERLAP       | -8    | Source and target buffers overlap    |
| LUB_TRUNCATED     | -9    | Result was truncated                 |

## Error Classification Macros

Use the `LUB_*_ERR` macros to test or cast error values without
comparing raw integers:

```c
// Check whether any error occurred:
if (LUB_SIZE_ERR(result, 0))  { /* error */ }
if (LUB_PTR_ERR(result, 0))   { /* error */ }
if (LUB_INT_ERR(result, 0))   { /* error */ }

// Check for a specific error:
if (LUB_SIZE_ERR(result, LUB_UNTERMINATED)) { /* unterminated */ }

// Propagate an error from a called function:
if (LUB_SIZE_ERR(result, 0))
    return (uchar_t *)LUB_PTR_ERR(result, 0);
```

## Function Naming Conventions

`{}` required · `[]` optional · `<>` token · `|` alternatives

### Character and String Type Codes

| Code | Meaning                        |
|------|--------------------------------|
| l    | Latin (`lchar_t`)              |
| u    | Unicode (`uchar_t`)            |
| b    | Byte (`byte_t`)                |

### Parm Types (target ← source)

| Code | Target  | Source    |
|------|---------|-----------|
| ll   | Latin   | Latin     |
| lu   | Latin   | Unicode   |
| ul   | Unicode | Latin     |
| uu   | Unicode | Unicode   |
| lb   | Latin   | Byte      |
| ub   | Unicode | Byte      |
| bl   | Byte    | Latin     |
| bu   | Byte    | Unicode   |

Polymorphic parm types (`x`, `xl`, `xu`, `ux`, `xx`, `xb`, `bx`) map
x to l (xlatin.h) or u (xunicode.h).

### Bound Suffixes

- `n` — bounded source: explicit `sn` parameter.
- `nn` — bounded target **and** bounded source: explicit `tn` and `sn` parameters.
- No suffix — source is scanned to the null terminator (clamped to the
  type maximum); no explicit `sn` or `tn` parameter.

### Verb Flavors

| Verb suffix  | Meaning                              |
|--------------|--------------------------------------|
| *(none)*     | Case-sensitive / case-preserving     |
| `c` (lower)  | Lowercase-transform (e.g., `cpyc`)   |
| `C` (upper)  | Uppercase-transform (e.g., `CPYC`)   |
| Uppercase op | Case-insensitive (e.g., `CMP`)       |

Not all verb flavors are defined for every function. When a flavor is
absent, apply a separate case transform using `cpyc` / `CPYC` after the
operation:

```c
llstrim(t, s, 'B', ':');  // trim into t
llsCPYC(t, t);            // uppercase t in-place
```

### Function Groups and Example Names

| Group                    | Example names                                       |
|--------------------------|-----------------------------------------------------|
| Character classification | `isualpha`, `islhexdigit`, `isuname1c`              |
| Character case transform | `ultoupper`, `lutolower`, `lutocase`                |
| Hex digit → int          | `iuhexdigit`, `ilhexdigit`                          |
| String length            | `lcsnlen`, `ucsnlen`                                |
| String classification    | `isunlatinstr`, `isuRESERVED`, `isuQNAME`           |
| Compare                  | `llsncmp`, `uusnCMP`                                |
| Fixed-length compare     | `llsnfxdcmp`, `uusnFXDCMP`                          |
| Prefix compare           | `llsnpfxcmp`, `uusnPFXCMP`                          |
| Suffix compare           | `llsnsfxcmp`, `uusnSFXCMP`                          |
| Search                   | `llsnstrm`, `uusnSTRM`                              |
| Count                    | `llsncnt`, `uusnCNT`                                |
| Concatenate              | `llsnncat`, `ulsnncatc`, `ulsnnCATC`                |
| Copy                     | `llsnncpy`, `ulsnncpy`, `ulsnncpyc`                 |
| Trim                     | `llsntrim`, `uusntrim`                              |
| Pointer trim             | `llsnptrim`, `uusnptrim`                            |
| Skip                     | `llsnskip`, `uusnskip`                              |
| Reverse                  | `llsnreverse`, `uusnreverse`                        |
| Pad                      | `llsnnpad`, `uusnnpad`                              |
| Repeat                   | `llsnnrepeat`, `uusnnrepeat`                        |
| Replace                  | `llsnnreplace`, `uusnREPLACE`                       |
| Printf-style formatting  | `llsnnprintf`, `uusnnprintf`                        |

See `lubtype.h` for the complete function reference and parameter details.

MAX_XSTRLEN maps to MAX_LSTRLEN (with xlatin.h) or MAX_USTRLEN (with xunicode.h)

Bounds passed by the caller are silently clamped to these maxima.

For functions that are not explicitly bounded, strings are implicitly
bounded to these naxima.

## Usage Examples

### Append/Copy/Concatenate

#include "lubtype.h"

lchar_t buf[64+1];
lstr_t buf_p = buf;  // Buf pointer.
lstr_t buf_ap = buf; // Buf append pointer.

// Latin <- Latin case-preserving append
buf_ap = llsapp(buf_ap, (lcstr_t)"Hello");    // buf = "Hello", buf_ap = buf + 5
buf_ap = llsapp(buf_ap, (lcstr_t)" world");   // buf = "Hello world", buf_ap = buf + 10

// Latin <- Latin uppercase copy
buf_p = llsCPYC(buf_p, (lcstr_t)"Hello");      // buf = "HELLO", buf_p = buf

// Latin <- Latin lowercase concatenate
buf_p = llscatc(buf_p, (lcstr_t)" WoRlD");     // buf = "HELLO world", buf_p = buf

// Latin <- Unicode copy (unmappable chars replaced with '?')
uchar_t usrc[] = {0x0068, 0x0069, 0x0393, 0}; // Unicode string with non-Latin character
buf_p = luscpy(buf_p, usrc, '?');             // buf = "hi?", buf_p = buf

// Bounded copy (at most 3 chars + null terminator)
buf_p = llsncpy(buf_p, (lcstr_t)"hello", 3);  // buf = "", buf_p = NULL

### Trim

lchar_t out[64+1];
lstr_t out_p = out;

// Trim bounded source (n = 64) on both left and right
// (trim = 'B'), collapse internal whitespace to a single
// space (delim = ' ').
out_p = llsntrim(out_p, (lcstr_t)"  foo   bar  ", 64, 'B', ' ');
// out = "foo bar", out_p = out

// Trim unbounded source on left only (trim = 'L'),
// no collapse of intenal whitespace(delim = 0).
out_p = llstrim(out_p, (lcstr_t)"  hello  world  ", 'L', 0);
// out = "hello  world  ", out_p = out

### Pad

lchar_t out[8+1];

// Right-align in 8 chars (m > 0 → left-pad)
llsnpad(out, 8, (lcstr_t)"hi", 64, ' ', 1);
// out = "      hi"

// Left-align (m < 0 → right-pad)
llsnpad(out, 8, (lcstr_t)"hi", 64, ' ', -1);
// out = "hi      "

// Center (m == 0)
llsnpad(out, 8, (lcstr_t)"hi", 64, '-', 0);
// out = "---hi---"

### Replace

lchar_t out[64+1];

// Replace all occurrences of "foo" with "bar" (m == 0 → all)
llsnreplace(out, 64, (lcstr_t)"foo foo foo", (lcstr_t)"foo|bar", '|', 64, 0);
// out = "bar bar bar"

// Replace only the 2nd occurrence (m == 2)
llsnreplace(out, 64, (lcstr_t)"foo foo foo", (lcstr_t)"foo|bar", '|', 64, 2);
// out = "foo bar foo"

// Replace last occurrence (m == -1)
llsnreplace(out, 64, (lcstr_t)"foo foo foo", (lcstr_t)"foo|bar", '|', 64, -1);
// out = "foo foo bar"

// Multi-pair map (m must be 0; first matching needle wins)
llsnreplace(out, 64, (lcstr_t)"cat dog", (lcstr_t)"cat|kitten|dog|puppy", '|', 64, 0);
// out = "kitten puppy"

### Repeat

lchar_t out[16+1];

llsnrepeat(out, 16, (lcstr_t)"ab", 64, 4);
// out = "abababab"

// Overflow → returns NULL, out[16] = '\0'
lstr_t r = llsnrepeat(out, 16, (lcstr_t)"abc", 64, (size_t)-1);
// r == NULL

### Transliterate (sub)

lchar_t out[16+1];

// Map individual characters: 'a'→'A', 'b'→'B'
llsnsub(out, (lcstr_t)"abc", (lcstr_t)"ab", (lcstr_t)"AB", 64, 64, 64);
// out = "ABc"

// from/to length mismatch → returns NULL
lstr_t r = llsnsub(out, (lcstr_t)"abc", (lcstr_t)"ab", (lcstr_t)"X", 64, 64, 64);
// r == NULL

lchar_t buf[64+1];

## Building and Running Tests

### Tests

Windows (MSVC)

1. Open a Developer Command Prompt for VS 2022 (or equivalent MSVC environment).
2. Build the test suite using the provided PowerShell script:

     powershell -ExecutionPolicy Bypass -File ./build_lubtype_tests.ps1

3. Run the test executable:
     md
     .\tests\lubtype_tests.exe

   This will generate a human-readable test report at tests/lubtype_tests_report.txt.

  Linux / macOS

    gcc -std=c11 -Wall -Wextra -Werror -o lubtype_tests \
      tests/lubtype_tests.c tests/test_reserved_matrix.c tests/test_search_families.c \
      tests/test_span_count.c tests/test_core_families.c tests/test_type_matrix.c tests/test_utilities.c
    ./lubtype_tests

Requires Visual Studio Build Tools 2022 (or any VS 2022 installation) on Windows.

#### Test Report

After running the test suite on Windows, a summary report is written to:

  tests/lubtype_tests_report.txt

This report includes the date/time, a list of test modules executed, and a completion message. If any assertion fails, the test executable will abort and the report may be incomplete.

## Continuous Integration (CI)

GitHub Actions runs the test suite on every push and pull request:

- Linux - gcc and clang, -Wall -Wextra -Werror
- Windows - MSVC, /W4 /WX

See [.github/workflows/ci.yml](.github/workflows/ci.yml).

## Thread Safety

All functions are stateless (no global mutable state). Concurrent calls on
different buffers are safe. Concurrent writes to the same buffer require
external synchronisation.

# lubtype

[![CI](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml/badge.svg)](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml)

Copyright (c) 2026 paulsinclair51  
SPDX-License-Identifier: MIT  
For license details, see the LICENSE file in the project root.

## Header Files

The API is provided as three header files:

  - lubtype.h — Core API for Latin and Unicode character/string operations.
  - xlatin.h — Polymorphic macro mapping (x -> l) header.
  - xunicode.h — Polymorphic macro mapping (x -> u) header.

For all use cases, include "lubtype.h" and, optionally, include
"xlatin.h" or "include "xunicode.h" for generic code that uses
the polymorphic x macros.

See these *.h files for details beyond the overview provided below.

## Polymorphic Macro Mapping Headers

For *.c files that provide generic character and string logic for
both Latin and Unicode, two macro mapping headers are provided:

  - xlatin.h — Maps x to l in polymorphic macro names.
  - xunicode.h — Maps x to u in polymorphic macro names.

For example:

  - isxalpha -> islalpha (xlatin.h) or isualpha (xunicode.h)
  - xlsncpy -> llsncpy (xlatin.h) or ulsncpy (xunicode.h)
  - uxscpy -> ulscpy (xlatin.h) or uuscpy (xunicode.h)
  - xstr_t -> lstr_t (xlatin.h) or ustr_t (xunicode.h)

This allows you to write code using the x macros and the lubtype.h API
and compile it twice (once with xlatin.h and once with xunicode.h)
to generate both a Latin and an Unicode *.obj file from a single generic
source.

Example:

#include "lubtype.h"
#include "xlatin.h"   // or "xunicode.h"

// xstr_t defined to lstr_t (xlatin.h) or ustr_t (xunicode.h)
xstr_t buf[64+1];

// xlsncpy defined to llsncpy (xlatin.h) or ulsncpy (xunicode.h).
xlsncpy(buf, (lcstr_t)"hello", 5);
   
## Quick Start

#include "lubtype.h"

uchar_t buf[64+1]; // Buffer for up to 64 characters plus a null terminator.
ulscpy(buf, (lcstr_t)"hello world");
// buf = "hello world"

## Limitations & Notes

- Unicode classification and case conversion use the C runtime's
  wide-character routines (isw*, tow*), which are locale- and
  CRT-dependent. Results may differ across platforms or locale settings.

- This API does not perform Unicode normalization or surrogate pair
  handling; all operations are on individual code units.
- For functions that return NULL on error, the output buffer is
  always null-terminated on error if possible, to help prevent
  buffer overreads.

- Some search and replace functions (e.g., substring search, multi-pair
  replace) may have worst-case O(n*m) complexity, where n is the length
  of the input and m is the pattern or map size. Most other operations
  are O(n).

## Version Macros

LUB_VERSION                    // E.g., "1.0.0"
LUB_VERSION_NUM                // E.g., 10000
LUB_VERSION_AT_LEAST(1, 0, 0)  // E.g., true

## API Types

| Type     | Alias for       | Description              |
|----------|-----------------|--------------------------|
| lchar_t  | uint8_t         | Latin character          |
| uchar_t  | uint16_t        | Unicode character        |
| lstr_t   | lchar_t *       | Mutable Latin string     |
| ustr_t   | uchar_t *       | Mutable Unicode string   |
| lcstr_t  | const lchar_t * | Read-only Latin string   |
| ucstr_t  | const uchar_t * | Read-only Unicode string |

## Polymorphic Type Mapping

| Type     | xlatin.h  | xunicode.h |
|----------|-----------|------------|
| xchar_t  | lchar_t   | uchar_t	|
| xstr_t   | lstr_t    | ustr_t     |
| xcstr_t  | lcstr_t   | uccstr_t   |

## Function Naming Conventions

{} required. [] optional. <> token. | alternatives.

1. Single-source predicates and character transforms follow this pattern:

  {is|to}{l|u}<kind>

Note: islname1c, islnamec, islreserved, and islqname are not
supported since names are always Unicode.

Examples: isualpha, islhexdigits, isureserved, tolupper

2. int <- character transforms follow this patten:

  i{l|u}hexdigit

Example: iuhexdigit

3. String length follow this pattern:

  {l|u}cs[n]len

4. Transform/compare/search follow this pattern:

  <parmtype>s[n]<verbs>[qname]

Examples: lusncpy, ulsapp, uusCMP

Notes: cs and s indicate string operation.
       For n, see Source and Target Bounds below.

### Parm Types

- ll - Latin target <- Latin source or Latin source 1 and 2
- lu - Latin target <- Unicode source or Latin source 1 and Unicode source 2
- ul - Unicode target <- Latin source or Unicode source 1 and Latin source 2
- uu - Unicode target <- Unicode source or Unicode source 1 and 2
- lb - Latin target <- Byte source
- ub - Unicode target <- Byte source
- bl - Byte target <- Latin source
- bu - Byte target <- Unicode source

### Polymorphic Mapping to Parm Types

Polymorphic parm types:

- x, xl, xu, ux, xx, xb, bx

x is mapped to l if xlatin.h is included and to u if xunicode.h is included.

### Source and Target Bounds

- n in the name means explicit bounded source length is provided and function
  includes a parameter n.
- Some APIs also take a parameter tn for bounded target capacity.
- If n is omitted, source is scanned to null terminator (with internal max clamp)
  and n parameter is also omitted.

### Verbs

- Case-sensitive match/compare   - lowercase verbs (e.g., cmp)
- Case-insensitive match/compare - uppercase verbs (e.g., CMP)
- Case-preserving transform      - lowercase verbs (e.g., app, cpy, cat)
- Lowercase-transform            - lowercase verb with c suffix (e.g., appc, cpyc, catc)
- Uppercase-transform            - uppercase verb with C suffix (e.g., APPC, CPYC, CATC)

Note: Not all lowercase-transform and uppercase-transform verbs are supported. For
example, trim (case-preserving) is supported but trimc and TRIMC are not supported. For these cases,
the case can be transformed in the target using cpyc or CPYC function after the
transform. For example,

llstrim(t, s, 'B', ':');  // trim s into target.
llsCPYC(t, t); // Uppercase target.

Examples:

- llsncpy  - Latin <- Latin bounded case-preserving string copy
- lusncatc - Latin <- Unicode bounded string concatenation with lowercase mapping
- uusnCMP  - Unicode to Unicode case-insensitive bounded string comparison

## Errors

Functions that write to a caller-supplied buffer return NULL on error.

lcslen/ucslen return MISSING_STR_TERMINATOR ((size_t)-1) when no null terminator
is found at or before s[n] (i.e., within the first n+1 characters, from s[0] to s[n]). 

On error, the output buffer is always null-terminated by at least t[tn] (bounded variants)
or t[0] (unbounded copy variants).

## Limits

| Constant     | Value   |
|--------------|---------|
| MAX_LSTRLEN  | 1000000 |
| MAX_USTRLEN  |  500000 |

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

# lubtype

[![CI](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml/badge.svg)](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml)

Copyright (c) 2026 paulsinclair51  
SPDX-License-Identifier: MIT  
For license details, see the LICENSE file in the project root.

`lubtype` is a header-only C/C++ API for Latin-8 (`lchar_t`), Unicode-16 (`uchar_t`), and raw byte (`byte_t`) character and string operations.

This README reflects the current headers in this repository:

- `lubtype.h` - core API and version macros.
- `xlatin.h` - polymorphic `x...` macro remapping to Latin-backed operations.
- `xunicode.h` - polymorphic `x...` macro remapping to Unicode-backed operations.

The current documented API version is `1.0.0` (`LUB_VERSION` in `lubtype.h`).

## Header Layout

Include `lubtype.h` everywhere you use the API.

```c
#include "lubtype.h"
```

`lubtype.h` is header-only, but externally visible function bodies are emitted only when `__LUB_DEFINITIONS__` is defined before the include. Define it in exactly one translation unit per final binary:

```c
#define __LUB_DEFINITIONS__
#include "lubtype.h"
```

All other translation units should include `lubtype.h` without that define.

## What The API Covers

The current `lubtype.h` surface includes:

- Character classification and case transformation.
- Hex-digit conversion helpers.
- Bounded string-length validation.
- String classification helpers.
- Compare, fixed-length compare, prefix compare, and suffix compare.
- Search and count helpers.
- Concatenate and copy families.
- Trim, pointer-trim, and skip helpers.
- Reverse, pad, repeat, and replace families.
- Latin formatting helpers (`llsnprintf`, `llsnvprintf`).

Operations are provided in Latin/Unicode/byte combinations where they are meaningful. Function names encode the target/source family, bounds, and case behavior.

## Polymorphic Mapping Headers

`xlatin.h` and `xunicode.h` remap current `x...` macros onto existing `lubtype.h` symbols so one source file can be built twice: once for Latin and once for Unicode.

Current remapped families include:

- Limits: `LUB_MAX_XCHAR`, `LUB_MAX_XSTRLEN`
- Character classification: `isxalpha`, `isxdigit`, `isxspace`, and related predicates
- Hex-digit conversion: `ixhexdigit`
- String length: `xcsnlen`
- Case transforms: `xltocase`, `xltolower`, `xltoupper`, `xxtocase`, `xxtolower`, `xxtoupper`
- Compare/search/count: `xlsncmp`, `xxsncmp`, `xlsnstrm`, `xxsnstrm`, `xlsncnt`, `xxsncnt`, plus the case-insensitive and prefix/suffix variants
- Concatenate/copy: `xlsnncat`, `xxsnncat`, `xlsnncpy`, `xxsnncpy`, plus lowercase/uppercase/quoted variants
- Replace: `xlsnnREPLACE`, `xxsnnREPLACE`, and in `xunicode.h` also `xxsnnreplace`
- Pad/repeat/reverse: `xlsnnpad`, `xxsnnpad`, `xlsnnrepeat`, `xxsnnrepeat`, `xlsnnreverse`, `xxsnnreverse`
- Latin-only formatting via `xlatin.h`: `xlsnprintf`, `xlsnvprintf`

These mapping headers do not define stale `xchar_t`, `xstr_t`, or `xcstr_t` aliases.

## Quick Start

```c
#define __LUB_DEFINITIONS__
#include "lubtype.h"

int main(void) {
    lchar_t src[] = { 'h', 'e', 'l', 'l', 'o', 0 };
    uchar_t dst[16];

    if (!ulsnncpy(dst, 15, src, 5, NULL)) {
        return 1;
    }

    return 0;
}
```

## Verified Examples

```c
/* Search for the second occurrence of "two". */
const lchar_t haystack[] = { 'o','n','e',' ','t','w','o',' ','t','w','o',0 };
const lchar_t needle[] = { 't','w','o',0 };
const lchar_t *match = llsnstrm(haystack, needle, 32, 0, 2);
```

```c
/* Trim both ends and collapse runs of whitespace to a single space. */
lchar_t out[16 + 1];
const lchar_t spaced[] = { ' ',' ','a','b','c',' ',' ',0 };
llsnntrim(out, 16, spaced, 16, 0, NULL, 'B', ' ');
```

```c
/* Replace the second occurrence of "cat" with "dog". */
lchar_t out[32 + 1];
const lchar_t src[] = { 'c','a','t',' ','a','n','d',' ','c','a','t',0 };
const lchar_t map[] = { 'c','a','t','|','d','o','g',0 };
llsnnreplace(out, 32, src, 32, map, '|', 2);
```

For the full API reference and additional examples, see the docblocks in `lubtype.h`.

## Version Macros

```c
LUB_VERSION                     /* "1.0.0" */
LUB_VERSION_NUM                 /* 10000 */
LUB_VERSION_HEX                 /* 0x010000 */
LUB_VERSION_EQ(1, 0, 0)         /* true if exactly 1.0.0 */
LUB_VERSION_AT_LEAST(1, 0, 0)   /* true if >= 1.0.0 */
```

## Core Types And Limits

| Type    | Base type | Notes |
|---------|-----------|-------|
| `lchar_t` | `uint8_t`  | Latin-8 code unit |
| `uchar_t` | `uint16_t` | Unicode BMP code unit |
| `byte_t`  | `uint8_t`  | Raw byte, no null-string semantics |

Important limits from `lubtype.h`:

| Macro | Value |
|-------|-------|
| `LUB_MAX_LCHAR` | `255` |
| `LUB_MAX_UCHAR` | `65535` |
| `LUB_MAX_BYTE` | `255` |
| `LUB_MAX_LSTRLEN` | `1000000` |
| `LUB_MAX_USTRLEN` | `500000` |
| `LUB_MAX_UNAMELEN` | `128` |
| `LUB_MAX_UQNAMELEN` | `258` |
| `LUB_MAX_LOPTLEN` | `64000` |
| `LUB_MAX_UOPTLEN` | `32000` |
| `LUB_MAX_BSTRLEN` | `1000000` |
| `LUB_MAX_BOPTLEN` | `64000` |

Caller-supplied bounds are clamped to the corresponding maximums.

## Return Values And Error Handling

Comparison functions return exactly:

- `LUB_CMP_LESS_THAN` = `-1`
- `LUB_CMP_EQUAL` = `0`
- `LUB_CMP_GREATER_THAN` = `1`

Name classification helpers also use:

- `LUB_UNQUOTEDNAME` = `0`
- `LUB_QUOTEDNAME` = `1`

Reserved error values occupy the range `-99` to `-2` and are exposed through macros such as:

- `LUB_PTR_INVALID`
- `LUB_UNTERMINATED`
- `LUB_NAME_INVALID`
- `LUB_OPT_TOO_LONG`
- `LUB_OPT_INVALID`
- `LUB_OPT_RESERVED`
- `LUB_OVERLAP`
- `LUB_TRUNCATED`

Use the error helpers instead of comparing raw casted values:

```c
size_t len = lcsnlen(src, 32);
if (LUB_SIZE_ERR(len, 0)) {
    /* handle error */
}
```

Pointer-returning functions return `NULL` on ordinary failure and may also encode reserved error values through the `LUB_PTR_ERR` helpers.

## Naming Conventions

The naming grammar in `lubtype.h` is still the authoritative reference. In short:

- `l`, `u`, and `b` identify Latin, Unicode, and byte families.
- `ll`, `lu`, `ul`, `uu`, `lb`, `ub`, `bl`, and `bu` identify target/source combinations.
- `n` means a bounded source argument is present.
- `nn` means both target size and source bound are explicit.
- Lowercase operation suffixes like `cpyc` are lowercase-transform variants.
- Uppercase operation suffixes like `CMP` and `REPLACE` are case-insensitive variants.

Examples:

- `llsncmp` - Latin/Latin bounded compare, case-sensitive
- `ulsnCMP` - Unicode/Latin bounded compare, case-insensitive
- `llsnncpy` - Latin target, Latin source, explicit target and source bounds
- `uusnnREPLACE` - Unicode target/source replace, explicit bounds, case-insensitive

## Notes

- Unicode classification and case conversion use the C runtime's `isw*` and `tow*` routines, so results are locale- and CRT-dependent.
- The API operates on individual code units only. It does not perform Unicode normalization or surrogate-pair handling.
- When a function writes into a caller-supplied target buffer, the target is null-terminated on error.
- Overlapping source and target buffers are implementation-defined: a function may succeed or return an error depending on the operation.
- No dynamic memory allocation or recursion is used.
- The API is thread-safe as long as threads do not concurrently write to the same target buffer without external synchronization.
- Supported platforms must satisfy the compile-time assertions in `lubtype.h`, including the documented pointer/integer error-value compatibility assumptions.

## Building And Running Tests

### Windows (MSVC)

From the `tests` directory:

```powershell
powershell -ExecutionPolicy Bypass -File ./build_lubtype_tests.ps1
.\lubtype_tests.exe
```

### Linux / macOS

No dedicated Linux/macOS build script is currently checked in at the repository root. Use the CI workflow as the source of truth for cross-platform coverage, or mirror the Windows test-runner structure with your local toolchain if you need an ad hoc build.

The checked-in test runner writes `lubtype_tests_report.txt` to its current working directory when executed.

## Continuous Integration

GitHub Actions runs the test suite on pushes and pull requests. See `.github/workflows/ci.yml`.

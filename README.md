# lubtype

[![CI](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml/badge.svg)](https://github.com/paulsinclair51/lubtype/actions/workflows/ci.yml)

Single-header C11 library for Latin (`uint8_t`) and Unicode (`uint16_t`) string operations.
Drop `lubtype.h` into your project — no build step required.

## Types

| Type       | Alias for              | Description              |
|------------|------------------------|--------------------------|
| `lchar_t`  | `uint8_t`              | Latin character          |
| `uchar_t`  | `uint16_t`             | Unicode character        |
| `lstr_t`   | `lchar_t *`            | Mutable Latin string     |
| `ustr_t`   | `uchar_t *`            | Mutable Unicode string   |
| `lcstr_t`  | `const lchar_t *`      | Read-only Latin string   |
| `ucstr_t`  | `const uchar_t *`      | Read-only Unicode string |

## Naming convention

```
[ll | lu | ul | uu] s [n] verb
 ^^                   ^   ^^^^
 source→target        n=bounded   e.g. cpy, cat, trim, pad, replace, repeat, sub
```

- `ll` — Latin → Latin
- `lu` — Unicode source → Latin target
- `ul` — Latin source → Unicode target
- `uu` — Unicode → Unicode
- `n` present — caller supplies a bound `n` (input) and/or `tn` (output); omitting `n` means null-terminated only
- Uppercase verb suffix (e.g. `CPY`, `APPC`) — case-insensitive variant

## Error sentinel

Functions that write to a caller-supplied buffer return `NULL` on error.  
`lcslen` / `ucslen` return `MISSING_STR_TERMINATOR` (`(size_t)-1`) when no null terminator
is found within the bound.  
On error the output buffer is always null-terminated at `t[tn]` (bounded variants)
or `t[0]` (unbounded copy variants).

## Limits

| Constant       | Value     |
|----------------|-----------|
| `MAX_LSTRLEN`  | 1 000 000 |
| `MAX_USTRLEN`  |   500 000 |

Bounds passed by the caller are silently clamped to these maxima.

## Usage examples

### Copy / append

```c
#include "lubtype.h"

lchar_t buf[64];

// Latin → Latin copy
llscpy(buf, (lcstr_t)"hello");          // buf = "hello"

// Bounded copy (at most 3 chars + NUL)
llsncpy(buf, (lcstr_t)"hello", 3);      // buf = "hel"

// Append
llscat(buf, (lcstr_t)" world");         // buf = "hel world"

// Unicode → Latin copy (unmappable chars replaced with '?')
uchar_t usrc[] = {0x68, 0x69, 0};      // u"hi"
luscpy(buf, usrc, '?');                 // buf = "hi"
```

### Trim

```c
lchar_t out[64];

// Trim both sides, collapse internal whitespace to a single space
llsntrim(out, (lcstr_t)"  foo   bar  ", 64, 'B', ' ');
// out = "foo bar"

// Trim left only, no collapse (delim = 0)
llsntrim(out, (lcstr_t)"  hello", 64, 'L', 0);
// out = "hello"
```

`trim` flag: `'L'` left, `'R'` right, `'B'` both.

### Pad

```c
lchar_t out[8];

// Right-align in 8 chars (m > 0 → left-pad)
llsnpad(out, 8, (lcstr_t)"hi", 64, ' ', 1);
// out = "      hi"

// Left-align (m < 0 → right-pad)
llsnpad(out, 8, (lcstr_t)"hi", 64, ' ', -1);
// out = "hi      "

// Center (m == 0)
llsnpad(out, 8, (lcstr_t)"hi", 64, '-', 0);
// out = "---hi---"
```

### Replace

```c
lchar_t out[64];

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
```

### Repeat

```c
lchar_t out[16];

llsnrepeat(out, 15, (lcstr_t)"ab", 64, 4);
// out = "abababab"

// Overflow → returns NULL, out[15] = '\0'
lstr_t r = llsnrepeat(out, 15, (lcstr_t)"abc", 64, (size_t)-1);
// r == NULL
```

### Transliterate (sub)

```c
lchar_t out[16];

// Map individual characters: 'a'→'A', 'b'→'B'
llsnsub(out, (lcstr_t)"abc", (lcstr_t)"ab", (lcstr_t)"AB", 64, 64, 64);
// out = "ABc"

// from/to length mismatch → returns NULL
lstr_t r = llsnsub(out, (lcstr_t)"abc", (lcstr_t)"ab", (lcstr_t)"X", 64, 64, 64);
// r == NULL
```

## Version macros

```c
LUB_VERSION          // "1.0.0"
LUB_VERSION_NUM      // 10000
LUB_VERSION_AT_LEAST(1, 0, 0)  // true
```

## Building and running tests

**Windows (MSVC)**
```cmd
build_msvc_regression.cmd
```
Requires Visual Studio Build Tools 2022 (or any VS 2022 installation).

**Linux / macOS**
```sh
gcc -std=c11 -Wall -Wextra -Werror -o lubtype_regression_tests lubtype_regression_tests.c
./lubtype_regression_tests
```

## CI

GitHub Actions runs the test suite on every push and pull request:

- **Linux** — gcc and clang, `-Wall -Wextra -Werror`
- **Windows** — MSVC, `/W4 /WX`

See [.github/workflows/ci.yml](.github/workflows/ci.yml).

## Thread safety

All functions are stateless (no global mutable state). Concurrent calls on
**different** buffers are safe. Concurrent writes to the **same** buffer require
external synchronisation.

## License

MIT License

Copyright (c) 2026 paulsinclair51

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

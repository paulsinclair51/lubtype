# Packaging Notes for lubtype

Copyright (c) 2026 paulsinclair51
SPDX-License-Identifier: MIT

## Minimal Distribution
- Only lubtype.h, xlatin.h, xunicode.h, README.md, and LICENSE are required for end users.
- Tests and CI scripts can be included in a “full” or “dev” package.

## Header-Only
- lubtype.h, xlatin.h, and xunicode.h are designed for direct inclusion—no build step required.
- Document required defines/macros in the README.
- Document that one translation unit must define __LUB_DEFINITIONS__ before including lubtype.h.
- Ensure xlatin.h and xunicode.h only expose x-mappings for symbols that exist in lubtype.h (no stale aliases).
- Ensure removed aliases are not reintroduced (for example, xstr_t/xcstr_t).

## Versioning
- Use semantic versioning (MAJOR.MINOR.PATCH).
- Update version macros in lubtype.h and reflect in README/changelog.

## Testing
- Provide instructions in the README for running tests against the current tests/ layout and API.
- Ensure README examples compile against current function names/signatures.

## CI/CD
- Ensure CI runs all tests on all supported platforms before release.

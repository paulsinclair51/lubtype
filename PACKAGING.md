# Packaging Notes for lubtype

Copyright (c) 2026 paulsinclair51
SPDX-License-Identifier: MIT

## Minimal Distribution
- Only lubtype.h, xlatin.h, xunicode.h, README.md, and LICENSE are required for end users.
- Tests and CI scripts can be included in a “full” or “dev” package.

## Header-Only
- lubtype.h, xlatin.h, and xunicode.h are designed for direct inclusion—no build step required.
- Document any required defines or configuration macros in the README.

## Versioning
- Use semantic versioning (MAJOR.MINOR.PATCH).
- Update version macros in lubtype.h and reflect in README/changelog.

## Testing
- Provide instructions in the README for running tests (e.g., `gcc -o test_runner tests/lubtype_tests.c && ./test_runner`).

## CI/CD
- Ensure CI runs all tests on all supported platforms before release.

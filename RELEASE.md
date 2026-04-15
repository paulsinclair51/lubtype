# Release Checklist for lubtype

Copyright (c) 2026 paulsinclair51
SPDX-License-Identifier: MIT
For license details, see the LICENSE file in the project root.

## Code Quality & Documentation
- [ ] All public APIs are documented with usage, parameters, return values, and error conditions.
- [ ] All function families and variants are implemented and reviewed.
- [ ] Top-level documentation (README, header docblocks) is up to date and clear.
- [ ] Version number and changelog are updated.

## Testing
- [ ] All tests pass on all supported platforms and compilers.
- [ ] Edge cases, error paths, and overlapping buffer scenarios are tested.
- [ ] Fuzz/randomized tests are included and pass.
- [ ] No memory leaks or undefined behavior (use tools like ASan/Valgrind if possible).

## Packaging
- [ ] Header files (lubtype.h, xlatin.h, xunicode.h) are self-contained and ready for drop-in use.
- [ ] All test files are in a dedicated tests/ directory.
- [ ] README.md includes usage, API overview, and examples.
- [ ] License file is present and correct.
- [ ] Copyright headers (if required) are present.

## Distribution
- [ ] Tag the release in version control (e.g., git tag v1.0.0).
- [ ] Prepare a release archive (zip/tar.gz) with:
  - lubtype.h
  - xlatin.h
  - xunicode.h
  - README.md
  - LICENSE
  - tests/ (optional, for users who want to run tests)
- [ ] (Optional) Publish to package managers or upload to GitHub Releases.

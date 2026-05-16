/**
 * @file test_charclass.c
 * @brief X-macro tests for character classification functions in lubtype.h.
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 *       Each test file needs `static lub_test_result_t test_result;`.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
#define LUB_X_IS_L
#endif

#include <assert.h>
#include <stdio.h>

#include "lubtype_test_declarations.h"
#include "../lubtype.h"

static lub_test_result_t test_result;

/**
 * @brief Run tests for x-macro character classification and
 *        conversion functions.
 *
 * Tests isxalpha, isxupper, isxlower, isxdigit, isxalnum, xxtoupper, xxtolower.
 * Also checks edge cases for null and non-alphabetic input.
 * This file is compiled twice: once with -DLUB_X_IS_L and once
 * with -DLUB_X_IS_U.
 */

lub_test_result_t LUB_PASTE(run_charclass_tests_, LUB_X)(void)
{ test_result = (lub_test_result_t){0};
// Test: isxalpha
  LUB_ASSERT(isxalpha((xchar_t)'A'));
  LUB_ASSERT(isxalpha((xchar_t)'z'));
  LUB_ASSERT(!isxalpha((xchar_t)'1'));
  // Test: isxupper
  LUB_ASSERT(isxupper((xchar_t)'Z'));
  LUB_ASSERT(!isxupper((xchar_t)'a'));
  // Test: isxlower
  LUB_ASSERT(isxlower((xchar_t)'a'));
  LUB_ASSERT(!isxlower((xchar_t)'Z'));
  // Test: isxdigit
  LUB_ASSERT(isxdigit((xchar_t)'5'));
  LUB_ASSERT(!isxdigit((xchar_t)'A'));
  // Test: isxalnum
  LUB_ASSERT(isxalnum((xchar_t)'9'));
  LUB_ASSERT(isxalnum((xchar_t)'B'));
  LUB_ASSERT(!isxalnum((xchar_t)'!'));
  // Test: xxtoupper, xxtolower
  LUB_ASSERT(xxtoupper((xchar_t)'a') == (xchar_t)'A');
  LUB_ASSERT(xxtolower((xchar_t)'Z') == (xchar_t)'z');
  // Null/zero edge cases
  LUB_ASSERT(!isxalpha((xchar_t)0));
  LUB_ASSERT(!isxupper((xchar_t)0));
  LUB_ASSERT(!isxlower((xchar_t)0));
  LUB_ASSERT(!isxdigit((xchar_t)0));
  LUB_ASSERT(!isxalnum((xchar_t)0));
  LUB_ASSERT(xxtoupper((xchar_t)0) == (xchar_t)0);
  LUB_ASSERT(xxtolower((xchar_t)0) == (xchar_t)0);
  // Non-alphabetic character passthrough
  LUB_ASSERT(xxtoupper((xchar_t)'!') == (xchar_t)'!');
  LUB_ASSERT(xxtolower((xchar_t)'!') == (xchar_t)'!');

  printf("Character classification/conversion tests passed for LUB_X=%s.\n",
         LUB_STRINGIFY(LUB_X));
  return test_result;
}

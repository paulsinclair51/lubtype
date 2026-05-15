/**
 * @file test_charclass.c
 * @brief Polymorphic (x-macro) tests for character classification functions in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
#error "Compile with -DLUB_X_IS_L or -DLUB_X_IS_U"
#endif

#include <assert.h>
#include "../lubtype.h"
#include "test_declarations.h"
#include <stdio.h>

/* Define xchar_t as lchar_t or uchar_t based on the compile-time define */
#if defined(LUB_X_IS_L)
typedef lchar_t xchar_t;
#define LUB_X_NAME "l"
#define RUN_XCHARCLASS run_lcharclass_tests
#else
typedef uchar_t xchar_t;
#define LUB_X_NAME "u"
#define RUN_XCHARCLASS run_ucharclass_tests
#endif

/**
 * @brief Run tests for x-macro character classification and conversion functions.
 *
 * Tests isxalpha, isxupper, isxlower, isxdigit, isxalnum, xxtoupper, xxtolower.
 * Also checks edge cases for null and non-alphabetic input.
 * This file is compiled twice: once with -DLUB_X_IS_L and once with -DLUB_X_IS_U.
 */
void RUN_XCHARCLASS(void) {
    /* Test: isxalpha */
    assert(isxalpha((xchar_t)'A'));
    assert(isxalpha((xchar_t)'z'));
    assert(!isxalpha((xchar_t)'1'));
    /* Test: isxupper */
    assert(isxupper((xchar_t)'Z'));
    assert(!isxupper((xchar_t)'a'));
    /* Test: isxlower */
    assert(isxlower((xchar_t)'a'));
    assert(!isxlower((xchar_t)'Z'));
    /* Test: isxdigit */
    assert(isxdigit((xchar_t)'5'));
    assert(!isxdigit((xchar_t)'A'));
    /* Test: isxalnum */
    assert(isxalnum((xchar_t)'9'));
    assert(isxalnum((xchar_t)'B'));
    assert(!isxalnum((xchar_t)'!'));
    /* Test: xxtoupper, xxtolower */
    assert(xxtoupper((xchar_t)'a') == (xchar_t)'A');
    assert(xxtolower((xchar_t)'Z') == (xchar_t)'z');
    /* Null/zero edge cases */
    assert(!isxalpha((xchar_t)0));
    assert(!isxupper((xchar_t)0));
    assert(!isxlower((xchar_t)0));
    assert(!isxdigit((xchar_t)0));
    assert(!isxalnum((xchar_t)0));
    assert(xxtoupper((xchar_t)0) == (xchar_t)0);
    assert(xxtolower((xchar_t)0) == (xchar_t)0);
    /* Non-alphabetic character passthrough */
    assert(xxtoupper((xchar_t)'!') == (xchar_t)'!');
    assert(xxtolower((xchar_t)'!') == (xchar_t)'!');
    printf("x-macro character classification/conversion tests passed for LUB_X=%s.\n",
           LUB_X_NAME);
}

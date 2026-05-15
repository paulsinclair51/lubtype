/**
 * @file test_cmp_search.c
 * @brief Tests for compare and search functions in lubtype.h.
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

static void test_search_delim_negative_m(void) {
    const xchar_t hay[] = {'a','b','a','b','a',0};
    const xchar_t needle[] = {'a','b','a',0};
    assert(xlsnstrm(hay, 16, needle, '|', -1) == hay + 2);
    assert(xlsnSTRM(hay, 16, needle, '|', -1) == hay + 2);
}

static void test_search_charset_negative_m(void) {
    const xchar_t hay[] = {'a','b','a','c','a','d',0};
    const xchar_t set[] = {'b','d',0};
    assert(xlsnstrm(hay, 16, set, 0, -1) == hay + 5);
    assert(xlsnSTRM(hay, 16, set, 0, -1) == hay + 5);
}

static void test_search_case_insensitive(void) {
    const xchar_t hay[] = {'a','B','a','b','A',0};
    const xchar_t needle[] = {'A','B','A',0};
    assert(xlsnSTRM(hay, 16, needle, '|', -1) == hay + 2);
}

static void test_search_empty_token(void) {
    const xchar_t empty[] = {0};
    const xchar_t one[] = {'a',0};
    const xchar_t one_delim[] = {'|',0};
    assert(xlsnstrm(empty, 4, one_delim, '|', 1) == empty);
    assert(xlsnstrm((const xchar_t *)"a", 4, empty, 0, 1) == NULL);
    assert(xlsnstrm(one, 4, empty, 0, 1) == NULL);
}

/**
 * @brief Run tests for compare and search functions.
 *
 * Tests current compare and string-search/count helpers.
 * Each block checks a specific function or family for
 * correct behavior and edge cases.
 */
void LUB_PASTE(LUB_PASTE(run_, LUB_X), cmp_search_tests)(void) {
    xchar_t s1[16] = {'a','b','c','\0'};
    xchar_t s2[16] = {'a','b','c','\0'};
    xchar_t s3[16] = {'a','b','d','\0'};

    // Compare.
    assert(xlsnncmp(s1, 3, s2, 3) == 0);
    assert(xlsnncmp(s1, 3, s3, 3) < 0);

    // Search/count.
    test_search_delim_negative_m();
    test_search_charset_negative_m();
    test_search_case_insensitive();
    test_search_empty_token();
    assert(xlsncnt((const xchar_t *)"abcabc", 6, (const xchar_t *)"ab", '|') == 2);

    printf("x-macro comparison/search tests passed for LUB_X=%s.\n",
           LUB_STRINGIFY(LUB_X));
}

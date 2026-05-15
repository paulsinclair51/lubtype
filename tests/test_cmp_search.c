/**
 * @file test_cmp_search.c
 * @brief Tests for compare and search functions in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include <stdio.h>

static void test_search_delim_negative_m_all_variants(void) {
    const lchar_t lhay[] = {'a','b','a','b','a',0};
    const uchar_t uhay[] = {'a','b','a','b','a',0};
    const lchar_t lneedle[] = {'a','b','a',0};
    const uchar_t uneedle[] = {'a','b','a',0};

    assert(llsnstrm(lhay, lneedle, 16, '|', -1) == lhay + 2);
    assert(lusnstrm(lhay, uneedle, 16, '|', -1) == lhay + 2);
    assert(ulsnstrm(uhay, lneedle, 16, '|', -1) == uhay + 2);
    assert(uusnstrm(uhay, uneedle, 16, '|', -1) == uhay + 2);

    assert(llsnSTRM(lhay, lneedle, 16, '|', -1) == lhay + 2);
    assert(lusnSTRM(lhay, uneedle, 16, '|', -1) == lhay + 2);
    assert(ulsnSTRM(uhay, lneedle, 16, '|', -1) == uhay + 2);
    assert(uusnSTRM(uhay, uneedle, 16, '|', -1) == uhay + 2);
}

static void test_search_charset_negative_m_all_variants(void) {
    const lchar_t lhay[] = {'a','b','a','c','a','d',0};
    const uchar_t uhay[] = {'a','b','a','c','a','d',0};
    const lchar_t lset[] = {'b','d',0};
    const uchar_t uset[] = {'b','d',0};

    assert(llsnstrm(lhay, lset, 16, 0, -1) == lhay + 5);
    assert(lusnstrm(lhay, uset, 16, 0, -1) == lhay + 5);
    assert(ulsnstrm(uhay, lset, 16, 0, -1) == uhay + 5);
    assert(uusnstrm(uhay, uset, 16, 0, -1) == uhay + 5);

    assert(llsnSTRM(lhay, lset, 16, 0, -1) == lhay + 5);
    assert(lusnSTRM(lhay, uset, 16, 0, -1) == lhay + 5);
    assert(ulsnSTRM(uhay, lset, 16, 0, -1) == uhay + 5);
    assert(uusnSTRM(uhay, uset, 16, 0, -1) == uhay + 5);
}

static void test_search_case_insensitive_variants(void) {
    const lchar_t lhay[] = {'a','B','a','b','A',0};
    const uchar_t uhay[] = {'a','B','a','b','A',0};
    const lchar_t lneedle[] = {'A','B','A',0};
    const uchar_t uneedle[] = {'A','B','A',0};

    assert(llsnSTRM(lhay, lneedle, 16, '|', -1) == lhay + 2);
    assert(lusnSTRM(lhay, uneedle, 16, '|', -1) == lhay + 2);
    assert(ulsnSTRM(uhay, lneedle, 16, '|', -1) == uhay + 2);
    assert(uusnSTRM(uhay, uneedle, 16, '|', -1) == uhay + 2);
}

static void test_search_empty_token_behavior(void) {
    const lchar_t empty_l[] = {0};
    const uchar_t empty_u[] = {0};
    const uchar_t one_u[] = {'a',0};
    const lchar_t one_delim_l[] = {'|',0};
    const uchar_t one_delim_u[] = {'|',0};

    assert(llsnstrm(empty_l, one_delim_l, 4, '|', 1) == empty_l);
    assert(lusnstrm(empty_l, one_delim_u, 4, '|', -1) == empty_l);
    assert(ulsnstrm(empty_u, one_delim_l, 4, '|', 1) == empty_u);
    assert(uusnstrm(empty_u, one_delim_u, 4, '|', -1) == empty_u);

    assert(llsnstrm((const lchar_t *)"a", empty_l, 4, 0, 1) == NULL);
    assert(lusnstrm((const lchar_t *)"a", empty_u, 4, 0, 1) == NULL);
    assert(ulsnstrm(one_u, empty_l, 4, 0, 1) == NULL);
    assert(uusnstrm(one_u, empty_u, 4, 0, 1) == NULL);
}

/**
 * @brief Run tests for compare and search functions.
 *
 * Tests current compare and string-search/count helpers.
 * Each block checks a specific function or family for
 * correct behavior and edge cases.
 */
void run_cmp_search_tests(void) {
    lchar_t l1[16] = {'a','b','c','\0'};
    lchar_t l2[16] = {'a','b','c','\0'};
    lchar_t l3[16] = {'a','b','d','\0'};

    // Compare.
    assert(llsnncmp(l1, 3, l2, 3) == 0);
    assert(llsnncmp(l1, 3, l3, 3) < 0);

    // Search/count.
    test_search_delim_negative_m_all_variants();
    test_search_charset_negative_m_all_variants();
    test_search_case_insensitive_variants();
    test_search_empty_token_behavior();
    assert(llsncnt((const lchar_t *)"abcabc", (const lchar_t *)"ab", 6, 0) == 2);

    printf("Comparison/search tests passed.\n");

}

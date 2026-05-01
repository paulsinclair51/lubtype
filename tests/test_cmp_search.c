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
    assert(llsncmp(l1, l2, 3) == 0);
    assert(llsncmp(l1, l3, 3) < 0);

    // Substring search/count.
    assert(llsnstrm(l1, (const lchar_t *)"bc", 3, '|', 1) == &l1[1]);
    assert(llsncnt((const lchar_t *)"abcabc", (const lchar_t *)"ab", 6, 0) == 2);

    // Null/empty/error edges on current APIs.
    assert(llsncmp(NULL, l2, 3) == LUB_CMP_LESS_THAN);
    assert(llsnstrm(NULL, l2, 3, 0, 1) == NULL);
    assert(llsncnt(NULL, l2, 3, 0) == 0);

    printf("Comparison/search tests passed.\n");

}

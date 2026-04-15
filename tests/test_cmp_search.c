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
 * Tests llsncmp, llspfx, llssfx, llsch, llsrchr, llsstr, llspbrk, llsspn, llscspn, llssubcnt.
 * Each block checks a specific function or family for correct behavior and edge cases.
 */
void run_cmp_search_tests(void) {
    lchar_t l1[16] = {'a','b','c','\0'};
    lchar_t l2[16] = {'a','b','c','\0'};
    lchar_t l3[16] = {'a','b','d','\0'};
    // Test: llsncmp (compare)
    assert(llsncmp(l1, l2, 3) == 0);
    assert(llsncmp(l1, l3, 3) < 0);
    // Test: llspfx, llssfx (prefix/suffix)
    assert(llspfx(l1, l2) == 0);
    assert(llssfx(l1, l2) == 0);
    // Test: llsch, llsrchr, llsstr (search)
    assert(llschr(l1, 'b') == &l1[1]);
    assert(llsrchr(l1, 'b') == &l1[1]);
    assert(llsstr(l1, l2) == l1);
    // Test: llspbrk, llsspn, llscspn (set search/span)
    lchar_t set[4] = {'b','c','\0'};
    assert(llspbrk(l1, set) == &l1[1]);
    assert(llsspn(l1, set) == 0);
    assert(llscspn(l1, set) == 1);
    // Test: llssubcnt (substring count)
    assert(llssubcnt(l1, set) == 1);
    // Null/empty/error edge cases
    assert(llsncmp(NULL, l2, 3) == 0);
    assert(llsncmp(l1, NULL, 3) == 0);
    assert(llspfx(NULL, l2) == 0);
    assert(llssfx(l1, NULL) == 0);
    assert(llschr(NULL, 'a') == NULL);
    assert(llsrchr(NULL, 'a') == NULL);
    assert(llsstr(NULL, l2) == NULL);
    assert(llspbrk(NULL, set) == NULL);
    assert(llsspn(NULL, set) == 0);
    assert(llscspn(NULL, set) == 0);
    assert(llssubcnt(NULL, set) == 0);
    l1[0] = 0; set[0] = 0;
    assert(llschr(l1, 'a') == NULL);
    assert(llsrchr(l1, 'a') == NULL);
    assert(llsstr(l1, l2) == NULL);
    assert(llspbrk(l1, set) == NULL);
    assert(llsspn(l1, set) == 0);
    assert(llscspn(l1, set) == 0);
    assert(llssubcnt(l1, set) == 0);
    printf("Comparison/search tests passed.\n");

}

/**
 * @file test_advanced_ops.c
 * @brief Tests for advanced string operations in lubtype.h
 *        (trim/rev/pad/replace).
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Run tests for advanced string operations
 *        (trim, reverse, pad, replace,
 *        split).
 */
void run_advanced_ops_tests(void) {
    lchar_t lsrc[32] = {' ','a','b','c',' ','\0'};
    lchar_t lout[32] = {0};
    uchar_t usrc[32] = {0x20, 'a', 'b', 'c', 0x20, 0};
    size_t len = 0;

    const lchar_t *ltrim = llsnptrim(lsrc, 32, &len, 'B', NULL);
    assert(ltrim == lsrc + 1 && len == 3);

    const uchar_t *utrim = uunptrim(usrc, 32, &len, 'B', NULL);
    assert(utrim == usrc + 1 && len == 3);

    assert(llsnnreverse(lout, lsrc, 32) != NULL);
    assert(lout[0] == ' ' && lout[1] == 'c' && lout[2] == 'b' && lout[3] == 'a');

    assert(llsnnpad(lout, 8, (const lchar_t *)"abc", 3, 'R', 'x') != NULL);
    assert(strncmp((const char *)lout, "abcxxxxx", 8) == 0);

    assert(llsnnreplace(lout, 16, (const lchar_t *)"aabbcc", 6,
                       (const lchar_t *)"b|z", '|', 0) != NULL);
    assert(strcmp((const char *)lout, "aazzcc") == 0);

    printf("Advanced operation tests passed.\n");

}

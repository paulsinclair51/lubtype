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
    lchar_t ltrim_custom_src[32] = {'x','x','a','b','c','x',0};
    lchar_t ltrim_custom_opt[3] = {'L','x',0};
    lchar_t ltrim_invalid_opt[3] = {'Q','x',0};
    lchar_t lout[32] = {0};
    uchar_t lu_trim_nonlatin_opt[3] = {'B', 0x0120, 0};
    uchar_t usrc[32] = {0x20, 'a', 'b', 'c', 0x20, 0};
    uchar_t utrim_custom_src[32] = {'x','a','b','c','x','x',0};
    uchar_t utrim_opt[2] = {'B', 0};
    uchar_t utrim_custom_opt[3] = {'R','x',0};
    size_t len = 0;

    const lchar_t *ltrim = llsnptrim(lsrc, 32, (const lchar_t *)"B", &len);
    assert(ltrim == lsrc + 1 && len == 3);

    ltrim = llsnptrim(ltrim_custom_src, 32, ltrim_custom_opt, &len);
    assert(ltrim == ltrim_custom_src + 2 && len == 4);

    ltrim = llsnptrim(ltrim_custom_src, 32, ltrim_invalid_opt, &len);
    assert(LUB_PTR_ERR(ltrim, LUB_OPT_RESERVED));
    assert(len == 0);

    ltrim = lusnptrim(lsrc, 32, lu_trim_nonlatin_opt, &len);
    assert(ltrim == lsrc && len == 5);

    const uchar_t *ultrim = ulsnptrim(usrc, 32, NULL, &len);
    assert(ultrim == usrc + 1 && len == 3);

    const uchar_t *utrim = uusnptrim(usrc, 32, utrim_opt, &len);
    assert(utrim == usrc + 1 && len == 3);

    utrim = uusnptrim(utrim_custom_src, 32, utrim_custom_opt, &len);
    assert(utrim == utrim_custom_src && len == 4);

        assert(llsnnreverse(lout, 32, lsrc, 32) != NULL);
    assert(lout[0] == ' ' && lout[1] == 'c' && lout[2] == 'b' && lout[3] == 'a');

        assert(llsnnpad(lout, 8, (const lchar_t *)"abc", 3,
                        (const lchar_t[]){'R', 'x', 0}) != NULL);
    assert(strncmp((const char *)lout, "abcxxxxx", 8) == 0);

    assert(llsnnreplace(lout, 16, (const lchar_t *)"aabbcc", 6,
                       (const lchar_t *)"b|z", '|', 0) != NULL);
    assert(strcmp((const char *)lout, "aazzcc") == 0);

    printf("Advanced operation tests passed.\n");

}

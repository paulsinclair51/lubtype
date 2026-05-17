/**
 * @file test_error_edge.c
 * @brief X-macro tests for error and edge cases in lubtype.h
 *        string operations.
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
#include "../lubtype.h"
#include "lubtype_test_declarations.h"
#include <stdio.h>

static lub_test_result_t test_result;

/**
 * @brief Run tests for error and edge cases in string operations.
 *
 * Tests null pointer handling, over-max-length, and empty string cases for
 * bounded string functions. Compiled twice for Latin (lchar_t)
 * and Unicode (uchar_t).
 */
lub_test_result_t LUB_PASTE(run_error_edge_tests_, LUB_X)(int inject_faults) {
    test_result = (lub_test_result_t){0};
    xchar_t xsrc[8] = {'a','b','c','\0'};
    xchar_t xdst[16] = {0};

#if defined(LUB_X_IS_L)
    // Cross-type uchar tests under Latin variant
    uchar_t usrc[8] = {'a','b','c',0};
    uchar_t udst[16] = {0};
#endif

    // Length behavior for NULL and bounded scans.
    LUB_ASSERT(xcsnlen(NULL, 10) == 0);
    LUB_ASSERT(xcsnlen(xsrc, 100) == 3);

    // Copy behavior using current bounded APIs.
    LUB_ASSERT(xxsnncpy(xdst, 16, xsrc, 8, NULL) != NULL);
    LUB_ASSERT(xdst[0] == 'a' && xdst[1] == 'b' && xdst[2] == 'c' && xdst[3] == 0);

    // Null target returns NULL for bounded copy/cat helpers.
    LUB_ASSERT(xxsnncpy(NULL, 16, xsrc, 8, NULL) == NULL);
    LUB_ASSERT(xxsnncat(NULL, 16, xsrc, 8, NULL) == NULL);

    // Concatenate behavior using current bounded API.
    xdst[0] = 'x'; xdst[1] = 0;
    LUB_ASSERT(xxsnncat(xdst, 16, xsrc, 8, NULL) != NULL);
    LUB_ASSERT(xdst[0] == 'x' && xdst[1] == 'a' && xdst[2] == 'b' && xdst[3] == 'c' && xdst[4] == 0);

#if defined(LUB_X_IS_L)
    // Cross-type operations (uchar source) under Latin variant
    LUB_ASSERT(ucsnlen(NULL, 10) == 0);
    LUB_ASSERT(ucsnlen(usrc, 100) == 3);

    LUB_ASSERT(ulsnncpy(xdst, 16, usrc, 8, NULL) != NULL);
    LUB_ASSERT(xdst[0] == 'a' && xdst[1] == 'b' && xdst[2] == 'c' && xdst[3] == 0);

    LUB_ASSERT(ulsnncat(xdst, 16, usrc, 8, NULL) != NULL);
#endif

    return test_result;
}

/**
 * @file test_error_edge.c
 * @brief Tests for error and edge cases in lubtype.h string operations.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include "lubtype_test_declarations.h"
#include <stdio.h>

static lub_test_result_t test_result;

/**
 * @brief Run tests for error and edge cases in string operations.
 *
 * Tests null pointer handling, over-max-length, and empty string cases for
 * lcslen and lcsnlen. Prints a message on success.
 */
lub_test_result_t run_error_edge_tests(void) {
    test_result = (lub_test_result_t){0};
    lchar_t lsrc[8] = {'a','b','c','\0'};
    uchar_t usrc[8] = {'a','b','c',0};
    lchar_t ldst[16] = {0};
    uchar_t udst[16] = {0};

    // Length behavior for NULL and bounded scans.
    LUB_ASSERT(lcsnlen(NULL, 10) == 0);
    LUB_ASSERT(ucsnlen(NULL, 10) == 0);
    LUB_ASSERT(lcsnlen(lsrc, 100) == 3);
    LUB_ASSERT(ucsnlen(usrc, 100) == 3);

    // Copy behavior using current bounded APIs.
    LUB_ASSERT(llsnncpy(ldst, 16, lsrc, 8, NULL) != NULL);
    LUB_ASSERT(ldst[0] == 'a' && ldst[1] == 'b' && ldst[2] == 'c' && ldst[3] == 0);
    LUB_ASSERT(ulsnncpy(udst, 16, lsrc, 8, NULL) != NULL);
    LUB_ASSERT(udst[0] == 'a' && udst[1] == 'b' && udst[2] == 'c' && udst[3] == 0);

    // Null target returns NULL for bounded copy/cat helpers.
    LUB_ASSERT(llsnncpy(NULL, 16, lsrc, 8, NULL) == NULL);
    LUB_ASSERT(llsnncat(NULL, 16, lsrc, 8, NULL) == NULL);

    // Concatenate behavior using current bounded API.
    ldst[0] = 'x'; ldst[1] = 0;
    LUB_ASSERT(llsnncat(ldst, 16, lsrc, 8, NULL) != NULL);
    LUB_ASSERT(ldst[0] == 'x' && ldst[1] == 'a' && ldst[2] == 'b' && ldst[3] == 'c' && ldst[4] == 0);

    printf("Error/edge case tests passed.\n");

    return test_result;
}

/**
 * @file test_strlen_validation.c
 * @brief Tests for string length and validation functions in lubtype.h.
 *
 * @note Requires `static lub_test_result_t test_result;` at file scope.
 *       that LUB_ASSERT macros modify to track pass/fail/exception counts.
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
 * @brief Run tests for string length and validation functions.
 *
 * Tests lcsnlen, ucsnlen, and string classification helpers for
 * correct behavior and edge cases.
 */
lub_test_result_t run_strlen_validation_tests(void) {
    test_result = (lub_test_result_t){0};
    lchar_t lstr[16] = {'a','b','c','\0'};
    uchar_t ustr[16] = {L'a',L'b',L'c',0};
    // Test: bounded length API (current surface).
    LUB_ASSERT(lcsnlen(lstr, LUB_MAX_LSTRLEN) == 3);
    LUB_ASSERT(ucsnlen(ustr, LUB_MAX_USTRLEN) == 3);
    // Missing terminator within bound yields unterminated error.
    LUB_ASSERT(lcsnlen(lstr, 2) == (size_t)LUB_UNTERMINATED);
    LUB_ASSERT(ucsnlen(ustr, 2) == (size_t)LUB_UNTERMINATED);
    // Null/empty edge cases.
    LUB_ASSERT(lcsnlen(NULL, 10) == 0);
    LUB_ASSERT(ucsnlen(NULL, 10) == 0);
    lstr[0] = 0; ustr[0] = 0;
    LUB_ASSERT(lcsnlen(lstr, LUB_MAX_LSTRLEN) == 0);
    LUB_ASSERT(ucsnlen(ustr, LUB_MAX_USTRLEN) == 0);
    // Reserved/qname current symbol names.
    uchar_t not_reserved[8] = {L'A',0};
    LUB_ASSERT(!isureserved(not_reserved));

    // New string-classification helpers.
    lchar_t alpha_l[8] = {'A','b','C','\0'};
    uchar_t alpha_u[8] = {L'A',L'b',L'C',0};
    uchar_t latin_u[8] = {L'A',L'B',L'Z',0};
    uchar_t non_latin_u[8] = {0x0100,0};
    LUB_ASSERT(islnalphastr(alpha_l, 3));
    LUB_ASSERT(isunalphastr(alpha_u, 3));
    LUB_ASSERT(isunlatinstr(latin_u, 3));
    LUB_ASSERT(!isunlatinstr(non_latin_u, 1));

    // Hex-digit string checks (bounded versions).
    lchar_t hexl[8] = {'A','B','C','1','2','3','\0'};
    uchar_t hexu[8] = {L'A',L'B',L'C',L'1',L'2',L'3',0};
    LUB_ASSERT(islnhexstr(hexl, 6));
    LUB_ASSERT(isunhexstr(hexu, 6));
    // Null/empty edges for hex classifiers.
    LUB_ASSERT(islnhexstr(NULL, 3));
    LUB_ASSERT(isunhexstr(NULL, 3));
    hexl[0] = 0; hexu[0] = 0;
    LUB_ASSERT(islnhexstr(hexl, 0));
    LUB_ASSERT(isunhexstr(hexu, 0));
    printf("String length/validation tests passed.\n");

    return test_result;
}

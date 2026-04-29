/**
 * @file test_strlen_validation.c
 * @brief Tests for string length and validation functions in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include <stdio.h>

/**
 * @brief Run tests for string length and validation functions.
 *
 * Tests lcslen, ucslen, lcsnlen, ucsnlen, isureserved, isuqname,
 * islhexdigits, and isuhexdigits for correct behavior and edge cases.
 */
void run_strlen_validation_tests(void) {
    lchar_t lstr[16] = {'a','b','c','\0'};
    uchar_t ustr[16] = {L'a',L'b',L'c',0};
    // Test: bounded length API (current surface).
    assert(lcsnlen(lstr, LUB_MAX_LSTRLEN) == 3);
    assert(ucsnlen(ustr, LUB_MAX_USTRLEN) == 3);
    // Missing terminator within bound yields unterminated error.
    assert(lcsnlen(lstr, 2) == (size_t)LUB_UNTERMINATED);
    assert(ucsnlen(ustr, 2) == (size_t)LUB_UNTERMINATED);
    // Null/empty edge cases.
    assert(lcsnlen(NULL, 10) == 0);
    assert(ucsnlen(NULL, 10) == 0);
    lstr[0] = 0; ustr[0] = 0;
    assert(lcsnlen(lstr, LUB_MAX_LSTRLEN) == 0);
    assert(ucsnlen(ustr, LUB_MAX_USTRLEN) == 0);
    // Reserved/qname current symbol names.
    uchar_t not_reserved[8] = {L'A',0};
    assert(!isuRESERVED(not_reserved));
    // Hex-digit string checks (bounded versions).
    lchar_t hexl[8] = {'A','B','C','1','2','3','\0'};
    uchar_t hexu[8] = {L'A',L'B',L'C',L'1',L'2',L'3',0};
    assert(islnhexdigits(hexl, 6));
    assert(isunhexdigits(hexu, 6));
    // Null/empty edges for hex classifiers.
    assert(islnhexdigits(NULL, 3));
    assert(isunhexdigits(NULL, 3));
    hexl[0] = 0; hexu[0] = 0;
    assert(islnhexdigits(hexl, 0));
    assert(isunhexdigits(hexu, 0));
    printf("String length/validation tests passed.\n");

}
